/* This file contains the implementations of menu commands */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// local file includes
#include "pci-commands.h"
#include "sys-utils.h"
#include "pci-types.h"
#include "pci-print.h"

// macros
#define MAXLINES	(256)


// Function to execute a system command and collect output
static void sys_command(const char *cmd, char *output[], int maxLines)
{
	FILE *pipe = popen(cmd, "r");
	if (!pipe)
	{
		perror("popen() failed");
		exit(EXIT_FAILURE);
	}

	char buffer[256];
	int lineCount = 0;

	while (fgets(buffer, sizeof(buffer), pipe) != NULL && lineCount < maxLines)
	{
		// Remove newline character from the end of each line
		buffer[strcspn(buffer, "\n")] = '\0';

		// Allocate memory for the line and copy the content
		output[lineCount] = strdup(buffer);
		lineCount++;
	}

	pclose(pipe);
}


void cmd_select_device(char dev_addr[]) {
	int device_num, max_dev_num;
	char *output[MAXLINES] = { NULL };
	char command[] = "lspci";

	sys_command(command, output, MAXLINES);
	printf("\n\nPCIe Devices List:\n------------------\n");

	for (int i = 0; i < MAXLINES && output[i] != NULL; i++)
	{
		printf("  [%2d] %s\n", i+1, output[i]);
		max_dev_num = i+1;
	}
	printf("\nType the device number and press ENTER: ");
	scanf("%d", &device_num);
	if ((device_num < 1) || (device_num > max_dev_num)) {
		printf("ERROR: There is no such device with number %d!\n", device_num);
		exit(1);
	}
	strcpy(dev_addr, strtok(output[device_num-1], " "));


	// free the memory created by strdup() here, to avoid memory leak
	for (int i = 0; i < MAXLINES && output[i] != NULL; i++)
	{
		free(output[i]); // Free allocated memory
	}
}


static void get_dev_details(const char dev_addr[], pci_cfg_t *pconf) {
	char *output[MAXLINES] = { NULL };
	char command[32] = "lspci -s ";
	strcat(command, dev_addr);
	sys_command(command, output, MAXLINES);
	char* token = strtok(output[0], " ");
	for (int i = 0; token != NULL; i++) {
		token = strtok(NULL, ":");
		if (i == 0) {
			// get the device type
			strcpy(pconf->cmn.device_type, token);
		}
		else if (i == 1) {
			// remove the leading space
			if (token[0] == ' ')
				token++;
			// get the device details
			strcpy(pconf->cmn.device_str, token);
			break;
		}
		else {
			break;
		}
	}

}


int cmd_get_config_header(const char dev_addr[], pci_cfg_t *pconf) {
	int bytes_read;
	char file_name[128] = "/sys/bus/pci/devices/0000:";
	unsigned char *buffer = NULL;

	// formulate the filename with path to read the config
	strcat(file_name, dev_addr);
	strcat(file_name, "/config");
	printf("\nReading file: %s\n", file_name);

	// read the file contents into the buffer passed by argument
	bytes_read = read_binary_file(file_name, &buffer);
	printf("Bytes read = %d\n", bytes_read);
	dump_buffer_hex(buffer, bytes_read);

	// acquire text information related to this device
	get_dev_details(dev_addr, pconf);

	// read PCI configs - PCIe bytes are always Little Endian
	pconf->cmn.vendor_id  = buffer[0] | (buffer[1] << 8);
	pconf->cmn.device_id  = buffer[2] | (buffer[3] << 8);
	pconf->cmn.cmd_reg    = buffer[4] | (buffer[5] << 8);
	pconf->cmn.stat_reg   = buffer[6] | (buffer[7] << 8);
	pconf->cmn.rev_id     = buffer[8];
	pconf->cmn.class_code = buffer[9] | (buffer[0xa] << 8) | (buffer[0xb] << 16);
	pconf->cmn.cache_line = buffer[0xc];
	pconf->cmn.lat_timer  = buffer[0xd];
	pconf->cmn.hdr_type   = buffer[0xe];
	pconf->cmn.bist       = buffer[0xf];
	pconf->cmn.cap_ptr    = buffer[0x34];
	pconf->cmn.irq_line   = buffer[0x3c];
	pconf->cmn.irq_pin    = buffer[0x3d];

	if (pconf->cmn.hdr_type & 0x7f) {
		// Type 1 Config Space Structure
		pconf->u.t1.bar0         = buffer[0x10] | (buffer[0x11] << 8) | (buffer[0x12] << 16) | (buffer[0x13] << 24);
		pconf->u.t1.bar1         = buffer[0x14] | (buffer[0x15] << 8) | (buffer[0x16] << 16) | (buffer[0x17] << 24);
		pconf->u.t1.pri_bus_n    = buffer[0x18];
		pconf->u.t1.sec_bus_n    = buffer[0x19];
		pconf->u.t1.sbo_bus_n    = buffer[0x1A];
		pconf->u.t1.sec_lat_tm   = buffer[0x1B];
		pconf->u.t1.io_base      = buffer[0x1C];
		pconf->u.t1.io_limit     = buffer[0x1D];
		pconf->u.t1.sec_status   = buffer[0x1E] | (buffer[0x1F] << 8);
		pconf->u.t1.mem_base     = buffer[0x20] | (buffer[0x21] << 8);
		pconf->u.t1.mem_limit    = buffer[0x22] | (buffer[0x23] << 8);
		pconf->u.t1.pf_mem_base  = buffer[0x24] | (buffer[0x25] << 8);
		pconf->u.t1.pf_mem_limit = buffer[0x26] | (buffer[0x27] << 8);
		pconf->u.t1.pf_base_u32  = buffer[0x28] | (buffer[0x29] << 8) | (buffer[0x2A] << 16) | (buffer[0x2B] << 24);
		pconf->u.t1.pf_limit_u32 = buffer[0x2C] | (buffer[0x2D] << 8) | (buffer[0x2E] << 16) | (buffer[0x2F] << 24);
		pconf->u.t1.io_base_u16  = buffer[0x30] | (buffer[0x31] << 8);
		pconf->u.t1.io_limit_u16 = buffer[0x32] | (buffer[0x33] << 8);
		pconf->u.t1.exp_rom_ba   = buffer[0x38] | (buffer[0x39] << 8) | (buffer[0x3A] << 16) | (buffer[0x3B] << 24);
		pconf->u.t1.bridge_ctrl  = buffer[0x3E] | (buffer[0x3F] << 8);
	}
	else {
		// Type 0 Config Space Structure
		pconf->u.t0.bar0       = buffer[0x10] | (buffer[0x11] << 8) | (buffer[0x12] << 16) | (buffer[0x13] << 24);
		pconf->u.t0.bar1       = buffer[0x14] | (buffer[0x15] << 8) | (buffer[0x16] << 16) | (buffer[0x17] << 24);
		pconf->u.t0.bar2       = buffer[0x18] | (buffer[0x19] << 8) | (buffer[0x1a] << 16) | (buffer[0x1b] << 24);
		pconf->u.t0.bar3       = buffer[0x1c] | (buffer[0x1d] << 8) | (buffer[0x1e] << 16) | (buffer[0x1f] << 24);
		pconf->u.t0.bar4       = buffer[0x20] | (buffer[0x21] << 8) | (buffer[0x22] << 16) | (buffer[0x23] << 24);
		pconf->u.t0.bar5       = buffer[0x24] | (buffer[0x25] << 8) | (buffer[0x26] << 16) | (buffer[0x27] << 24);
		pconf->u.t0.card_bus_p = buffer[0x28] | (buffer[0x29] << 8) | (buffer[0x2a] << 16) | (buffer[0x2b] << 24);
		pconf->u.t0.subsys_vid = buffer[0x2c] | (buffer[0x2d] << 8);
		pconf->u.t0.subsys_did = buffer[0x2e] | (buffer[0x2f] << 8);
		pconf->u.t0.exp_rom_ba = buffer[0x30] | (buffer[0x31] << 8) | (buffer[0x32] << 16) | (buffer[0x33] << 24);
	}

	// free the buffer allocated by read_binary_file function
	if (buffer)
		free(buffer);
	
	return 0;
}


void cmd_print_configs(FILE *fp, pci_cfg_t *pconf, prnt_t prnt_dir) {
	char sep[16];

	// prepare the separator string for each parameter
	if (prnt_dir == PRNT_COL ) {
		strcpy(sep, "\n");
	}
	else {
		strcpy(sep, "|");
	}

	// print configs
	print_comn_cfg_params(fp, pconf, prnt_dir, sep);
	if (pconf->cmn.hdr_type & 0x7F) {
		// type 1
		print_type1_cfg_params(fp, pconf, prnt_dir, sep);
	}
	else {
		// type 0
		print_type0_cfg_params(fp, pconf, prnt_dir, sep);
	}

	if (strcmp(sep, "\n"))
		fprintf(fp, "\n");
}


void cmd_get_all_configs_to_file(void) {
	char dev_addr[32];
	pci_cfg_t pci_config;
	char *output[MAXLINES] = { NULL };
	char command[] = "lspci";

	// open file for writing the output
	FILE *fp = fopen("pci-configs.csv", "w");
	if (fp == NULL) {
		printf("Error: Can't open pci-configs.csv\n");
		exit(1);
	}

	// read pci data of all device and write them to output file
	sys_command(command, output, MAXLINES);

	// TYPE 0 Configs print
	print_string(fp, "Type 0 Configs|||||||||||||||||||||||||||||||\n"); // these additional '|' are needed to satisfy Microsoft's CSV file reading bug.
	print_config_type0_param_title(fp);
	for (int i = 0; i < MAXLINES && output[i] != NULL; i++) {
		strcpy(dev_addr, strtok(output[i], " "));
		cmd_get_config_header(dev_addr, &pci_config);
		if ((pci_config.cmn.hdr_type & 0x7F) == 0) {
			// print them horizontally with comma as separator
			cmd_print_configs(fp, &pci_config, PRNT_ROW);
		}
	}

	// TYPE 1 Configs print
	print_string(fp, "\n\nType 1 Configs\n");
	print_config_type1_param_title(fp);
	for (int i = 0; i < MAXLINES && output[i] != NULL; i++) {
		strcpy(dev_addr, strtok(output[i], " "));
		cmd_get_config_header(dev_addr, &pci_config);
		if ((pci_config.cmn.hdr_type & 0x7F) == 1) {
			// print them horizontally with comma as separator
			cmd_print_configs(fp, &pci_config, PRNT_ROW);
		}
	}

	if(fp)
		fclose(fp);
}



void cmd_print_power_mgmt_caps(const char dev_addr[], FILE *fp, pci_cfg_t *pconf, prnt_t prnt_dir) {
	char sep[16];
	unsigned char pmc_offset;
	unsigned char cap_offset;

	int bytes_read;
	char file_name[128] = "/sys/bus/pci/devices/0000:";
	unsigned char *buffer = NULL;

	// formulate the filename with path to read the config
	strcat(file_name, dev_addr);
	strcat(file_name, "/config");
	printf("\nReading file: %s\n", file_name);

	// read the file contents into the buffer passed by argument
	bytes_read = read_binary_file(file_name, &buffer);
	printf("Bytes read = %d\n", bytes_read);
	dump_buffer_hex(buffer, bytes_read);

	// prepare the separator string for each parameter
	if (prnt_dir == PRNT_COL ) {
		strcpy(sep, "\n");
	}
	else {
		strcpy(sep, "|");
	}

	// get the PMC offset
	cap_offset = pconf->cmn.cap_ptr & 0xFC; //mask the last 2 bits
	for (int i = 0; i < (256 / 8); i++) { // PMC should be placed within the first 256 bytes
		if (buffer[cap_offset] == PWR_MGMT_CAPABILITY_ID) {
			// we found the capability structure
			pmc_offset = cap_offset;
			fprintf(fp, "Found Pwr. Mgmt. Capability at offset = 0x%02X\n", pmc_offset);
			break;
		}
		else if (buffer[cap_offset+1]) {
			cap_offset = buffer[cap_offset+1];
			continue;
		}
		else {
			fprintf(fp, "ERROR: No Power Management Capabilty found for PCIe device %s\n", dev_addr);
			goto pmc_exit;
		}
	}

	pci_pmr_mgmt_cap_t pmc_cap;

	pmc_cap.pmc   = buffer[pmc_offset+2] | (buffer[pmc_offset+3] << 8);
	pmc_cap.pmcsr = buffer[pmc_offset+4] | (buffer[pmc_offset+5] << 8);
	pmc_cap.data  = buffer[pmc_offset+7];

	print_string(stdout, "\n\n");
	print_pwr_mgmt_cap_params(stdout, &pmc_cap, PRNT_COL, "\n");

pmc_exit:
	// free the buffer allocated by read_binary_file function
	if (buffer)
		free(buffer);
}