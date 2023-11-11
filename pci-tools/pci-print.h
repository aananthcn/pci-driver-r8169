#ifndef PCI_PRINT_H
#define PCI_PRINT_H

#include "pci-types.h"

void print_comn_cfg_params(FILE *fp, pci_cfg_type0_t *pconf, prnt_t prnt_dir, const char *sep);
void print_type0_cfg_params(FILE *fp, pci_cfg_type0_t *pconf, prnt_t prnt_dir, const char *sep);

void print_config_type0_param_title(FILE *fp);


#endif