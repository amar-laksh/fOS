// TODO - add full support for pci_list.h
/*
FIRST STEP - PARSE ALL REQUIRED CONFIG FIELDS

VENDOR_D, DEVICE_ID, REVISION_ID. HEADER TYPE
, CLASS CODE


SECOND STEP - (HEADER TYPE)IDENTIFY IF DEVICE IS-
(BY BITS 6-0)
        (00H)AN ENDPOINT
        (01H)A PCI-TO-PCI BRIDGE
        (02H)A CARDBUS BRIDGE

THIRD STEP - IDENTIFY IF DEVICE IS MULTI OR
SINGLE FUNCITON

IF BIT-7 == 0= SINGLE FUNCTION
ELSE MULTI FUNCITON

FOURTH STEP - PARSE APPROPRIATE CONFIG HEADER FOR
DISCOVERD DEVICE.






*/
#include <drivers/pci_list.h>
#include <kernel/fos.h>
pci_device** pci_devices = 0;
uint32_t devs = 0;

pci_driver** pci_drivers = 0;
uint32_t drivs = 0;

void
add_pci_device(pci_device* pdev)
{
  pci_devices[devs] = pdev;
  devs++;
  return;
}

uint16_t
pci_read_word(uint16_t bus, uint16_t slot, uint16_t func, uint16_t offset)
{
  uint32_t address;
  uint32_t lbus = (uint32_t)bus;
  uint32_t lslot = (uint32_t)slot;
  uint32_t lfunc = (uint32_t)func;
  uint16_t tmp = 0;
  address = (uint32_t)((lbus << 16) | (lslot << 11) | (lfunc << 8) |
                       (offset & 0xfc) | ((uint32_t)0x80000000));

  outl(0xCF8, address);
  tmp = (uint16_t)((inl(0xCFC) >> ((offset & 2) * 8)) & 0xffff);
  return (tmp);
}

uint16_t
getVendorID(uint16_t bus, uint16_t device, uint16_t function)
{

  return pci_read_word(bus, device, function, 0x0);
}

uint16_t
getDeviceID(uint16_t bus, uint16_t device, uint16_t function)
{

  return pci_read_word(bus, device, function, 0x2);
}

uint8_t
getClassID(uint16_t bus, uint16_t device, uint16_t function)
{

  return (pci_read_word(bus, device, function, 0xA) & ~0x00FF) >> 8;
}

uint8_t
getSubClassID(uint16_t bus, uint16_t device, uint16_t function)
{

  return (pci_read_word(bus, device, function, 0xA) & ~0xFF00);
}

uint8_t
getHeaderType(uint16_t bus, uint16_t device, uint16_t function)
{
  return pci_read_word(bus, device, function, 0x0E);
}

uint32_t
getBAR0(uint16_t bus, uint16_t device, uint16_t function)
{
  return pci_read_word(bus, device, function, 0x10) & 0xFFFFFFF0;
}
void
pci_probe()
{
  for (uint32_t bus = 0; bus < 256; bus++) {
    for (uint32_t slot = 0; slot < 32; slot++) {
      for (uint32_t function = 0; function < 8; function++) {
        uint16_t vendor = getVendorID(bus, slot, function);
        if (vendor == 0xFFFF)
          continue;
        uint8_t headertype = getHeaderType(bus, slot, function);
        uint16_t device = getDeviceID(bus, slot, function);
        uint8_t class = getClassID(bus, slot, function);
        uint8_t subClass = getSubClassID(bus, slot, function);
        uint32_t BAR0 = getBAR0(bus, slot, function);

        pci_device* pdev = (pci_device*)malloc(sizeof(pci_device));
        pdev->vendor = vendor;
        pdev->device = device;
        pdev->headerType = headertype;
        pdev->class = class;
        pdev->subClass = subClass;
        pdev->func = function;
        pdev->driver = 0;
        pdev->BAR0 = BAR0;
        /**
        sprintf("%x-->[%x:%x:%x]::[%x:%x]\n"
        , pdev->headerType
        , pdev->vendor
        , pdev->device
        , pdev->func
        , pdev->class
        , pdev->subClass);
        **/
        add_pci_device(pdev);
      }
    }
  }
}

void
pci_install()
{
  devs = drivs = 0;
  pci_devices = (pci_device**)malloc(32 * sizeof(pci_device));
  pci_drivers = (pci_driver**)malloc(32 * sizeof(pci_driver));
  pci_probe();
}

void
pci_register_driver(pci_driver* driv)
{
  pci_drivers[drivs] = driv;
  drivs++;
  return;
}

void
getVendorName(pci_device* pci_dev)
{
  for (uint32_t j = 0; j < PCI_VENDOR_TABLE_LEN; j++) {
    if (pci_dev->vendor == vendor_table[j].ven_ID) {
      kprintf("\nVendor Name: %s", vendor_table[j].ven_full);
      break;
    }
  }
}

void
getDeviceName(pci_device* pci_dev)
{
  for (unsigned long m = 0; m < PCI_DEVICE_TABLE_LEN; m++) {
    if (pci_dev->vendor == device_table[m].ven_ID &&
        pci_dev->device == device_table[m].dev_ID) {
      kprintf("\nDevice Name: %s", device_table[m].chip_desc);
      break;
    }
  }
}

void
getClasses(pci_device* pci_dev)
{
  for (unsigned long k = 0; k < PCI_CLASS_CODE_TABLE_LEN; k++) {
    if (pci_dev->class == class_code_table[k].base_class &&
        pci_dev->subClass == class_code_table[k].sub_class) {
      kprintf("\nBase Class: %s\nSub-Class: %s",
              class_code_table[k].base_desc,
              class_code_table[k].sub_desc);
      break;
    }
  }
}
void
pci_proc_dump(int start)
{
  kprintf("\nTotal Devices found: %d\n", devs);
  for (uint32_t i = start; i < devs; i++) {
    pci_device* pci_dev = pci_devices[i];
    if (pci_dev->driver) {
      getVendorName(pci_dev);
      getDeviceName(pci_dev);
      getClasses(pci_dev);
      kprintf("[%x:%x:%x] => %s\n",
              pci_dev->vendor,
              pci_dev->device,
              pci_dev->func,
              pci_dev->driver->name);
    } else {
      getVendorName(pci_dev);
      getDeviceName(pci_dev);
      getClasses(pci_dev);
      kprintf("\n%x==>[%x:%x]:[%x:%x:%x]:[%x]\n",
              pci_dev->headerType,
              pci_dev->vendor,
              pci_dev->device,
              pci_dev->class,
              pci_dev->subClass,
              pci_dev->func,
              pci_dev->BAR0);
    }
  }
  return;
}
