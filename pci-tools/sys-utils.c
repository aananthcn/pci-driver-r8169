#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void prep_pci_cfg_filename_str(char *file_name, const char *dev_addr) {
	file_name[0] = '\0'; // flush old contents
	strcat(file_name, "/sys/bus/pci/devices/0000:");
	strcat(file_name, dev_addr);
	strcat(file_name, "/config");
}


int read_binary_file(const char *filename, unsigned char *buffer[])
{
	int file_size;

	FILE *file_handle = fopen(filename, "rb");
	if (!file_handle)
	{
		perror("Error opening file");
		return -1;
	}

	// Determine file size
	fseek(file_handle, 0, SEEK_END);
	file_size = ftell(file_handle);
	rewind(file_handle);

	// Allocate memory for character array
	*buffer = (unsigned char *)malloc(file_size);
	if (!*buffer)
	{
		perror("Memory allocation failed");
		fclose(file_handle);
		return -1;
	}

	// Read data from file
	size_t bytes_read = fread(*buffer, 1, file_size, file_handle);
	// Now 'buffer' contains the binary data from the file

	// Clean up
	fclose(file_handle);

	return bytes_read;
}


void dump_buffer_hex(const void *buffer, size_t size)
{
	const unsigned char *bytes = (const unsigned char *)buffer;

	for (size_t i = 0; i < size; i++)
	{
		// Print a newline every 16 bytes with address at start
		if ((i % 16) == 0) {
			printf("\n  %04lX| ", i);
		}
		else if ((i % 4) == 0) {
			printf(" ");
		}

		printf("%02X ", bytes[i]);
	}
	printf("\n\n");
}
