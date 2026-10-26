#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include "cmd_aicd.h"
#include "argtable3.h"

typedef struct {
    struct arg_lit *logical;
    struct arg_lit *physical;
    struct arg_file *dir;
    ai_global_args_t globals;
    struct arg_end *end;
} aicd_args_t;

static void print_usage(FILE *out) {
    fprintf(out, "Usage: aicd [-LP] [dir] [-h] [-v] [--json]\n");
    fprintf(out, "Change the current working directory.\n\n");
    fprintf(out, "Options:\n");
    fprintf(out, "  -L, --logical   force logical directory structure [default]\n");
    fprintf(out, "  -P, --physical  resolve physical directory structure\n");
    fprintf(out, "  -h, --help      show this help message and exit\n");
    fprintf(out, "  -v, --version   show version information and exit\n");
    fprintf(out, "  --json          output results in JSON format\n");
}

static int aicd_run(int argc, char **argv) {
    aicd_args_t args;
    args.logical  = arg_lit0("L", "logical",  "force logical directory structure");
    args.physical = arg_lit0("P", "physical", "resolve physical directory structure");
    args.dir      = arg_file0(NULL, NULL, "<dir>", "directory to change to");
    ai_init_global_args(&args.globals);
    args.end      = arg_end(20);

    void *argtable[] = {
        args.logical,
        args.physical,
        args.dir,
        args.globals.help,
        args.globals.version,
        args.globals.json,
        args.end
    };

    int nerrors = arg_parse(argc, argv, argtable);

    if (nerrors > 0) {
        arg_print_errors(stderr, args.end, "aicd");
        arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
        return 1;
    }

    if (ai_handle_global_flags(&args.globals, &cmd_aicd, stdout)) {
        arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
        return 0;
    }

    const char *target_dir = NULL;
    char expanded_path[PATH_MAX];
    int print_target = 0;

    if (args.dir->count == 0) {
        /* Default to $HOME */
        target_dir = getenv("HOME");
        if (!target_dir || target_dir[0] == '\0') {
            fprintf(stderr, "aicd: HOME not set\n");
            arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
            return 1;
        }
    } else {
        const char *raw_dir = args.dir->filename[0];
        if (strcmp(raw_dir, "-") == 0) {
            /* Change to $OLDPWD */
            target_dir = getenv("OLDPWD");
            if (!target_dir || target_dir[0] == '\0') {
                fprintf(stderr, "aicd: OLDPWD not set\n");
                arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
                return 1;
            }
            print_target = 1;
        } else if (raw_dir[0] == '~') {
            /* Handle tilde expansion */
            const char *home = getenv("HOME");
            if (!home) home = "";
            if (raw_dir[1] == '\0') {
                snprintf(expanded_path, sizeof(expanded_path), "%s", home);
            } else if (raw_dir[1] == '/') {
                snprintf(expanded_path, sizeof(expanded_path), "%s%s", home, raw_dir + 1);
            } else {
                snprintf(expanded_path, sizeof(expanded_path), "%s", raw_dir);
            }
            target_dir = expanded_path;
        } else {
            target_dir = raw_dir;
        }
    }

    /* Save current directory before changing */
    char current_cwd[PATH_MAX];
    if (getcwd(current_cwd, sizeof(current_cwd)) == NULL) {
        current_cwd[0] = '\0';
    }

    /* Perform directory change */
    if (chdir(target_dir) != 0) {
        perror("aicd");
        arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
        return 1;
    }

    /* Get updated absolute directory */
    char new_cwd[PATH_MAX];
    if (getcwd(new_cwd, sizeof(new_cwd)) == NULL) {
        snprintf(new_cwd, sizeof(new_cwd), "%s", target_dir);
    }

    /* Update environment variables PWD and OLDPWD */
    if (current_cwd[0] != '\0') {
        setenv("OLDPWD", current_cwd, 1);
    }
    setenv("PWD", new_cwd, 1);

    /* Print output */
    if (args.globals.json->count > 0) {
        printf("{\"pwd\":\"%s\",\"oldpwd\":\"%s\"}\n", new_cwd, current_cwd);
    } else if (print_target) {
        printf("%s\n", new_cwd);
    }

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
    return 0;
}

const cmd_spec_t cmd_aicd = {
    .name = "aicd",
    .summary = "Change the current working directory",
    .long_help = "Change the current working directory to DIR.",
    .run = aicd_run,
    .print_usage = print_usage
};

void register_aicd_command(void) {
    register_command(&cmd_aicd);
}