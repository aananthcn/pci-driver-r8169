#ifndef SYS_UTILS_H
#define SYS_UTILS_H

#include <stdio.h>

void prep_pci_cfg_filename_str(char *file_name, const char *dev_addr);
int read_binary_file(const char *filename, unsigned char *buffer[]);
void dump_buffer_hex(const void* buffer, size_t size);


#endif