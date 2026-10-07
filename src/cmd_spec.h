#ifndef CMD_SPEC_H
#define CMD_SPEC_H

#include <stdio.h>
#include <third-party/argtable3/argtable3.h>

#define AISHELL_VERSION "0.2.0"

typedef struct cmd_spec {
    const char *name;
    const char *summary;
    const char *long_help;
    int (*run)(int argc, char **argv);
    void (*print_usage)(FILE *out);
} cmd_spec_t;

// Global options shared across all commands
typedef struct {
    struct arg_lit *help;     // -h, --help
    struct arg_lit *json;     // --json
    struct arg_lit *version;  // -v, --version
} ai_global_args_t;

void ai_init_global_args(ai_global_args_t *globals);
int  ai_handle_global_flags(const ai_global_args_t *globals, const cmd_spec_t *spec, FILE *out);

// Command Registry interface
void register_command(const cmd_spec_t *spec);
const cmd_spec_t *find_command(const char *name);
void for_each_command(void (*cb)(const cmd_spec_t *spec, void *userdata), void *userdata);

#endif // CMD_SPEC_H