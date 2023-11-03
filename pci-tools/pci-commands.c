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
static void sys_coommand(const char *cmd, char *output[], int maxLines)
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

	sys_coommand(command, output, MAXLINES);
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


void cmd_print_configs(const char dev_addr[]) {
	int bytes_read;
	char file_name[128] = "/sys/bus/pci/devices/0000:";
	unsigned char *buffer = NULL;
	pci_config_t pconf;

	// formulate the filename with path to read the config
	strcat(file_name, dev_addr);
	strcat(file_name, "/config");
	printf("\nReading file: %s\n", file_name);

	// read the file contents into the buffer passed by argument
	bytes_read = read_binary_file(file_name, &buffer);
	printf("Bytes read = %d\n", bytes_read);
	dump_buffer_hex(buffer, bytes_read);

	// read PCI configs - PCIe bytes are always Little Endian
	pconf.vendor_id  = buffer[0] | (buffer[1] << 8);
	pconf.device_id  = buffer[2] | (buffer[3] << 8);
	pconf.cmd_reg    = buffer[4] | (buffer[5] << 8);
	pconf.stat_reg   = buffer[6] | (buffer[7] << 8);
	pconf.rev_id     = buffer[8];
	pconf.class_code = buffer[9] | (buffer[0xa] << 8) | (buffer[0xb] << 16);
	pconf.cache_line = buffer[0xc];
	pconf.lat_timer  = buffer[0xd];
	pconf.hdr_type   = buffer[0xe];
	pconf.bist       = buffer[0xf];
	pconf.bar0       = buffer[0x10] | (buffer[0x11] << 8) | (buffer[0x12] << 16) | (buffer[0x13] << 24);
	pconf.bar1       = buffer[0x14] | (buffer[0x15] << 8) | (buffer[0x16] << 16) | (buffer[0x17] << 24);
	pconf.bar2       = buffer[0x18] | (buffer[0x19] << 8) | (buffer[0x1a] << 16) | (buffer[0x1b] << 24);
	pconf.bar3       = buffer[0x1c] | (buffer[0x1d] << 8) | (buffer[0x1e] << 16) | (buffer[0x1f] << 24);
	pconf.bar4       = buffer[0x20] | (buffer[0x21] << 8) | (buffer[0x22] << 16) | (buffer[0x23] << 24);
	pconf.bar5       = buffer[0x24] | (buffer[0x25] << 8) | (buffer[0x26] << 16) | (buffer[0x27] << 24);
	pconf.card_bus_p = buffer[0x28] | (buffer[0x29] << 8) | (buffer[0x2a] << 16) | (buffer[0x2b] << 24);
	pconf.subsys_vid = buffer[0x2c] | (buffer[0x2d] << 8);
	pconf.subsys_did = buffer[0x2e] | (buffer[0x2f] << 8);
	pconf.exp_rom_ba = buffer[0x30] | (buffer[0x31] << 8) | (buffer[0x32] << 16) | (buffer[0x33] << 24);
	pconf.irq_line   = buffer[0x3c];
	pconf.irq_pin    = buffer[0x3d];

	// free the buffer allocated by read_binary_file function
	if (buffer)
		free(buffer);

	// print configs
	printf("\n");
	printf("Vendor ID       : 0x%X\n",  pconf.vendor_id);
	printf("Device ID       : 0x%X\n",  pconf.device_id);
	printf("Command Reg     : 0x%X\n",  pconf.cmd_reg);
	printf("Status Reg      : 0x%X\n",  pconf.stat_reg);
	printf("Revision ID     : %d\n",    pconf.rev_id);
	printf("Class Code      : 0x%X\n",  pconf.class_code);
	printf("Cache Line      : 0x%X\n",  pconf.cache_line);
	printf("Latency Timer   : 0x%X\n",  pconf.lat_timer);
	printf("Header Type     : 0x%X\n",  pconf.hdr_type);
	printf("BIST            : 0x%X\n",  pconf.bist);
	printf("BAR0            : 0x%X\n",  pconf.bar0);
	printf("BAR1            : 0x%X\n",  pconf.bar1);
	printf("BAR2            : 0x%X\n",  pconf.bar2);
	printf("BAR3            : 0x%X\n",  pconf.bar3);
	printf("BAR4            : 0x%X\n",  pconf.bar4);
	printf("BAR5            : 0x%X\n",  pconf.bar5);
	printf("Cardbus CIS Ptr : 0x%X\n",  pconf.card_bus_p);
	printf("Subs. Vendor ID : 0x%X\n",  pconf.subsys_vid);
	printf("Subs. Device ID : 0x%X\n",  pconf.subsys_did);
	printf("IRQ Line        : 0x%X\n",  pconf.irq_line);
	printf("IRQ Pin         : 0x%X\n",  pconf.irq_pin);
}
