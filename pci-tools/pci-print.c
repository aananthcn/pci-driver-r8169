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

void print_comn_cfg_params(FILE *fp, pci_cfg_t *pconf, prnt_t prnt_dir, const char *sep) {
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
	"Exp. ROM Base   : ", /* 9 */
};
#define PCIE_TYP0_PAR_SIZE	10

void print_type0_cfg_params(FILE *fp, pci_cfg_t *pconf, prnt_t prnt_dir, const char *sep) {
	fprintf(fp, "%s0x%X%s",  ((prnt_dir == PRNT_COL) ? PCIe_Type0Params[0] : ""), pconf->u.t0.bar0, sep);
	fprintf(fp, "%s0x%X%s",  ((prnt_dir == PRNT_COL) ? PCIe_Type0Params[1] : ""), pconf->u.t0.bar1, sep);
	fprintf(fp, "%s0x%X%s",  ((prnt_dir == PRNT_COL) ? PCIe_Type0Params[2] : ""), pconf->u.t0.bar2, sep);
	fprintf(fp, "%s0x%X%s",  ((prnt_dir == PRNT_COL) ? PCIe_Type0Params[3] : ""), pconf->u.t0.bar3, sep);
	fprintf(fp, "%s0x%X%s",  ((prnt_dir == PRNT_COL) ? PCIe_Type0Params[4] : ""), pconf->u.t0.bar4, sep);
	fprintf(fp, "%s0x%X%s",  ((prnt_dir == PRNT_COL) ? PCIe_Type0Params[5] : ""), pconf->u.t0.bar5, sep);
	fprintf(fp, "%s0x%X%s",  ((prnt_dir == PRNT_COL) ? PCIe_Type0Params[6] : ""), pconf->u.t0.card_bus_p, sep);
	fprintf(fp, "%s0x%X%s",  ((prnt_dir == PRNT_COL) ? PCIe_Type0Params[7] : ""), pconf->u.t0.subsys_vid, sep);
	fprintf(fp, "%s0x%X%s",  ((prnt_dir == PRNT_COL) ? PCIe_Type0Params[8] : ""), pconf->u.t0.subsys_did, sep);
	fprintf(fp, "%s0x%X%s",  ((prnt_dir == PRNT_COL) ? PCIe_Type0Params[9] : ""), pconf->u.t0.exp_rom_ba, sep);
}



const char *PCIe_Type1Params[] = {
	"BAR0            : ", /*  0 */
	"BAR1            : ", /*  1 */
	"Pri. Bus Number : ", /*  2 */
	"Sec. Bus Number : ", /*  3 */
	"SubOrd. Bus No  : ", /*  4 */
	"Sec. Lat. Timer : ", /*  5 */
	"I/O Base        : ", /*  6 */
	"I/O Limit       : ", /*  7 */
	"Sec. Status     : ", /*  8 */
	"Mem. Base       : ", /*  9 */
	"Mem. Limit      : ", /* 10 */
	"Prftch Mem Base : ", /* 11 */
	"Prftch Mem Limt : ", /* 12 */
	"Prftch Base U32 : ", /* 13 */
	"Prftch Limt U32 : ", /* 14 */
	"I/O Base U16    : ", /* 15 */
	"I/O Limit U16   : ", /* 16 */
	"Exp. ROM Base   : ", /* 17 */
	"Bridge Control  : ", /* 18 */
};

#define PCIE_TYP1_PAR_SIZE	19

void print_type1_cfg_params(FILE *fp, pci_cfg_t *pconf, prnt_t prnt_dir, const char *sep) {
	fprintf(fp, "%s0x%X%s",  ((prnt_dir == PRNT_COL) ? PCIe_Type1Params[0] : ""), pconf->u.t1.bar0, sep);
	fprintf(fp, "%s0x%X%s",  ((prnt_dir == PRNT_COL) ? PCIe_Type1Params[1] : ""), pconf->u.t1.bar1, sep);
	fprintf(fp, "%s0x%X%s",  ((prnt_dir == PRNT_COL) ? PCIe_Type1Params[2] : ""), pconf->u.t1.pri_bus_n, sep);
	fprintf(fp, "%s0x%X%s",  ((prnt_dir == PRNT_COL) ? PCIe_Type1Params[3] : ""), pconf->u.t1.sec_bus_n, sep);
	fprintf(fp, "%s0x%X%s",  ((prnt_dir == PRNT_COL) ? PCIe_Type1Params[4] : ""), pconf->u.t1.sbo_bus_n, sep);
	fprintf(fp, "%s0x%X%s",  ((prnt_dir == PRNT_COL) ? PCIe_Type1Params[5] : ""), pconf->u.t1.sec_lat_tm, sep);
	fprintf(fp, "%s0x%X%s",  ((prnt_dir == PRNT_COL) ? PCIe_Type1Params[6] : ""), pconf->u.t1.io_base, sep);
	fprintf(fp, "%s0x%X%s",  ((prnt_dir == PRNT_COL) ? PCIe_Type1Params[7] : ""), pconf->u.t1.io_limit, sep);
	fprintf(fp, "%s0x%X%s",  ((prnt_dir == PRNT_COL) ? PCIe_Type1Params[8] : ""), pconf->u.t1.sec_status, sep);
	fprintf(fp, "%s0x%X%s",  ((prnt_dir == PRNT_COL) ? PCIe_Type1Params[9] : ""), pconf->u.t1.mem_base, sep);
	fprintf(fp, "%s0x%X%s",  ((prnt_dir == PRNT_COL) ? PCIe_Type1Params[10] : ""), pconf->u.t1.mem_limit, sep);
	fprintf(fp, "%s0x%X%s",  ((prnt_dir == PRNT_COL) ? PCIe_Type1Params[11] : ""), pconf->u.t1.pf_mem_base, sep);
	fprintf(fp, "%s0x%X%s",  ((prnt_dir == PRNT_COL) ? PCIe_Type1Params[12] : ""), pconf->u.t1.pf_mem_limit, sep);
	fprintf(fp, "%s0x%X%s",  ((prnt_dir == PRNT_COL) ? PCIe_Type1Params[13] : ""), pconf->u.t1.pf_base_u32, sep);
	fprintf(fp, "%s0x%X%s",  ((prnt_dir == PRNT_COL) ? PCIe_Type1Params[14] : ""), pconf->u.t1.pf_limit_u32, sep);
	fprintf(fp, "%s0x%X%s",  ((prnt_dir == PRNT_COL) ? PCIe_Type1Params[15] : ""), pconf->u.t1.io_base_u16, sep);
	fprintf(fp, "%s0x%X%s",  ((prnt_dir == PRNT_COL) ? PCIe_Type1Params[16] : ""), pconf->u.t1.io_limit_u16, sep);
	fprintf(fp, "%s0x%X%s",  ((prnt_dir == PRNT_COL) ? PCIe_Type1Params[17] : ""), pconf->u.t1.exp_rom_ba, sep);
	fprintf(fp, "%s0x%X%s",  ((prnt_dir == PRNT_COL) ? PCIe_Type1Params[18] : ""), pconf->u.t1.bridge_ctrl, sep);
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


void print_config_type1_param_title(FILE *fp) {
	// print common parameter title
	for (int i = 0; i < PCIE_COMN_PAR_SIZE; i++) {
		fprintf(fp, "%s|", PCIe_CommonParams[i]);
	}

	// print type 0 parameter title
	for (int i = 0; i < PCIE_TYP1_PAR_SIZE; i++) {
		fprintf(fp, "%s|", PCIe_Type1Params[i]);
	}

	fprintf(fp, "\n");
}


void print_string(FILE *fp, const char *str) {
	if (fp) {
		fprintf(fp, "%s", str);
	}
}
