# AI Work Log - Week 01

**Course:** Linux Systems Programming  
**Project:** AiShell Milestone 1 Baseline  

## Prompts & Intent Summary
1. Established 10-week project roadmap and AI assistant constraints.
2. Defined repository layout and strict separation of concerns (keeping `docs/` strictly for command reference).
3. Configured local Git repository and GitHub remote using `gh` CLI.
4. Drafted `AGENTS.md` for AI collaboration guidelines and security constraints.

## Changes & Artifacts Created
- `AGENTS.md`: Collaboration guidelines and security rules (no secrets committed).
- `Makefile`: Strict standard C build configuration (`-Wall -Wextra -Werror -std=c11 -pedantic`).
- `docs/commands/sysinfo.md`: CLI reference doc for `sysinfo`.
- `scripts/test.sh`: Automated test harness for happy and error paths.
- `submission/week-01/`: Acceptance matrix and demo script outline.

## Verification
- Built binary using `make`.
- Executed `scripts/test.sh` to confirm proper exit codes (`0` on success/help, `1` on error).