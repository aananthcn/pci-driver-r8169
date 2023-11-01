/* This file contains the implementations of menu commands */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// local file includes
#include "pci-commands.h"
#include "sys-utils.h"

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

	// formulate the filename with path to read the config
	strcat(file_name, dev_addr);
	strcat(file_name, "/config");
	printf("\nReading file: %s\n", file_name);

	// read the file contents into the buffer passed by argument
	bytes_read = read_binary_file(file_name, &buffer);
	printf("Bytes read = %d\n", bytes_read);
	dump_buffer_hex(buffer, bytes_read);

	// read PCI configs - PCIe bytes are always Little Endian
	unsigned short vendor_id  = buffer[0] | (buffer[1] << 8);
	unsigned short device_id  = buffer[2] | (buffer[3] << 8);
	unsigned short cmd_reg    = buffer[4] | (buffer[5] << 8);
	unsigned short stat_reg   = buffer[6] | (buffer[7] << 8);
	unsigned char  rev_id     = buffer[8];
	unsigned int   class_code = buffer[9] | (buffer[0xa] << 8) | (buffer[0xb] << 16);
	unsigned char  cache_line = buffer[0xc];
	unsigned char  lat_timer  = buffer[0xd];
	unsigned char  hdr_type   = buffer[0xe];
	unsigned char  bist       = buffer[0xf];
	unsigned int   bar0       = buffer[0x10] | (buffer[0x11] << 8) | (buffer[0x12] << 16) | (buffer[0x13] << 24);
	unsigned int   bar1       = buffer[0x14] | (buffer[0x15] << 8) | (buffer[0x16] << 16) | (buffer[0x17] << 24);
	unsigned int   bar2       = buffer[0x18] | (buffer[0x19] << 8) | (buffer[0x1a] << 16) | (buffer[0x1b] << 24);
	unsigned int   bar3       = buffer[0x1c] | (buffer[0x1d] << 8) | (buffer[0x1e] << 16) | (buffer[0x1f] << 24);
	unsigned int   bar4       = buffer[0x20] | (buffer[0x21] << 8) | (buffer[0x22] << 16) | (buffer[0x23] << 24);
	unsigned int   bar5       = buffer[0x24] | (buffer[0x25] << 8) | (buffer[0x26] << 16) | (buffer[0x27] << 24);
	unsigned int   card_bus_p = buffer[0x28] | (buffer[0x29] << 8) | (buffer[0x2a] << 16) | (buffer[0x2b] << 24);
	unsigned short subsys_vid = buffer[0x2c] | (buffer[0x2d] << 8);
	unsigned short subsys_did = buffer[0x2e] | (buffer[0x2f] << 8);
	unsigned int   exp_rom_ba = buffer[0x30] | (buffer[0x31] << 8) | (buffer[0x32] << 16) | (buffer[0x33] << 24);
	unsigned char  irq_line   = buffer[0x3c];
	unsigned char  irq_pin    = buffer[0x3d];

	// free the buffer allocated by read_binary_file function
	if (buffer)
		free(buffer);

	// print configs
	printf("\n");
	printf("Vendor ID       : 0x%X\n", vendor_id);
	printf("Device ID       : 0x%X\n", device_id);
	printf("Command Reg     : 0x%X\n", cmd_reg);
	printf("Status Reg      : 0x%X\n", stat_reg);
	printf("Revision ID     : %d\n", rev_id);
	printf("Class Code      : 0x%X\n", class_code);
	printf("Cache Line      : 0x%X\n", cache_line);
	printf("Latency Timer   : 0x%X\n", lat_timer);
	printf("Header Type     : 0x%X\n", hdr_type);
	printf("BIST            : 0x%X\n", bist);
	printf("BAR0            : 0x%X\n", bar0);
	printf("BAR1            : 0x%X\n", bar1);
	printf("BAR2            : 0x%X\n", bar2);
	printf("BAR3            : 0x%X\n", bar3);
	printf("BAR4            : 0x%X\n", bar4);
	printf("BAR5            : 0x%X\n", bar5);
	printf("Cardbus CIS Ptr : 0x%X\n", card_bus_p);
	printf("Subs. Vendor ID : 0x%X\n", subsys_vid);
	printf("Subs. Device ID : 0x%X\n", subsys_did);
	printf("IRQ Line        : 0x%X\n", irq_line);
	printf("IRQ Pin         : 0x%X\n", irq_pin);
}
