#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// local file includes
#include "pci-commands.h"
#include "sys-utils.h"
#include "pci-types.h"



const char *PCIe_CommonParams[] = {
	"Vendor ID       : ", /*  0 */
	"Device ID       : ", /*  1 */
	"Device Type     : ", /*  2 */
	"Device Details  : ", /*  3 */
	"Command Reg     : ", /*  4 */
	"Status Reg      : ", /*  5 */
	"Revision ID     : ", /*  6 */
	"Class Code      : ", /*  7 */
	"Cache Line      : ", /*  8 */
	"Latency Timer   : ", /*  9 */
	"Header Type     : ", /* 10 */
	"BIST            : ", /* 11 */
	"Capability Ptr  : ", /* 12 */
	"IRQ Line        : ", /* 13 */
	"IRQ Pin         : "  /* 14 */
};
#define PCIE_COMN_PAR_SIZE	15

void print_comn_cfg_params(FILE *fp, pci_cfg_type0_t *pconf, prnt_t prnt_dir, const char *sep) {
	fprintf(fp, "%s0x%X%s",  ((prnt_dir == PRNT_COL) ? PCIe_CommonParams[0] : ""), pconf->cmn.vendor_id, sep);
	fprintf(fp, "%s0x%X%s",  ((prnt_dir == PRNT_COL) ? PCIe_CommonParams[1] : ""), pconf->cmn.device_id, sep);
	fprintf(fp, "%s%s%s",    ((prnt_dir == PRNT_COL) ? PCIe_CommonParams[2] : ""), pconf->cmn.device_type, sep);
	fprintf(fp, "%s%s%s",    ((prnt_dir == PRNT_COL) ? PCIe_CommonParams[3] : ""), pconf->cmn.device_str, sep);
	fprintf(fp, "%s0x%X%s",  ((prnt_dir == PRNT_COL) ? PCIe_CommonParams[4] : ""), pconf->cmn.cmd_reg, sep);
	fprintf(fp, "%s0x%X%s",  ((prnt_dir == PRNT_COL) ? PCIe_CommonParams[5] : ""), pconf->cmn.stat_reg, sep);
	fprintf(fp, "%s%d%s",    ((prnt_dir == PRNT_COL) ? PCIe_CommonParams[6] : ""), pconf->cmn.rev_id, sep);
	fprintf(fp, "%s0x%X%s",  ((prnt_dir == PRNT_COL) ? PCIe_CommonParams[7] : ""), pconf->cmn.class_code, sep);
	fprintf(fp, "%s0x%X%s",  ((prnt_dir == PRNT_COL) ? PCIe_CommonParams[8] : ""), pconf->cmn.cache_line, sep);
	fprintf(fp, "%s0x%X%s",  ((prnt_dir == PRNT_COL) ? PCIe_CommonParams[9] : ""), pconf->cmn.lat_timer, sep);
	fprintf(fp, "%s0x%X%s",  ((prnt_dir == PRNT_COL) ? PCIe_CommonParams[10] : ""), pconf->cmn.hdr_type, sep);
	fprintf(fp, "%s0x%X%s",  ((prnt_dir == PRNT_COL) ? PCIe_CommonParams[11] : ""), pconf->cmn.bist, sep);
	fprintf(fp, "%s0x%X%s",  ((prnt_dir == PRNT_COL) ? PCIe_CommonParams[12] : ""), pconf->cmn.cap_ptr, sep);
	fprintf(fp, "%s0x%X%s",  ((prnt_dir == PRNT_COL) ? PCIe_CommonParams[13] : ""), pconf->cmn.irq_line, sep);
	fprintf(fp, "%s0x%X%s",  ((prnt_dir == PRNT_COL) ? PCIe_CommonParams[14] : ""), pconf->cmn.irq_pin, sep);
}



const char *PCIe_Type0Params[] = {
	"BAR0            : ", /* 0 */
	"BAR1            : ", /* 1 */
	"BAR2            : ", /* 2 */
	"BAR3            : ", /* 3 */
	"BAR4            : ", /* 4 */
	"BAR5            : ", /* 5 */
	"Cardbus CIS Ptr : ", /* 6 */
	"Subs. Vendor ID : ", /* 7 */
	"Subs. Device ID : ", /* 8 */
};
#define PCIE_TYP0_PAR_SIZE	9

void print_type0_cfg_params(FILE *fp, pci_cfg_type0_t *pconf, prnt_t prnt_dir, const char *sep) {
	fprintf(fp, "%s0x%X%s",  ((prnt_dir == PRNT_COL) ? PCIe_Type0Params[0] : ""), pconf->bar0, sep);
	fprintf(fp, "%s0x%X%s",  ((prnt_dir == PRNT_COL) ? PCIe_Type0Params[1] : ""), pconf->bar1, sep);
	fprintf(fp, "%s0x%X%s",  ((prnt_dir == PRNT_COL) ? PCIe_Type0Params[2] : ""), pconf->bar2, sep);
	fprintf(fp, "%s0x%X%s",  ((prnt_dir == PRNT_COL) ? PCIe_Type0Params[3] : ""), pconf->bar3, sep);
	fprintf(fp, "%s0x%X%s",  ((prnt_dir == PRNT_COL) ? PCIe_Type0Params[4] : ""), pconf->bar4, sep);
	fprintf(fp, "%s0x%X%s",  ((prnt_dir == PRNT_COL) ? PCIe_Type0Params[5] : ""), pconf->bar5, sep);
	fprintf(fp, "%s0x%X%s",  ((prnt_dir == PRNT_COL) ? PCIe_Type0Params[6] : ""), pconf->card_bus_p, sep);
	fprintf(fp, "%s0x%X%s",  ((prnt_dir == PRNT_COL) ? PCIe_Type0Params[7] : ""), pconf->subsys_vid, sep);
	fprintf(fp, "%s0x%X%s",  ((prnt_dir == PRNT_COL) ? PCIe_Type0Params[8] : ""), pconf->subsys_did, sep);
}


void print_config_type0_param_title(FILE *fp) {
	// print common parameter title
	for (int i = 0; i < PCIE_COMN_PAR_SIZE; i++) {
		fprintf(fp, "%s|", PCIe_CommonParams[i]);
	}

	// print type 0 parameter title
	for (int i = 0; i < PCIE_TYP0_PAR_SIZE; i++) {
		fprintf(fp, "%s|", PCIe_Type0Params[i]);
	}

	fprintf(fp, "\n");
}



