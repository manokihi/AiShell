#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <argtable3.h>
#include "cmd_aipwd.h"

typedef struct {
    struct arg_lit *logical;
    struct arg_lit *physical;
    ai_global_args_t globals;
    struct arg_end *end;
} aipwd_args_t;

static void print_usage(FILE *out) {
    fprintf(out, "Usage: aipwd [-LP] [-h] [-v] [--json]\n");
    fprintf(out, "Print the current working directory.\n\n");
    fprintf(out, "Options:\n");
    fprintf(out, "  -L, --logical   print logical PWD (preserve symlinks) [default]\n");
    fprintf(out, "  -P, --physical  print physical PWD (resolve all symlinks)\n");
    fprintf(out, "  -h, --help      show this help message and exit\n");
    fprintf(out, "  -v, --version   show version information and exit\n");
    fprintf(out, "  --json          output results in JSON format\n");
}

static int aipwd_run(int argc, char **argv) {
    aipwd_args_t args;
    args.logical  = arg_lit0("L", "logical",  "print logical current working directory");
    args.physical = arg_lit0("P", "physical", "print physical current working directory");
    ai_init_global_args(&args.globals);
    args.end      = arg_end(20);

    void *argtable[] = {
        args.logical,
        args.physical,
        args.globals.help,
        args.globals.version,
        args.globals.json,
        args.end
    };

    int nerrors = arg_parse(argc, argv, argtable);

    if (nerrors > 0) {
        arg_print_errors(stderr, args.end, "aipwd");
        arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
        return 1;
    }

    if (ai_handle_global_flags(&args.globals, &cmd_aipwd, stdout)) {
        arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
        return 0;
    }

    int physical = (args.physical->count > 0);
    char cwd_buf[PATH_MAX];
    char *pwd_str = NULL;

    if (physical) {
        if (getcwd(cwd_buf, sizeof(cwd_buf)) != NULL) {
            pwd_str = cwd_buf;
        }
    } else {
        /* Logical preferred: try $PWD first, fallback to getcwd */
        char *env_pwd = getenv("PWD");
        if (env_pwd && env_pwd[0] != '\0') {
            pwd_str = env_pwd;
        } else if (getcwd(cwd_buf, sizeof(cwd_buf)) != NULL) {
            pwd_str = cwd_buf;
        }
    }

    if (!pwd_str) {
        fprintf(stderr, "aipwd: error retrieving current directory\n");
        arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
        return 1;
    }

    if (args.globals.json->count > 0) {
        fprintf(stdout, "{\"pwd\":\"%s\",\"type\":\"%s\"}\n", 
                pwd_str, physical ? "physical" : "logical");
    } else {
        fprintf(stdout, "%s\n", pwd_str);
    }

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
    return 0;
}

const cmd_spec_t cmd_aipwd = {
    .name = "aipwd",
    .summary = "Print current working directory",
    .run = aipwd_run,
    .print_usage = print_usage
};

void register_aipwd_command(void) {
    register_command(&cmd_aipwd);
}