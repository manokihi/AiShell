# AI Work Log — Week 01

**Course:** Linux Systems Programming  
**Project:** AiShell Milestone 1 Baseline  
**Date:** September 2026  

---

## 1. Intent & Prompt Objectives
- **Repository Setup:** Established a clean, modular repository skeleton adhering to C11/POSIX standards and strict directory isolation (`src/`, `docs/`, `logs/`, `submission/`, `scripts/`).
- **Build Infrastructure:** Wrote a strict `Makefile` enforcing `-Wall -Wextra -Werror -std=c11 -pedantic`.
- **POSIX Tool Development:** Implemented `sysinfo` wrapping the `uname()` POSIX API with robust option parsing via `getopt_long()`.
- **Quality Assurance & Verification:** Developed an automated test harness (`scripts/test.sh`) to validate both happy paths and error paths (exit code `0` on success/help, non-zero on invalid flags).

## 2. Key Technical Decisions & AI Recommendations
- **Re-entrancy in C Code:** Ensured `run_sysinfo()` explicitly resets `optind = 1` before invoking `getopt_long()`, ensuring global flag state is clean when invoked repeatedly in future interactive shell loops.
- **Stream Separation:** Enforced printing data and `--help` to `stdout`, while routing error messages and usage hints strictly to `stderr`.
- **Directory Discipline:** Reserved `docs/` exclusively for Markdown command reference documentation; logs stored in `logs/` and grading materials in `submission/`.
- **Git & SSH Security:** Verified GitHub ED25519 host key fingerprint before authenticating host access.

## 3. Prompts Summary
1. *"Help me initialize the AiShell repo structure and establish coding guidelines in `AGENTS.md`."*
2. *"Write a Makefile using strict C11 flags and generate the C implementation for `sysinfo` using POSIX `uname()`."*
3. *"Create an automated test script (`scripts/test.sh`) to test happy and error paths."*
4. *"Generate documentation in `docs/` and submission checklists for Milestone 1."*
5. *"Review, compile, test, and troubleshoot Markdown/base64 formatting for auxiliary files."*

## 4. Verification & Validation
- **Compilation:** Clean build using `make` with zero warnings under strict flags.
- **Test Harness:** `scripts/test.sh` executed and passed all happy and error path checks.
- **Remote Push:** Staged, committed, tagged (`v0.1.0-week01`), and pushed to GitHub remote `main`.