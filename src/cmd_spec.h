#ifndef CMD_SPEC_H
#define CMD_SPEC_H

#include <stdio.h>
#include <third-party/argtable3/argtable3.h>
#include "registry/registry.h"

#define AISHELL_VERSION "0.2.0"

// Global options shared across all tools
typedef struct {
    struct arg_lit *help;     // -h, --help
    struct arg_lit *json;     // --json
    struct arg_lit *version;  // -v, --version
} ai_global_args_t;

void ai_init_global_args(ai_global_args_t *globals);
int  ai_handle_global_flags(const ai_global_args_t *globals, const cmd_spec_t *spec, FILE *out);
#endif // CMD_SPEC_H