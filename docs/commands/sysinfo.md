# `cmdbox`
# `sysinfo` Command Reference

Displays system information using the POSIX `uname()` system API.

## Usage
``ctext
aishell [sysinfo] [OPTIONS]
```

## Options
| Flag | Long Flag | Description |
| :--- | :--- | :--- |
| `-h` | `--help` | Display usage message and exit 0 |
| `-a` | `--all` | Display all system fields (default) |
| `-s` | `--kernel-name` | Display OS kernel name |
| `-r` | `--release` | Display kernel release version |
| `-m` | `--machine` | Display hardware architecture |
| `-n` | `--nodename` | Display network node hostname |
| `-v` | `--version` | Display version information and exit 0 |

## Exit Status
- `0`: Success or help/version requested cleanly.
- `1`: Invalid flag or unexpected positional argument encountered.