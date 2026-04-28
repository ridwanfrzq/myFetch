#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sysinfo.h>
#include <sys/utsname.h>
#include <pwd.h>
#include <unistd.h>
#include "fetch.h"
#include "struct.h"

void get_user(Info *ptr) {
    struct passwd *pw = getpwuid(getuid());

    if (!pw) {
        return;
    }

    snprintf(ptr->user, sizeof(ptr->user), "%s", pw->pw_name);
}

void get_hostname(Info *ptr) {
    struct utsname u;
    uname(&u);
    snprintf(ptr->hostname, sizeof(ptr->hostname), "%s", u.nodename);
}

void get_os(Info *ptr) {
  FILE *file = fopen("/etc/os-release", "r");
    
    if (!file) {
        snprintf(ptr->os, sizeof(ptr->os), "Unknown");
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

            snprintf(ptr->os, sizeof(ptr->os), "%s", value);
            break;
        }
    }
    fclose(file);
}

void get_kernel(Info *ptr) {
    struct utsname u;
    uname(&u);
    snprintf(ptr->kernel, sizeof(ptr->kernel), "%s %s", u.sysname, u.release);
}

void get_shell(Info *ptr) {
    char *str = getenv("SHELL");

    if (!str) {
        return;
    }

    char* p = strrchr(str, '/');
    if (!p) {
        snprintf(ptr->shell, sizeof(ptr->shell), "%s", str);
        return;
    }
    
    p++;
    snprintf(ptr->shell, sizeof(ptr->shell), "%s", p);
}

void get_cpu(Info *ptr) {
    FILE *file = fopen("/proc/cpuinfo", "r");

    if (!file) {
        snprintf(ptr->cpu, sizeof(ptr->cpu), "Unknown");
        return;
    }

    char line[256];
    while(fgets(line, sizeof(line), file)) {
        if (strncmp(line, "model name", 10) == 0) {
            char *value = strchr(line, ':');
            value[strcspn(value, "\n")] = '\0';
            value += 2;
            snprintf(ptr->cpu, sizeof(ptr->cpu), "%s", value);
            break;
        }
    }
    fclose(file);
}

void get_memory(Info *ptr) {
    FILE *file = fopen("/proc/meminfo", "r");

    if (!file) {
        snprintf(ptr->memory, sizeof(ptr->memory), "N/A");
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
        snprintf(ptr->memory, sizeof(ptr->memory), "N/A");
        return;
    }

    double total_gb = mem_total / 1024.0 / 1024.0;
    double avail_gb = mem_available / 1024.0 / 1024.0;
    double used_gb = total_gb - avail_gb;
    int percentage = used_gb / total_gb * 100;

    snprintf(ptr->memory, sizeof(ptr->memory), "%.2f GB / %.2f GB (%d%%)", used_gb, total_gb, percentage);
}

void get_uptime(Info *ptr) {
    struct sysinfo info;
    if (sysinfo(&info) == 0) {
        long seconds = info.uptime;
        long minutes = seconds / 60;
        long hours = seconds / 3600;

        if (seconds < 60) {
            snprintf(ptr->uptime, sizeof(ptr->uptime), "%ld seconds", seconds);
        } else if (seconds >= 60 && seconds < 3600) {
            snprintf(ptr->uptime, sizeof(ptr->uptime), "%ld minutes", minutes);
        } else {
            minutes = (seconds % 3600) / 60;
            snprintf(ptr->uptime, sizeof(ptr->uptime), "%ld hours %ld minutes", hours, minutes);
        }
    }
}

void get_info(Info *ptr) {
    get_user(ptr);
    get_hostname(ptr);
    get_os(ptr);
    get_kernel(ptr);
    get_shell(ptr);
    get_cpu(ptr);
    get_memory(ptr);
    get_uptime(ptr);
}
