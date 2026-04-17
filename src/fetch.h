#ifndef FETCH_H
#define FETCH_H

#include <stddef.h>

void get_os(char *buf, size_t size);
void get_kernel(char *buf, size_t size);
void get_hostname(char *buf, size_t size);
void get_shell(char *buf, size_t size);
void get_cpu(char *buf, size_t size);
void get_memory(char *buf, size_t size);

#endif
