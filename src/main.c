#include "fetch.h"
#include <stdio.h>

int main() {
    char os[50];
    char kernel[50];
    char hostname[50];
    char shell[50];
    char cpu[50];
    char memory[50];
    
    putchar('\n');

    get_os(os, sizeof(os));
    printf("os\t\t%s\n", os);

    get_kernel(kernel, sizeof(kernel));
    printf("kernel\t\t%s\n", kernel);

    get_hostname(hostname, sizeof(hostname));
    printf("hostname\t%s\n", hostname);

    get_shell(shell, sizeof(shell));
    printf("shell\t\t%s\n", shell);
    
    get_cpu(cpu, sizeof(cpu));
    printf("cpu\t\t%s\n", cpu);

    get_memory(memory, sizeof(memory));
    printf("memory\t\t%s\n", memory);
    
    return 0;
}
