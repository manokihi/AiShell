# AiShell Documentation

`AiShell` is a modular, POSIX-compliant C shell suite and collection of core UNIX utilities designed with static `argtable3` argument parsing, structured JSON output options, and unified command registration.

## Architecture

* **Modular Registry:** Commands implement the canonical `cmd_spec_t` interface and register execution hooks into the central shell REPL.
* **Dual Execution Modes:** Every command compiles both as a standalone utility binary (`bin/<cmd>`) and as an integrated built-in within `bin/aishell`.
* **Standardized Flags:** All commands support `-h` / `--help`, `-v` / `--version`, and `--json` machine-readable output.

## Available Commands

| Command | Summary | Standalone Binary | Built-in |
| :--- | :--- | :--- | :--- |
| [`aisysinfo`](commands/aisysinfo.md) | Display system metrics and memory usage | `bin/aisysinfo` | Yes |
| [`aipwd`](commands/aipwd.md) | Print current working directory | `bin/aipwd` | Yes |
| [`aicd`](commands/aicd.md) | Change current working directory | `bin/aicd` | Yes |
| [`ails`](commands/ails.md) | List directory contents | `bin/ails` | Yes |

## Building and Testing

```bash
# Build all binaries
make all

# Run integration test suite
make test