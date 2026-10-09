Week 2 Work Log (week-02.md)
Project: AiShell — Modular POSIX C Shell Suite

Author: Student

Course: Linux Systems Programming

1. Overview & Objectives
The primary objective for Week 2 was expanding the core command module suite within AiShell, transitioning test execution from standard Makefile targets to an automated integration test runner (scripts/test.sh), and standardizing core command signatures and module registration.

All command modules were built against the canonical cmd_spec_t interface, utilizing static argtable3 amalgamation for argument parsing and POSIX system calls for direct filesystem and environment interaction.

2. Work Completed
Module Implementations
aipwd (Print Working Directory)

Implemented logical (-L) and physical (-P) path resolution.

Utilized getenv("PWD") for logical directory traversal and getcwd(3) for physical link resolution.

Standardized command structure around aipwd_run and cmd_aipwd.

aicd (Change Directory)

Implemented directory switching logic with environment variable management ($PWD and $OLDPWD).

Integrated target expansion for $HOME (default when no argument is supplied), $OLDPWD (-), and tilde expansion (~ and ~/path).

Supported -L and -P flags alongside --json output options.

ails (List Directory Contents)

Implemented directory content listing using standard POSIX opendir(3), readdir(3), and closedir(3).

Built support for combined flag parsing (-al), long-listing formats using stat(2) for detailed file metadata, and suppressed hidden dotfiles by default unless -a / --all is passed.

Standardized registration using an explicit register_ails_command() helper.

Build System & Tooling Enhancements
POSIX Macro Conformance: Updated C source files to explicitly include #define _POSIX_C_SOURCE 200809L prior to system headers, resolving strict -std=c99 -pedantic compilation issues with PATH_MAX and POSIX glibc extensions.

Makefile Integration: Updated CMD_SRCS and ALL_BINS targets to build both individual standalone binaries (bin/aipwd, bin/aicd, bin/ails) and the integrated bin/aishell REPL binary.

Integration Test Framework (scripts/test.sh):

Migrated inline Makefile test rules to an automated Bash integration test runner.

Implemented structured test runners (run_pass_test and run_fail_test) to validate both successful executions (exit code 0) and error path/invalid flag handling (non-zero exit codes).

3. Summary of Key Files Added / Updated
AiShell/
├── Makefile                          # Updated binary targets & delegated 'test' rule
├── scripts/
│   └── test.sh                       # Automated test suite for success and error paths
└── src/
    ├── main.c                        # REPL entry point registering all command modules
    └── commands/
        ├── aipwd/
        │   ├── cmd_aipwd.h
        │   ├── cmd_aipwd.c
        │   └── aipwd_main.c
        ├── aicd/
        │   ├── cmd_aicd.h
        │   ├── cmd_aicd.c
        │   └── aicd_main.c
        └── ails/
            ├── cmd_ails.h
            ├── cmd_ails.c
            └── ails_main.c
4. Verification & Testing
Ran clean build and executed the automated test harness:

Bash
make clean
make test
Verification Results
aisysinfo: Verified -v, --json, -h, and error paths for unexpected positional arguments.

aipwd: Verified default execution, -L, -P, --json, and error paths.

aicd: Verified directory changes to $HOME, relative/absolute paths, non-existent directory error handling, and JSON formatted status output.

ails: Verified directory listing, -a, -l, combined flags (-al), --json, and non-existent path error paths.

aishell REPL: Verified command registration and direct command dispatch from the unified REPL interface.

All tests passed with zero failures.

5. Next Steps
Proceed to Week 3 requirements (expanding shell built-in pipeline execution, signal handling, or process execution controls).