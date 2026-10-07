#include <stdio.h>
#include <string.h>
#include "cmd_spec.h"

#define MAX_COMMANDS 64

static const cmd_spec_t *g_registry[MAX_COMMANDS];
static int g_command_count = 0;

void register_command(const cmd_spec_t *spec) {
    if (!spec || !spec->name) return;
    if (g_command_count >= MAX_COMMANDS) {
        fprintf(stderr, "Warning: Command registry full. Cannot register '%s'\n", spec->name);
        return;
    }
    // Avoid duplicate registration
    for (int i = 0; i < g_command_count; i++) {
        if (strcmp(g_registry[i]->name, spec->name) == 0) {
            return;
        }
    }
    g_registry[g_command_count++] = spec;
}

const cmd_spec_t *find_command(const char *name) {
    if (!name) return NULL;
    for (int i = 0; i < g_command_count; i++) {
        if (strcmp(g_registry[i]->name, name) == 0) {
            return g_registry[i];
        }
    }
    return NULL;
}

void for_each_command(void (*cb)(const cmd_spec_t *spec, void *userdata), void *userdata) {
    if (!cb) return;
    for (int i = 0; i < g_command_count; i++) {
        cb(g_registry[i], userdata);
    }
}