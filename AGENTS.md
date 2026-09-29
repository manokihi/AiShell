# AI Collaboration Directives for AiShell

## Project Purpose
AiShell is a 10-week incremental POSIX shell and systems project for the *Linux Systems Programming* course. 

## Strict Repository & Directory Rules
1. **`docs/` Restriction**: Reserved **EXCLUSIVELY** for user-facing shell documentation and CLI command reference guides (`docs/commands/<tool>.md`).
2. **Work Logs**: Place all prompt logs, interaction summaries, and progress history in `logs/week-XX.md`.
3. **Submissions**: Place weekly demo scripts, checklists, and turn-in deliverables in `submission/week-XX/`.
4. **Secrets & Security**: **NEVER commit secrets, API keys, credentials, or tokens.** Always rely on environment variables (`getenv()`) or `.env` files explicitly excluded by `.gitignore`.

## Coding Standards & Quality Gates
- **Language**: ISO C11 standard (`-std=c11`).
- **Compiler Flags**: All C code must compile cleanly with `-Wall -Wextra -Werror -pedantic`.
- **System Calls**: Always verify return values of POSIX system calls and print actionable error messages to `stderr` using `perror()` or `fprintf()`.
- **Memory Safety**: Free all dynamically allocated memory (`malloc`/`free`) and prevent file descriptor leaks.

## Commit Guidelines
- Use descriptive commit messages following the Conventional Commits format (e.g., `feat(sysinfo): add kernel and arch flags`, `docs(week01): update command usage`).
- Never commit broken builds or unverified code to `main`.