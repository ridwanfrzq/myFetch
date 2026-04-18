#include "fetch.h"
#include <stdio.h>

int main() {
    char user[50];
    char os[50];
    char kernel[50];
    char hostname[50];
    char shell[50];
    char cpu[50];
    char memory[50];
    char uptime[50];
    
    putchar('\n');

    get_user(user, sizeof(user));
    get_hostname(hostname, sizeof(hostname));
    printf("%s@%s\n", user, hostname);

    get_os(os, sizeof(os));
    printf("os\t%s\n", os);

    get_kernel(kernel, sizeof(kernel));
    printf("kernel\t%s\n", kernel);

    get_shell(shell, sizeof(shell));
    printf("shell\t%s\n", shell);
    
    get_cpu(cpu, sizeof(cpu));
    printf("cpu\t%s\n", cpu);

    get_memory(memory, sizeof(memory));
    printf("memory\t%s\n", memory);

    get_uptime(uptime, sizeof(uptime));
    printf("uptime\t%s\n", uptime);
    
    return 0;
}
