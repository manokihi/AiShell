# APP ANATOMY — Instructions for AI Refactoring

This document describes the standard anatomy of a command-line application in this course and provides guidelines for an AI assistant to refactor existing commands (e.g., `ls`, `wc`, `cat`, `pkg`) to match this specification[cite: 13].

**Scope:** Applies to all command modules that are part of the custom shell and its package ecosystem[cite: 13].

---

## 1. Primary Goals & Objectives

1. **Uniformity:** Establish one uniform structure per command across the codebase[cite: 13].
2. **Single Source of Truth:**
   - **CLI Options & Arguments:** Managed exclusively via `argtable3`[cite: 13].
   - **Help & Documentation:** Derived dynamically from `argtable3` structures[cite: 13].
   - **Shell Integration:** Standardized registration with the shell command registry[cite: 13].
   - **Packaging Metadata:** Generated for `pkg.json` and package documentation[cite: 13].

---

## 2. Core Target Structure: `cmd_spec_t` & Command Module

Every command module must define a command specification struct (`cmd_spec_t`), a primary `run` execution function, and a `print_usage` function leveraging `argtable3`[cite: 13]. Option wrappers allow standalone binary compilation[cite: 13].

### Standard Types Interface

```c
typedef struct cmd_spec {
    const char *name;        // Command name, e.g., "ls"
    const char *summary;     // One-line description
    const char *long_help;   // Longer description / Markdown (may be NULL)

    // Main entrypoint: parses args with argtable3 and runs the command
    int (*run)(int argc, char **argv);

    // Prints usage and option help (using argtable3) to the given stream
    void (*print_usage)(FILE *out);
} cmd_spec_t;

Specification Instance Example (cmd_ls.c)
Each command module must export exactly one instance of cmd_spec_t:

extern cmd_spec_t cmd_ls_spec;

cmd_spec_t cmd_ls_spec = {
    .name        = "ls",
    .summary     = "list directory contents",
    .long_help   = "List information about the FILES (the current directory by default).",
    .run         = ls_run,
    .print_usage = ls_print_usage,
};

## 3. Shell Integration & Command RegistryThe shell exposes a central command registry interface:

void register_command(const cmd_spec_t *spec);
const cmd_spec_t *find_command(const char *name);
void for_each_command(void (*cb)(const cmd_spec_t *spec, void *userdata), void *userdata);

Registration Helpers
Each command module provides a registration helper:

// Implemented in each command module
void register_ls_command(void) {
    register_command(&cmd_ls_spec);
}

During shell initialization, built-ins are registered centrally:

void register_all_builtin_commands(void) {
    register_ls_command();
    register_wc_command();
    register_cat_command();
    register_pkg_command();
}
```

```
Refactoring Rule: Avoid hard-coding command dispatch loops in the shell. The shell MUST use find_command() for execution and for_each_command() with spec->print_usage to generate help output.   4. argtable3 Usage & Execution Pattern   Every command's run and print_usage functions MUST use argtable3 as the single source of truth for CLI syntax.   Standard Argtable Construction Pattern
```
```c
static void build_ls_argtable(struct arg_lit **help,
                               struct arg_lit **all,
                               struct arg_file **paths,
                               struct arg_end **end,
                               void ***argtable_out)
{
    *help  = arg_lit0("h", "help", "show help and exit");
    *all   = arg_lit0("a", "all",  "do not ignore entries starting with .");
    *paths = arg_file0(NULL, NULL, "[PATH...]", "directories or files to list");
    *end   = arg_end(20);

    static void *argtable[5];
    argtable[0] = *help;
    argtable[1] = *all;
    argtable[2] = *paths;
    argtable[3] = *end;
    argtable[4] = NULL;

    *argtable_out = argtable;
}

Standard run() Execution Function

int ls_run(int argc, char **argv)
{
    struct arg_lit *help;
    struct arg_lit *all;
    struct arg_file *paths;
    struct arg_end *end;
    void **argtable;

    build_ls_argtable(&help, &all, &paths, &end, &argtable);

    int nerrors = arg_parse(argc, argv, argtable);

    if (help->count > 0) {
        ls_print_usage(stdout);
        return 0;
    }

    if (nerrors > 0) {
        arg_print_errors(stderr, end, "ls");
        ls_print_usage(stderr);
        return 1;
    }

    // Existing command logic goes here, using parsed argtable values
    // ...
}
```
```
Standard print_usage() Function
```
```c
void ls_print_usage(FILE *out)
{
    struct arg_lit *help;
    struct arg_lit *all;
    struct arg_file *paths;
    struct arg_end *end;
    void **argtable;

    build_ls_argtable(&help, &all, &paths, &end, &argtable);

    fprintf(out, "Usage: ls ");
    arg_print_syntax(out, argtable, "\n");
    fprintf(out, "\nOptions:\n");
    arg_print_glossary(out, argtable, "  %-20s %s\n");
}
```

```
5. Documentation Extraction & Packaging MetadataPackaging tools (pkg) do not parse raw C source code. Instead, documentation is extracted by:   Executing spec->print_usage() to capture text docs, OR   Invoking standalone binaries with special flags (--help-md or --help-json) derived from the same argtable3 definitions.   The metadata fields in cmd_spec_t (summary and long_help) populate:   Shell built-in help command output.   The description field inside pkg.json.   6. Standalone Binaries vs. Built-in CommandsModules MUST support two target build modes:   Built-in Shell Command: Compiled into an object library; linked into the shell binary and registered via register_<name>_command().   Standalone Binary: A minimal wrapper file (e.g., ls_main.c) delegating directly to the spec:
```
```c
int main(int argc, char **argv) {
    return cmd_ls_spec.run(argc, argv);
}
```

```
7. AI Refactoring Step-by-Step ChecklistWhen refactoring an existing command (e.g., ls.c, wc.c, cat.c), the AI agent MUST follow these steps:   Identify Command Scope: Determine command name, supported options, and positional arguments.   Create Standard Module: Implement or update cmd_<name>.c declaring run, print_usage, cmd_<name>_spec, and register_<name>_command().   Migrate Logic to run(): Move original main() logic into <name>_run(), replacing manual argv loops with argtable3.   Implement print_usage(): Output the usage syntax line and parameter glossary via argtable3.   Fill Metadata: Populate name, summary, and long_help fields in cmd_spec_t.   Add Main Wrapper: Create <name>_main.c calling cmd_<name>_spec.run(argc, argv) for standalone builds.   Register Command: Ensure register_<name>_command() invokes register_command(&cmd_<name>_spec).   Preserve Dependencies & Style: Rely strictly on C standard library, argtable3, and project conventions. Maintain surrounding coding style.   8. Special Case: pkg Command AnatomyThe package manager (pkg) follows the exact same anatomy:
```
```c
cmd_spec_t cmd_pkg_spec = {
    .name        = "pkg",
    .summary     = "manage mysh packages",
    .long_help   = "Build, install, list, remove, and upgrade packages for the mysh shell.",
    .run         = pkg_run,
    .print_usage = pkg_print_usage,
};
```
```
pkg additionally supports subcommands (e.g., build, install, list, remove, check-update, upgrade, compile), using argtable3 for subcommand option parsing while utilizing the central command registry to generate package documentation.
```