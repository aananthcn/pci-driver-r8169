#ifndef PCI_COMMANDS_H
#define PCI_COMMANDS_H

#include "pci-types.h"


typedef enum {
	PRNT_ROW, /* print config parameter in a column */
	PRNT_COL, /* print config parameter in a row */
	PRNT_MAX
} prnt_t;



void cmd_select_device(char dev_addr[]);
int  cmd_get_config_header(const char dev_addr[], pci_cfg_type0_t *pconf);
void cmd_get_all_configs_to_file(void);


void cmd_print_configs(FILE *fp, pci_cfg_type0_t *pconf, prnt_t prnt_dir);

#endif