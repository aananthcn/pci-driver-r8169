/* This file contains the implementations of menu commands */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// local file includes
#include "pci-commands.h"
#include "sys-utils.h"
#include "pci-types.h"

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


void get_dev_details(const char dev_addr[], pci_config_t *pconf) {
	char *output[MAXLINES] = { NULL };
	char command[32] = "lspci -s ";
	strcat(command, dev_addr);
	sys_command(command, output, MAXLINES);
	char* token = strtok(output[0], " ");
	for (int i = 0; token != NULL; i++) {
		token = strtok(NULL, ":");
		if (i == 0) {
			// get the device type
			strcpy(pconf->device_type, token);
		}
		else if (i == 1) {
			// remove the leading space
			if (token[0] == ' ')
				token++;
			// get the device details
			strcpy(pconf->device_str, token);
			break;
		}
		else {
			break;
		}
	}

}


int cmd_get_config_header(const char dev_addr[], pci_config_t *pconf) {
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
	pconf->vendor_id  = buffer[0] | (buffer[1] << 8);
	pconf->device_id  = buffer[2] | (buffer[3] << 8);
	pconf->cmd_reg    = buffer[4] | (buffer[5] << 8);
	pconf->stat_reg   = buffer[6] | (buffer[7] << 8);
	pconf->rev_id     = buffer[8];
	pconf->class_code = buffer[9] | (buffer[0xa] << 8) | (buffer[0xb] << 16);
	pconf->cache_line = buffer[0xc];
	pconf->lat_timer  = buffer[0xd];
	pconf->hdr_type   = buffer[0xe];
	pconf->bist       = buffer[0xf];
	pconf->bar0       = buffer[0x10] | (buffer[0x11] << 8) | (buffer[0x12] << 16) | (buffer[0x13] << 24);
	pconf->bar1       = buffer[0x14] | (buffer[0x15] << 8) | (buffer[0x16] << 16) | (buffer[0x17] << 24);
	pconf->bar2       = buffer[0x18] | (buffer[0x19] << 8) | (buffer[0x1a] << 16) | (buffer[0x1b] << 24);
	pconf->bar3       = buffer[0x1c] | (buffer[0x1d] << 8) | (buffer[0x1e] << 16) | (buffer[0x1f] << 24);
	pconf->bar4       = buffer[0x20] | (buffer[0x21] << 8) | (buffer[0x22] << 16) | (buffer[0x23] << 24);
	pconf->bar5       = buffer[0x24] | (buffer[0x25] << 8) | (buffer[0x26] << 16) | (buffer[0x27] << 24);
	pconf->card_bus_p = buffer[0x28] | (buffer[0x29] << 8) | (buffer[0x2a] << 16) | (buffer[0x2b] << 24);
	pconf->subsys_vid = buffer[0x2c] | (buffer[0x2d] << 8);
	pconf->subsys_did = buffer[0x2e] | (buffer[0x2f] << 8);
	pconf->exp_rom_ba = buffer[0x30] | (buffer[0x31] << 8) | (buffer[0x32] << 16) | (buffer[0x33] << 24);
	pconf->irq_line   = buffer[0x3c];
	pconf->irq_pin    = buffer[0x3d];

	// free the buffer allocated by read_binary_file function
	if (buffer)
		free(buffer);
	
	return 0;
}


const char *PCIe_Params[] = {
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
	"BAR0            : ", /* 12 */
	"BAR1            : ", /* 13 */
	"BAR2            : ", /* 14 */
	"BAR3            : ", /* 15 */
	"BAR4            : ", /* 16 */
	"BAR5            : ", /* 17 */
	"Cardbus CIS Ptr : ", /* 18 */
	"Subs. Vendor ID : ", /* 19 */
	"Subs. Device ID : ", /* 20 */
	"IRQ Line        : ", /* 21 */
	"IRQ Pin         : "  /* 22 */
};

void cmd_print_configs(FILE *fp, pci_config_t *pconf, prnt_t prnt_dir) {
	char sep[16];

	// prepare the separator string for each parameter
	if (prnt_dir == PRNT_COL ) {
		strcpy(sep, "\n");
	}
	else {
		strcpy(sep, "|");
	}

	// print configs
	fprintf(fp, "%s0x%X%s",  ((prnt_dir == PRNT_COL) ? PCIe_Params[0] : ""), pconf->vendor_id, sep);
	fprintf(fp, "%s0x%X%s",  ((prnt_dir == PRNT_COL) ? PCIe_Params[1] : ""), pconf->device_id, sep);
	fprintf(fp, "%s%s%s",    ((prnt_dir == PRNT_COL) ? PCIe_Params[2] : ""), pconf->device_type, sep);
	fprintf(fp, "%s%s%s",    ((prnt_dir == PRNT_COL) ? PCIe_Params[3] : ""), pconf->device_str, sep);
	fprintf(fp, "%s0x%X%s",  ((prnt_dir == PRNT_COL) ? PCIe_Params[4] : ""), pconf->cmd_reg, sep);
	fprintf(fp, "%s0x%X%s",  ((prnt_dir == PRNT_COL) ? PCIe_Params[5] : ""), pconf->stat_reg, sep);
	fprintf(fp, "%s%d%s",    ((prnt_dir == PRNT_COL) ? PCIe_Params[6] : ""), pconf->rev_id, sep);
	fprintf(fp, "%s0x%X%s",  ((prnt_dir == PRNT_COL) ? PCIe_Params[7] : ""), pconf->class_code, sep);
	fprintf(fp, "%s0x%X%s",  ((prnt_dir == PRNT_COL) ? PCIe_Params[8] : ""), pconf->cache_line, sep);
	fprintf(fp, "%s0x%X%s",  ((prnt_dir == PRNT_COL) ? PCIe_Params[9] : ""), pconf->lat_timer, sep);
	fprintf(fp, "%s0x%X%s",  ((prnt_dir == PRNT_COL) ? PCIe_Params[10] : ""), pconf->hdr_type, sep);
	fprintf(fp, "%s0x%X%s",  ((prnt_dir == PRNT_COL) ? PCIe_Params[11] : ""), pconf->bist, sep);
	fprintf(fp, "%s0x%X%s",  ((prnt_dir == PRNT_COL) ? PCIe_Params[12] : ""), pconf->bar0, sep);
	fprintf(fp, "%s0x%X%s",  ((prnt_dir == PRNT_COL) ? PCIe_Params[13] : ""), pconf->bar1, sep);
	fprintf(fp, "%s0x%X%s",  ((prnt_dir == PRNT_COL) ? PCIe_Params[14] : ""), pconf->bar2, sep);
	fprintf(fp, "%s0x%X%s",  ((prnt_dir == PRNT_COL) ? PCIe_Params[15] : ""), pconf->bar3, sep);
	fprintf(fp, "%s0x%X%s",  ((prnt_dir == PRNT_COL) ? PCIe_Params[16] : ""), pconf->bar4, sep);
	fprintf(fp, "%s0x%X%s",  ((prnt_dir == PRNT_COL) ? PCIe_Params[17] : ""), pconf->bar5, sep);
	fprintf(fp, "%s0x%X%s",  ((prnt_dir == PRNT_COL) ? PCIe_Params[18] : ""), pconf->card_bus_p, sep);
	fprintf(fp, "%s0x%X%s",  ((prnt_dir == PRNT_COL) ? PCIe_Params[19] : ""), pconf->subsys_vid, sep);
	fprintf(fp, "%s0x%X%s",  ((prnt_dir == PRNT_COL) ? PCIe_Params[20] : ""), pconf->subsys_did, sep);
	fprintf(fp, "%s0x%X%s",  ((prnt_dir == PRNT_COL) ? PCIe_Params[21] : ""), pconf->irq_line, sep);
	fprintf(fp, "%s0x%X%s",  ((prnt_dir == PRNT_COL) ? PCIe_Params[22] : ""), pconf->irq_pin, sep);

	if (strcmp(sep, "\n"))
		fprintf(fp, "\n");
}

static void print_config_param_title(FILE *fp) {
	int i = 0;

	for (; i < 22; i++) {
		fprintf(fp, "%s|", PCIe_Params[i]);
	}
	fprintf(fp, "%s\n", PCIe_Params[i]);
}




void cmd_get_all_configs_to_file(void) {
	char dev_addr[32];
	pci_config_t pci_config;
	char *output[MAXLINES] = { NULL };
	char command[] = "lspci";

	// open file for writing the output
	FILE *fp = fopen("pci-configs.csv", "w");
	if (fp == NULL) {
		printf("Error: Can't open pci-configs.csv\n");
		exit(1);
	}

	// print title-text on csv file for better column readability
	print_config_param_title(fp);

	// read pci data of all device and write them to output file
	sys_command(command, output, MAXLINES);
	for (int i = 0; i < MAXLINES && output[i] != NULL; i++) {
		strcpy(dev_addr, strtok(output[i], " "));
		cmd_get_config_header(dev_addr, &pci_config);
		// print them horizontally with comma as separator
		cmd_print_configs(fp, &pci_config, PRNT_ROW);
	}

	if(fp)
		fclose(fp);
}
