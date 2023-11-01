#ifndef SYS_UTILS_H
#define SYS_UTILS_H

#include <stdio.h>

int read_binary_file(const char *filename, unsigned char *buffer[]);
void dump_buffer_hex(const void* buffer, size_t size);


#endif