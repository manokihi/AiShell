#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "main.h"
#include "registry/registry.h"

static int check_and_bootstrap_pkg(void) {
    const char *home = getenv("HOME");
    if (!home) {
        fprintf(stderr, "Error: Environment variable 'HOME' not set.\n");
        return -1;
    }

    char installed_path[MAX_LINE_LENGTH];
    snprintf(installed_path, sizeof(installed_path), "%s/.mysh/installed/pkg", home);

    // Check if the 'pkg' package is installed under ~/.mysh/installed/
    if (access(installed_path, F_OK) != 0) {
        printf("========================================\n");
        printf("  Welcome to AiShell (Modular Architecture)\n");
        printf("========================================\n");
        printf("The package manager ('pkg') is not installed.\n");
        printf("Installing default package from ~/.mysh/dist/...\n");

        char tarball_path[MAX_LINE_LENGTH];
        snprintf(tarball_path, sizeof(tarball_path), "%s/.mysh/dist/pkg.tar.gz", home);

        // Ensure the installation target directory exists
        char mkdir_cmd[MAX_LINE_LENGTH];
        snprintf(mkdir_cmd, sizeof(mkdir_cmd), "mkdir -p \"%s\"", installed_path);
        if (system(mkdir_cmd) != 0) {
            fprintf(stderr, "Error: Failed to create installation directory.\n");
            return -1;
        }

        // Extract the distribution package
        char cmd[MAX_LINE_LENGTH];
        snprintf(cmd, sizeof(cmd), "tar -xzf \"%s\" -C \"%s\"", tarball_path, installed_path);

        int rc = system(cmd);
        if (rc == 0) {
            printf("Package 'pkg' (v1.0.0) successfully installed!\n");
            printf("Default package installed successfully. Starting AiShell...\n");
        } else {
            fprintf(stderr, "Error: Failed to extract pkg.tar.gz during bootstrap.\n");
            return -1;
        }
    }
    return 0;
}

// Helper to tokenize a command line into argc/argv
static int parse_line(char *line, char **argv, int max_args) {
    int argc = 0;
    char *token = strtok(line, " \t\n");
    while (token != NULL && argc < max_args - 1) {
        argv[argc++] = token;
        token = strtok(NULL, " \t\n");
    }
    argv[argc] = NULL;
    return argc;
}

static void run_interactive_loop(void) {
    printf("AiShell ready. Type commands or 'pkg --help' for options.\n");
    char line[MAX_LINE_LENGTH];

    while (1) {
        printf("aishell> ");
        if (fgets(line, sizeof(line), stdin) == NULL) {
            printf("\nExiting AiShell.\n");
            break;
        }

        // Remove trailing newline
        line[strcspn(line, "\n")] = 0;
        if (strlen(line) == 0) continue;

        // Handle exit commands
        if (strcmp(line, "exit") == 0 || strcmp(line, "quit") == 0) {
            printf("Exiting AiShell.\n");
            break;
        }

        // Tokenize line into argv
        char *argv[64];
        int argc = parse_line(line, argv, 64);
        if (argc == 0) continue;

        // Command Registry Dispatch Lookup[cite: 3]
        const cmd_spec_t *spec = find_command(argv[0]);
        if (!spec) {
            fprintf(stderr, "aishell: command not found: %s\n", argv[0]);
            continue;
        }

        // Execute the registered command's run handler[cite: 3]
        int ret = spec->run(argc, argv);
        if (ret != 0) {
            // Optional: Handle non-zero exit status if desired
        }
    }
}

int main(int argc, char *argv[]) {
    // If invoked with arguments, check registry or handle directly
    if (argc > 1) {
        const cmd_spec_t *cmd = find_command(argv[1]);
        if (cmd != NULL) {
            return cmd->run(argc - 1, &argv[1]);
        }
        fprintf(stderr, "aishell: command not found: %s\n", argv[1]);
        return 1;
    }

    if (check_and_bootstrap_pkg() != 0) {
        fprintf(stderr, "aishell initialization failed");
        return 1;
    }
    run_interactive_loop();

    return 0;
}