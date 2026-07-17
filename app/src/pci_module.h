#ifndef PCI_MODULE_H
#define PCI_MODULE_H

void pci_reset(void);
void pci_update_hr(int hr);
void pci_update_vel_z(int vel_z);
int pci_get_value(void);

#endif
