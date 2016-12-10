/** @author Levente Kurusa <levex@linux.com> **/
#ifndef PCI_H_
#define PCI_H_

#include <kernel/fos.h>
#include <drivers/pci_list.h>
struct __pci_driver;

typedef struct {
	uint32_t vendor;
	uint32_t device;
	uint16_t headerType;
	uint16_t class;
	uint16_t subClass;
	uint16_t func;
	
	struct __pci_driver *driver;
} pci_device;

typedef struct {
	uint32_t vendor;
	uint32_t device;
	uint32_t func;
} pci_device_id;

typedef struct __pci_driver {
	pci_device_id *table;
	char *name;
	uint8_t (*init_one)(pci_device *);
	uint8_t (*init_driver)(void);
	uint8_t (*exit_driver)(void);
} pci_driver;

extern void pci_install();
extern void pci_proc_dump();

#endif
