#include "sysinfo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <sys/utsname.h>

static void print_usage(const char *prog_name) {
    printf("Usage: %s [OPTIONS]\n\n", prog_name);
    printf("A system information utility for the AiShell platform.\n\n");
    printf("Options:\n");
    printf("  -h, --help        Display this help message and exit 0\n");
    printf("  -a, --all         Print all system fields (kernel, hostname, release, arch)\n");
    printf("  -s, --kernel-name Print the kernel name\n");
    printf("  -r, --release     Print the kernel release version\n");
    printf("  -m, --machine     Print the machine hardware architecture\n");
    printf("  -n, --nodename    Print the network node hostname\n");
    printf("  -v, --version     Display version information and exit 0\n");
}

int run_sysinfo(int argc, char *argv[]) {
    int show_kernel = 0;
    int show_release = 0;
    int show_machine = 0;
    int show_node = 0;

    static struct option long_options[] = {
        {"help",        no_argument, 0, 'h'},
        {"all",         no_argument, 0, 'a'},
        {"kernel-name", no_argument, 0, 's'},
        {"release",     no_argument, 0, 'r'},
        {"machine",     no_argument, 0, 'm'},
        {"nodename",    no_argument, 0, 'n'},
        {"version",     no_argument, 0, 'v'},
        {0, 0, 0, 0}
    };

    // Reset getopt_long global state for modular re-entrancy inside a shell
    optind = 1;
    int opt;
    int option_index = 0;

    while ((opt = getopt_long(argc, argv, "hasrmnv", long_options, &option_index)) != -1) {
        switch (opt) {
            case 'h':
                print_usage(argv[0]);
                return 0;
            case 'a':
                show_kernel = show_release = show_machine = show_node = 1;
                break;
            case 's':
                show_kernel = 1;
                break;
            case 'r':
                show_release = 1;
                break;
            case 'm':
                show_machine = 1;
                break;
            case 'n':
                show_node = 1;
                break;
            case 'v':
                printf("aishell-sysinfo version %s\n", SYSINFO_VERSION);
                return 0;
            case '?':
                // getopt_long already prints an unknown option message to stderr
                fprintf(stderr, "Run '%s --help' for usage details.\n", argv[0]);
                return 1;
            default:
                return 1;
        }
    }

    // Reject unexpected extra positional arguments (e.g., ./aishell extra_arg)
    if (optind < argc) {
        fprintf(stderr, "Error: Unexpected argument '%s'.\n", argv[optind]);
        fprintf(stderr, "Run '%s --help' for usage details.\n", argv[0]);
        return 1;
    }

    // Default behavior: display all metrics if no specific metric flags were given
    if (!show_kernel && !show_release && !show_machine && !show_node) {
        show_kernel = show_release = show_machine = show_node = 1;
    }

    struct utsname info;
    if (uname(&info) < 0) {
        perror("sysinfo (uname system call failed)");
        return 1;
    }

    if (show_kernel)  printf("OS Kernel:   %s\n", info.sysname);
    if (show_node)    printf("Hostname:    %s\n", info.nodename);
    if (show_release) printf("Release:     %s\n", info.release);
    if (show_machine) printf("Arch:        %s\n", info.machine);

    return 0;
}