#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sysinfo.h>
#include <sys/utsname.h>
#include <pwd.h>
#include <unistd.h>
#include "fetch.h"

void get_user(char *buf, size_t size) {
    struct passwd *pw = getpwuid(getuid());

    if (!pw) {
        return;
    }

    snprintf(buf, size, "%s", pw->pw_name);
}

void get_hostname(char *buf, size_t size) {
    struct utsname u;
    uname(&u);
    snprintf(buf, size, "%s", u.nodename);
}

void get_os(char *buf, size_t size) {
  FILE *file = fopen("/etc/os-release", "r");
    
    if (!file) {
        snprintf(buf, size, "Unknown");
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
            if (len > 0 && value[len - 1] == '"') {
                value[len - 1] = '\0';
            }

            snprintf(buf, size, "%s", value);
            break;
        }
    }
    fclose(file);
}

void get_kernel(char *buf, size_t size) {
    struct utsname u;
    uname(&u);
    snprintf(buf, size, "%s %s", u.sysname, u.release);
}

void get_shell(char *buf, size_t size) {
    char *str = getenv("SHELL");

    if (!str) {
        return;
    }

    char* ptr = strrchr(str, '/');
    if (!ptr) {
        snprintf(buf, size, "%s", str);
        return;
    }
    
    ptr++;
    snprintf(buf, size, "%s", ptr);
}

void get_cpu(char *buf, size_t size) {
    FILE *file = fopen("/proc/cpuinfo", "r");

    if (!file) {
        snprintf(buf, size, "Unknown");
        return;
    }

    char line[256];
    while(fgets(line, sizeof(line), file)) {
        if (strncmp(line, "model name", 10) == 0) {
            char *value = strchr(line, ':');
            value[strcspn(value, "\n")] = '\0';
            value += 2;
            snprintf(buf, size, "%s", value);
            break;
        }
    }
    fclose(file);
}

void get_memory(char *buf, size_t size) {
    FILE *file = fopen("/proc/meminfo", "r");

    if (!file) {
        snprintf(buf, size, "N/A");
        return;
    }

    char line[256];
    long mem_total = 0, mem_available = 0;
    int found = 0;
    while(fgets(line, sizeof(line), file)) {
        if (strncmp(line, "MemTotal:", 9) == 0) {
            sscanf(line, "MemTotal: %ld kB", &mem_total);
            found++;
        } else if (strncmp(line, "MemAvailable:", 13) == 0) {
            sscanf(line, "MemAvailable: %ld kB", &mem_available);
            found++;
        }
        if (found == 2) break;
    }

    fclose(file);
    
     if (mem_total == 0) {
        printf("Memory: N/A\n");
        snprintf(buf, size, "N/A");
        return;
    }

    double total_gb = mem_total / 1024.0 / 1024.0;
    double avail_gb = mem_available / 1024.0 / 1024.0;
    double used_gb = total_gb - avail_gb;
    int percentage = used_gb / total_gb * 100;

    snprintf(buf, size, "%.2f GB / %.2f GB (%d%%)", used_gb, total_gb, percentage);
}

void get_uptime(char *buf, size_t size) {
    struct sysinfo info;
    if (sysinfo(&info) == 0) {
        long seconds = info.uptime;
        long minutes = seconds / 60;
        long hours = seconds / 3600;

        if (seconds < 60) {
            snprintf(buf, size, "%ld seconds", seconds);
        } else if (seconds >= 60 && seconds < 3600) {
            snprintf(buf, size, "%ld minutes", minutes);
        } else {
            minutes -= 60;
            snprintf(buf, size, "%ld hours %ld minutes", hours, minutes);
        }
    }
}
