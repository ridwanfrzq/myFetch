#include "fetch.h"
#include <stdio.h>

int main() {
    putchar('\n');

    Info p = {0};
    Info *ptr = &p;

    get_info(ptr);

    printf("\033[0;34m%s@%s\033[0m\n", ptr->user, ptr->hostname);

    printf("\033[0;33mos\033[0m\t%s\n", ptr->os);

    printf("\033[0;33mkernel\033[0m\t%s\n", ptr->kernel);

    printf("\033[0;33mshell\033[0m\t%s\n", ptr->shell);
    
    printf("\033[0;33mcpu\033[0m\t%s\n", ptr->cpu);

    printf("\033[0;33mmemory\033[0m\t%s\n", ptr->memory);

    printf("\033[0;33muptime\033[0m\t%s\n", ptr->uptime);

    return 0;
}
