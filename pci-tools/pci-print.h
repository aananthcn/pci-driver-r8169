#ifndef PCI_PRINT_H
#define PCI_PRINT_H

#include "pci-types.h"

void print_comn_cfg_params(FILE *fp, pci_cfg_t *pconf, prnt_t prnt_dir, const char *sep);
void print_type0_cfg_params(FILE *fp, pci_cfg_t *pconf, prnt_t prnt_dir, const char *sep);
void print_type1_cfg_params(FILE *fp, pci_cfg_t *pconf, prnt_t prnt_dir, const char *sep);

void print_config_type0_param_title(FILE *fp);
void print_config_type1_param_title(FILE *fp);

void print_string(FILE *fp, const char *str);

void print_pwr_mgmt_cap_params(FILE *fp, pci_pmr_mgmt_cap_t *pconf, prnt_t prnt_dir, const char *sep);
void print_ext_cap_params(FILE *fp, pci_ext_cap_t *pconf, prnt_t prnt_dir, const char *sep);


#endif