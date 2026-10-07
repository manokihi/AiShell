#include <stdio.h>
#include <stdlib.h>
#include "cmd_spec.h"

void ai_init_global_args(ai_global_args_t *globals) {
    globals->help    = arg_lit0("h", "help",    "show this help message and exit");
    globals->json    = arg_lit0(NULL, "json",   "output results in JSON format");
    globals->version = arg_lit0("v", "version", "show version information and exit");
}

int ai_handle_global_flags(const ai_global_args_t *globals, 
                           const cmd_spec_t *spec, 
                           FILE *out) 
{
    if (globals->help && globals->help->count > 0) {
        if (spec && spec->print_usage) {
            spec->print_usage(out);
        }
        return 1;
    }

    if (globals->version && globals->version->count > 0) {
        if (globals->json && globals->json->count > 0) {
            fprintf(out, "{\"command\":\"%s\",\"version\":\"%s\"}\n", 
                    spec ? spec->name : "aishell", AISHELL_VERSION);
        } else {
            fprintf(out, "%s version %s\n", 
                    spec ? spec->name : "aishell", AISHELL_VERSION);
        }
        return 1;
    }

    return 0;
}