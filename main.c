#include <stdio.h>
#include <string.h>
#include <sys/utsname.h>

void get_os() {
  FILE *file = fopen("/etc/os-release", "r");
    
    if (!file) {
        printf("OS: Unknown\n");
        return;
    }
    char line[256];

    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "PRETTY_NAME=", 12) == 0) {
        char *value = strchr(line, '=');
        if (!value) break;
        value++;
        value[strcspn(value, "\n")] = '\0';
        if (*value == '"') value++;
        size_t len = strlen(value);
        if (len > 0 && value[len - 1] == '"')
            value[len - 1] = '\0';
        printf("OS: %s\n", value);
        break;
        }
    }
    fclose(file);
}

void get_kernel() {
    struct utsname u;
    uname(&u);
    printf("Kernel: %s %s\n", u.sysname, u.release);
}

void get_hostname() {
    FILE *file = fopen("/etc/hostname", "r");

    if (!file) {
        printf("Hostname: Unknown\n");
        return;
    }

    char line[50];
  
    fgets(line, sizeof(line), file);
    line[strcspn(line, "\n")] = '\0';
    printf("Hostname: %s\n", line);
    fclose(file);
}

int main() {
    get_os();
    get_kernel();
    get_hostname();
    return 0;
}
