#!/usr/bin/env bash
set -e

echo "=== Running Baseline Acceptance Tests ==="
./aishell --help > /dev/null
./aishell --all > /dev/null
./aishell -s -r > /dev/null
./aishell --version > /dev/null

echo "=== Testing Error Handling Paths ==="
if ./aishell --invalid-flag 2>/dev/null; then
    echo "ERROR: --invalid-flag should have failed!"
    exit 1
fi

if ./aishell unexpected_arg 2>/dev/null; then
    echo "ERROR: positional argument should have failed!"
    exit 1
fi

echo "All Week 1 acceptance tests passed successfully!"