#ifndef PCI_COMMANDS_H
#define PCI_COMMANDS_H

#include "pci-types.h"

void cmd_select_device(char dev_addr[]);
int cmd_get_configs(const char dev_addr[], pci_config_t *pconf);
void cmd_print_configs(FILE *fp, pci_config_t *pconf, const char *sep);
void cmd_read_all_configs_to_file(void);

#endif