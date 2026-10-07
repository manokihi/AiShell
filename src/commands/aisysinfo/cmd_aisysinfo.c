#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/utsname.h>
#include <sys/sysinfo.h>
#include <third-party/argtable3/argtable3.h>

#include "cmd_spec.h"
#include "commands/aisysinfo/cmd_aisysinfo.h"

static void build_aisysinfo_argtable(ai_global_args_t *globals,
                                      struct arg_lit **all,
                                      struct arg_lit **sys,
                                      struct arg_lit **kernel,
                                      struct arg_lit **uptime,
                                      struct arg_lit **mem,
                                      struct arg_end **end,
                                      void ***argtable_out)
{
    ai_init_global_args(globals);

    *all    = arg_lit0("a", "all",     "display all system info metrics");
    *sys    = arg_lit0("s", "sysname", "display operating system name");
    *kernel = arg_lit0("k", "kernel",  "display kernel version");
    *uptime = arg_lit0("u", "uptime",  "display system uptime");
    *mem    = arg_lit0("m", "memory",  "display memory usage stats");
    *end    = arg_end(20);

    static void *argtable[10];
    argtable[0] = globals->help;
    argtable[1] = globals->json;
    argtable[2] = globals->version;
    argtable[3] = *all;
    argtable[4] = *sys;
    argtable[5] = *kernel;
    argtable[6] = *uptime;
    argtable[7] = *mem;
    argtable[8] = *end;
    argtable[9] = NULL;

    *argtable_out = argtable;
}

void aisysinfo_print_usage(FILE *out) {
    ai_global_args_t globals;
    struct arg_lit *all, *sys, *kernel, *uptime, *mem;
    struct arg_end *end;
    void **argtable;

    build_aisysinfo_argtable(&globals, &all, &sys, &kernel, &uptime, &mem, &end, &argtable);

    fprintf(out, "Usage: aisysinfo");
    arg_print_syntax(out, argtable, "\n");
    fprintf(out, "\nOptions:\n");
    arg_print_glossary(out, argtable, "  %-20s %s\n");

    arg_freetable(argtable, 9);
}

int aisysinfo_run(int argc, char **argv) {
    ai_global_args_t globals;
    struct arg_lit *all, *sys, *kernel, *uptime, *mem;
    struct arg_end *end;
    void **argtable;

    build_aisysinfo_argtable(&globals, &all, &sys, &kernel, &uptime, &mem, &end, &argtable);

    int nerrors = arg_parse(argc, argv, argtable);

    if (ai_handle_global_flags(&globals, &cmd_aisysinfo_spec, stdout)) {
        arg_freetable(argtable, 9);
        return 0;
    }

    if (nerrors > 0) {
        arg_print_errors(stderr, end, "aisysinfo");
        aisysinfo_print_usage(stderr);
        arg_freetable(argtable, 9);
        return 1;
    }

    int show_all = (all->count > 0) || (sys->count == 0 && kernel->count == 0 &&
                                        uptime->count == 0 && mem->count == 0);

    struct utsname uts;
    struct sysinfo info;

    if (uname(&uts) < 0) {
        perror("aisysinfo: uname failed");
        arg_freetable(argtable, 9);
        return 1;
    }

    if (sysinfo(&info) < 0) {
        perror("aisysinfo: sysinfo failed");
        arg_freetable(argtable, 9);
        return 1;
    }

    int show_sys    = show_all || (sys->count > 0);
    int show_kernel = show_all || (kernel->count > 0);
    int show_uptime = show_all || (uptime->count > 0);
    int show_mem    = show_all || (mem->count > 0);

    if (globals.json->count > 0) {
        printf("{");
        int printed = 0;
        if (show_sys) {
            printf("\"sysname\":\"%s\"", uts.sysname);
            printed = 1;
        }
        if (show_kernel) {
            printf("%s\"kernel\":\"%s\"", printed ? "," : "", uts.release);
            printed = 1;
        }
        if (show_uptime) {
            printf("%s\"uptime_sec\":%ld", printed ? "," : "", info.uptime);
            printed = 1;
        }
        if (show_mem) {
            printf("%s\"total_ram\":%lu,\"free_ram\":%lu", 
                   printed ? "," : "", info.totalram * info.mem_unit, info.freeram * info.mem_unit);
        }
        printf("}\n");
    } else {
        if (show_sys)    printf("OS Sysname : %s\n", uts.sysname);
        if (show_kernel) printf("Kernel Release: %s\n", uts.release);
        if (show_uptime) printf("Uptime     : %ld seconds\n", info.uptime);
        if (show_mem) {
            printf("Total RAM  : %lu MB\n", (info.totalram * info.mem_unit) / (1024 * 1024));
            printf("Free RAM   : %lu MB\n", (info.freeram * info.mem_unit) / (1024 * 1024));
        }
    }

    arg_freetable(argtable, 9);
    return 0;
}

cmd_spec_t cmd_aisysinfo_spec = {
    .name        = "aisysinfo",
    .summary     = "display system and kernel information",
    .long_help   = "Displays POSIX system information including OS name, kernel version, uptime, and memory statistics.",
    .run         = aisysinfo_run,
    .print_usage = aisysinfo_print_usage,
};

void register_aisysinfo_command(void) {
    register_command(&cmd_aisysinfo_spec);
}