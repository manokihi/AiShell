#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include "cmd_ails.h"
#include "argtable3.h"

typedef struct {
    struct arg_lit *all;
    struct arg_lit *long_fmt;
    struct arg_file *path;
    ai_global_args_t globals;
    struct arg_end *end;
} ails_args_t;

static void print_usage(FILE *out) {
    fprintf(out, "Usage: ails [-al] [path] [-h] [-v] [--json]\n");
    fprintf(out, "List directory contents.\n\n");
    fprintf(out, "Options:\n");
    fprintf(out, "  -a, --all       do not ignore entries starting with .\n");
    fprintf(out, "  -l, --long      use a long listing format\n");
    fprintf(out, "  -h, --help      show this help message and exit\n");
    fprintf(out, "  -v, --version   show version information and exit\n");
    fprintf(out, "  --json          output results in JSON format\n");
}

static int ails_run(int argc, char **argv) {
    ails_args_t args;
    args.all      = arg_lit0("a", "all",  "do not ignore entries starting with .");
    args.long_fmt = arg_lit0("l", "long", "use a long listing format");
    args.path     = arg_file0(NULL, NULL, "<path>", "target file or directory");
    ai_init_global_args(&args.globals);
    args.end      = arg_end(20);

    void *argtable[] = {
        args.all,
        args.long_fmt,
        args.path,
        args.globals.help,
        args.globals.version,
        args.globals.json,
        args.end
    };

    int nerrors = arg_parse(argc, argv, argtable);

    if (nerrors > 0) {
        arg_print_errors(stderr, args.end, "ails");
        arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
        return 1;
    }

    if (ai_handle_global_flags(&args.globals, &cmd_ails, stdout)) {
        arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
        return 0;
    }

    const char *target = (args.path->count > 0) ? args.path->filename[0] : ".";
    int show_all = (args.all->count > 0);
    int is_long  = (args.long_fmt->count > 0);
    int is_json  = (args.globals.json->count > 0);

    DIR *dir = opendir(target);
    if (!dir) {
        perror("ails");
        arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
        return 1;
    }

    struct dirent *entry;
    int first_json = 1;

    if (is_json) {
        printf("{\"target\":\"%s\",\"entries\":[", target);
    }

    while ((entry = readdir(dir)) != NULL) {
        if (!show_all && entry->d_name[0] == '.') {
            continue;
        }

        if (is_json) {
            if (!first_json) printf(",");
            printf("\"%s\"", entry->d_name);
            first_json = 0;
        } else if (is_long) {
            char full_path[1024];
            snprintf(full_path, sizeof(full_path), "%s/%s", target, entry->d_name);
            struct stat st;
            if (stat(full_path, &st) == 0) {
                printf((S_ISDIR(st.st_mode)) ? "d" : "-");
                printf((st.st_mode & S_IRUSR) ? "r" : "-");
                printf((st.st_mode & S_IWUSR) ? "w" : "-");
                printf((st.st_mode & S_IXUSR) ? "x" : "-");
                printf(" %ld %s\n", (long)st.st_size, entry->d_name);
            } else {
                printf("%s\n", entry->d_name);
            }
        } else {
            printf("%s\n", entry->d_name);
        }
    }

    if (is_json) {
        printf("]}\n");
    }

    closedir(dir);
    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
    return 0;
}

const cmd_spec_t cmd_ails = {
    .name = "ails",
    .summary = "List directory contents",
    .long_help = "List information about files in DIR (the current directory by default).",
    .run = ails_run,
    .print_usage = print_usage
};

void register_ails_command(void) {
    register_command(&cmd_ails);
}