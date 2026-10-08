#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cmd_spec.h"
#include "commands/aisysinfo/cmd_aisysinfo.h"
#include "commands/aipwd/cmd_aipwd.h"
#include "commands/aicd/cmd_aicd.h"
#include "commands/ails/cmd_ails.h"

static void register_all_builtin_commands(void) {
    register_aisysinfo_command();
    register_aipwd_command();
    register_aicd_command();
    register_ails_command();
}

int main(int argc, char **argv) {
    // Initialize registry
    register_all_builtin_commands();

    // If arguments are passed directly to aishell (e.g. ./bin/aishell aisysinfo --json)
    if (argc > 1) {
        const cmd_spec_t *spec = find_command(argv[1]);
        if (!spec) {
            fprintf(stderr, "aishell: unknown command '%s'\n", argv[1]);
            return 1;
        }
        return spec->run(argc - 1, &argv[1]);
    }

    // Otherwise, start interactive REPL loop
    char line[1024];
    printf("AiShell REPL (v%s) — Type 'exit' to quit.\n", AISHELL_VERSION);

    while (1) {
        printf("aishell> ");
        fflush(stdout);

        if (!fgets(line, sizeof(line), stdin)) {
            printf("\n");
            break;
        }

        // Trim newline
        line[strcspn(line, "\r\n")] = '\0';
        if (strlen(line) == 0) continue;
        if (strcmp(line, "exit") == 0 || strcmp(line, "quit") == 0) break;

        // Simple tokenization (convert line into argc/argv)
        char *cmd_argv[64];
        int cmd_argc = 0;
        char *token = strtok(line, " \t");

        while (token && cmd_argc < 63) {
            cmd_argv[cmd_argc++] = token;
            token = strtok(NULL, " \t");
        }
        cmd_argv[cmd_argc] = NULL;

        if (cmd_argc == 0) continue;

        // Command Registry Lookup
        const cmd_spec_t *spec = find_command(cmd_argv[0]);
        if (spec) {
            spec->run(cmd_argc, cmd_argv);
        } else {
            fprintf(stderr, "aishell: command not found: %s\n", cmd_argv[0]);
        }
    }

    return 0;
}