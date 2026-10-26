//
// Created by Kamil Synek on 10/24/26.
//
#include <string.h>
#include "registry.h"

#define MAX_COMMANDS 64

static const cmd_spec_t *registry[MAX_COMMANDS];
static int registry_count = 0;

void register_command(const cmd_spec_t *spec) {
    if (spec == NULL) {
        return;
    }
    // Prevent duplicate registrations
    for (int i = 0; i < registry_count; i++) {
        if (strcmp(registry[i]->name, spec->name) == 0) {
            registry[i] = spec; // Update existing
            return;
        }
    }
    if (registry_count < MAX_COMMANDS) {
        registry[registry_count++] = spec;
    }
}

const cmd_spec_t *find_command(const char *name) {
    if (name == NULL) {
        return NULL;
    }
    for (int i = 0; i < registry_count; i++) {
        if (strcmp(registry[i]->name, name) == 0) {
            return registry[i];
        }
    }
    return NULL;
}

void for_each_command(void (*cb)(const cmd_spec_t *spec, void *userdata), void *userdata) {
    if (cb == NULL) {
        return;
    }
    for (int i = 0; i < registry_count; i++) {
        cb(registry[i], userdata);
    }
}
