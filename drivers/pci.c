// TODO - add full support for pci_list.h
#include <drivers/pci.h>
pci_device **pci_devices = 0;
uint32_t devs = 0;

pci_driver **pci_drivers = 0;
uint32_t drivs = 0;


void add_pci_device(pci_device *pdev)
{
	pci_devices[devs] = pdev;
	devs ++;
	return;
}

uint16_t pci_read_word(uint16_t bus, uint16_t slot, uint16_t func, uint16_t offset){
	uint64_t address;
	uint64_t lbus = (uint64_t)bus;
	uint64_t lslot = (uint64_t)slot;
	uint64_t lfunc = (uint64_t)func;
	uint16_t tmp = 0;
	address = (uint64_t)((lbus << 16) | (lslot << 11) |
			(lfunc << 8) | (offset & 0xfc) | ((uint32_t)0x80000000));
	outl (0xCF8, address);
	tmp = (uint16_t)((inl (0xCFC) >> ((offset & 2) * 8)) & 0xffff);
	return (tmp);
}

uint16_t getVendorID(uint16_t bus, uint16_t device, uint16_t function)
{
        uint32_t r0 = pci_read_word(bus,device,function,0);
        return r0;
}

uint16_t getDeviceID(uint16_t bus, uint16_t device, uint16_t function)
{
        uint32_t r0 = pci_read_word(bus,device,function,2);
        return r0;
}

uint16_t getClassID(uint16_t bus, uint16_t device, uint16_t function)
{
        uint32_t r0 = pci_read_word(bus,device,function,0xA);
        return (r0 & ~0x00FF) >> 8;
}

uint16_t getSubClassID(uint16_t bus, uint16_t device, uint16_t function)
{
        uint32_t r0 = pci_read_word(bus,device,function,0xA);
        return (r0 & ~0xFF00);
}

void pci_probe()
{
	for(uint32_t bus = 0; bus < 256; bus++)
    {
        for(uint32_t slot = 0; slot < 32; slot++)
        {
            for(uint32_t function = 0; function < 8; function++)
            {
                    uint16_t vendor = getVendorID(bus, slot, function);
                    if(vendor == 0xFFFF) continue;
                    uint16_t device = getDeviceID(bus, slot, function);
                    uint16_t class = getClassID(bus, slot, function);
                    uint16_t subClass = getSubClassID(bus, slot, function);
                    //dprintf("vendor|device = %d | %d\n",vendor,device);
                    pci_device *pdev = (pci_device *)malloc(sizeof(pci_device));
                    pdev->vendor = vendor;
                    pdev->class =  class;
                    pdev->subClass = subClass;
                    pdev->device = device;
                    pdev->func = function;
                    pdev->driver = 0;
                    add_pci_device(pdev);
            }
        }
    }
}

 uint16_t pciCheckVendor(uint16_t bus, uint16_t slot)
 {
    uint16_t vendor,device;
        /* check if device is valid */
    if ((vendor = pci_read_word(bus,slot,0,0)) != 0xFFFF) {
       device = pci_read_word(bus,slot,0,2);
       /* valid device */
    } return (vendor);
 }


void pci_install()
{
	devs = drivs = 0;
	pci_devices = (pci_device **)malloc(32 * sizeof(pci_device));
	pci_drivers = (pci_driver **)malloc(32 * sizeof(pci_driver));
	pci_probe();
}

void pci_register_driver(pci_driver *driv)
{
	pci_drivers[drivs] = driv;
	drivs ++;
	return;
}

void pci_proc_dump(int start)
{
	for(int i = start; i < devs; i++)
	{
		pci_device *pci_dev = pci_devices[i];
        for(int j=0;j< PCI_VENDOR_TABLE_LEN ; j++){
            if(pci_dev->vendor == vendor_table[j].ven_ID){
                if(pci_dev->driver){
                    dprintf("Vendor Name: %s\n",vendor_table[j].ven_full);
                    dprintf("[%x:%x:%x] => %s\n"
                        ,pci_dev->vendor
                        ,pci_dev->device
                        ,pci_dev->func
                        ,pci_dev->driver->name);
                 }   
                else{
                    dprintf("\nVendor Name: %s"
                        ,vendor_table[j].ven_full);
                    for (int m = 0; m < PCI_DEVICE_TABLE_LEN; m++){
                        if(pci_dev->vendor == device_table[m].ven_ID
                            && pci_dev->device == device_table[m].dev_ID)
                            dprintf("\nDevice Name: %s"
                                ,device_table[m].chip_desc);
                    }
                    for (int k = 0; k < PCI_CLASS_CODE_TABLE_LEN; k++){
                        if(pci_dev->class == class_code_table[k].base_class 
                            && pci_dev->subClass == class_code_table[k].sub_class)
                            dprintf("\nBase Class: %s\nSub-Class: %s\n"
                                ,class_code_table[k].base_desc
                                ,class_code_table[k].sub_desc);
                }
                dprintf("[%x:%x:%x]\n", pci_dev->vendor, pci_dev->device, pci_dev->func);
            }
            }
        }
	}
    return;
}
