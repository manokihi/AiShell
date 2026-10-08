#!/usr/bin/env bash
set -euo pipefail

BIN_DIR="./bin"
PASSED=0
FAILED=0

echo "=========================================="
echo " Running AiShell Test Suite"
echo "=========================================="

# Helper: Expect command to succeed (exit code 0)
run_pass_test() {
    local name="$1"
    shift
    echo -n "Testing [$name]... "
    if "$@" > /dev/null 2>&1; then
        echo "PASS"
        PASSED=$((PASSED + 1))
    else
        echo "FAIL (Expected exit code 0)"
        FAILED=$((FAILED + 1))
    fi
}

# Helper: Expect command to fail (non-zero exit code)
run_fail_test() {
    local name="$1"
    shift
    echo -n "Testing Error Path [$name]... "
    if ! "$@" > /dev/null 2>&1; then
        echo "PASS"
        PASSED=$((PASSED + 1))
    else
        echo "FAIL (Expected non-zero exit code, but succeeded)"
        FAILED=$((FAILED + 1))
    fi
}

# ------------------------------------------------------------------------------
# aisysinfo Tests
# ------------------------------------------------------------------------------
echo "--- aisysinfo ---"
run_pass_test "aisysinfo default"      "$BIN_DIR/aisysinfo"
run_pass_test "aisysinfo --version"    "$BIN_DIR/aisysinfo" --version
run_pass_test "aisysinfo --json"       "$BIN_DIR/aisysinfo" --json
run_pass_test "aisysinfo -h"           "$BIN_DIR/aisysinfo" -h

# Error conditions
run_fail_test "aisysinfo invalid flag" "$BIN_DIR/aisysinfo" --invalid-flag
run_fail_test "aisysinfo extra arg"    "$BIN_DIR/aisysinfo" unexpected_argument

# ------------------------------------------------------------------------------
# aipwd Tests
# ------------------------------------------------------------------------------
echo "--- aipwd ---"
run_pass_test "aipwd default"          "$BIN_DIR/aipwd"
run_pass_test "aipwd -L"               "$BIN_DIR/aipwd" -L
run_pass_test "aipwd -P"               "$BIN_DIR/aipwd" -P
run_pass_test "aipwd --json"           "$BIN_DIR/aipwd" --json
run_pass_test "aipwd --version"        "$BIN_DIR/aipwd" -v
run_pass_test "aipwd --help"           "$BIN_DIR/aipwd" -h

# Error conditions
run_fail_test "aipwd invalid flag"     "$BIN_DIR/aipwd" --invalid-flag
run_fail_test "aipwd extra positional" "$BIN_DIR/aipwd" extra_arg

# ------------------------------------------------------------------------------
# aicd Tests
# ------------------------------------------------------------------------------
echo "--- aicd ---"
run_pass_test "aicd default ($HOME)"   "$BIN_DIR/aicd"
run_pass_test "aicd to /tmp"           "$BIN_DIR/aicd" /tmp
run_pass_test "aicd --version"         "$BIN_DIR/aicd" -v
run_pass_test "aicd --help"            "$BIN_DIR/aicd" -h
run_pass_test "aicd --json"            "$BIN_DIR/aicd" --json /tmp

# Error conditions
run_fail_test "aicd non-existent dir"  "$BIN_DIR/aicd" /non_existent_directory_xyz
run_fail_test "aicd invalid flag"      "$BIN_DIR/aicd" --invalid-flagecho "=========================================="
echo " Results: $PASSED Passed, $FAILED Failed"
echo "=========================================="

# ------------------------------------------------------------------------------
# ails Tests
# ------------------------------------------------------------------------------
echo "--- ails ---"
run_pass_test "ails default"           "$BIN_DIR/ails"
run_pass_test "ails -a"                "$BIN_DIR/ails" -a
run_pass_test "ails -l"                "$BIN_DIR/ails" -l
run_pass_test "ails --version"         "$BIN_DIR/ails" -v
run_pass_test "ails --help"            "$BIN_DIR/ails" -h
run_pass_test "ails --json"            "$BIN_DIR/ails" --json

# Error conditions
run_fail_test "ails non-existent dir"  "$BIN_DIR/ails" /non_existent_dir_12345
run_fail_test "ails invalid flag"      "$BIN_DIR/ails" --invalid-flag

# ------------------------------------------------------------------------------
# REPL Command Dispatch & Errors
# ------------------------------------------------------------------------------
echo "--- aishell REPL ---"
run_pass_test "aishell aisysinfo"      "$BIN_DIR/aishell" aisysinfo --version
run_pass_test "aishell aipwd"          "$BIN_DIR/aishell" aipwd
run_pass_test "aishell aicd"           "$BIN_DIR/aishell" aicd /tmp
run_pass_test "aishell ails"           "$BIN_DIR/aishell" ails -al
run_fail_test "aishell unknown cmd"    "$BIN_DIR/aishell" unknowncommand

if [ "$FAILED" -ne 0 ]; then
    exit 1
fi