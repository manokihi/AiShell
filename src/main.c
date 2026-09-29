#include <stdio.h>
#include <string.h>
#include "tools/sysinfo.h"

int main(int argc, char *argv[]) {
    // If explicitly invoked as a sub-command ("./aishell sysinfo ...")
    if (argc > 1 && strcmp(argv[1], "sysinfo") == 0) {
        return run_sysinfo(argc - 1, &argv[1]);
    }

    // Default driver runner for Week 1 baseline
    return run_sysinfo(argc, argv);
}