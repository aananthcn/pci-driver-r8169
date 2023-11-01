#ifndef PCI_COMMANDS_H
#define PCI_COMMANDS_H

void cmd_select_device(char dev_addr[]);
void cmd_print_configs(const char dev_addr[]);

#endif