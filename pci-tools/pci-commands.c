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

	// read PCI configs
	unsigned short vendor_id = buffer[0] | (buffer[1] << 8);
	unsigned short device_id = buffer[2] | (buffer[3] << 8);
	unsigned short cmd_reg   = buffer[4] | (buffer[5] << 8);
	unsigned short stat_reg  = buffer[6] | (buffer[7] << 8);
	unsigned char  rev_id    = buffer[8];

	// free the buffer allocated by read_binary_file function
	if (buffer)
		free(buffer);

	// print configs
	printf("\n");
	printf("Vendor ID   : 0x%X\n", vendor_id);
	printf("Device ID   : 0x%X\n", device_id);
	printf("Command Reg : 0x%X\n", cmd_reg);
	printf("Status Reg  : 0x%X\n", stat_reg);
	printf("Revision ID : %d\n", rev_id);
}
