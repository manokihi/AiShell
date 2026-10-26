#ifndef REGISTRY_H
#define REGISTRY_H

#include <stdio.h>

/**
 * Command specification structure defining a command module's identity,
 * metadata, execution entry point, and usage help formatter.
 */
typedef struct cmd_spec {
    const char *name;        // Command name, e.g., "sysinfo"
    const char *summary;     // One-line description
    const char *long_help;   // Longer description / Markdown (may be NULL)

    // Main entry point: parses args and runs the command.
    int (*run)(int argc, char **argv);

    // Prints usage and option help to the given stream.
    void (*print_usage)(FILE *out);
} cmd_spec_t;

/**
 * Registers a command specification into the shell's dynamic registry.
 * @param spec Pointer to the command specification.
 */
void register_command(const cmd_spec_t *spec);

/**
 * Finds a registered command by its name.
 * @param name The command name to search for.
 * @return Pointer to the command specification if found, or NULL otherwise.
 */
const cmd_spec_t *find_command(const char *name);

/**
 * Iterates over all registered commands, invoking the callback function for each.
 * @param cb Callback function taking a command spec and optional userdata.
 * @param userdata User-defined pointer passed to the callback.
 */
void for_each_command(void (*cb)(const cmd_spec_t *spec, void *userdata), void *userdata);

#endif /* REGISTRY_H */