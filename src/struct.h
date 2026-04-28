#ifndef STRUCT_H
#define STRUCT_H

typedef struct {
    char user[64];
    char hostname[128];
    char os[256];
    char kernel[256];
    char shell[64];
    char cpu[256];
    char memory[128];
    char uptime[128];
} Info;

#endif
