```markdown
# aisysinfo

Display operating system information, kernel versions, system uptime, and memory usage statistics.

## Synopsis

```text
aisysinfo [-hvaskum] [--json]
OptionsFlagLong OptionDescription-h--helpShow usage message and exit-v--versionShow version information and exit--jsonOutput system information in JSON format-a--allDisplay all system information metrics (default)-s--sysnameDisplay operating system name-k--kernelDisplay kernel release version-u--uptimeDisplay system uptime in seconds-m--memoryDisplay total and free memory statsExamplesStandard output:Bash$ aisysinfo
OS: Linux
Kernel: 7.0.0-34-generic
Uptime: 49876s
Memory: 16176435200 bytes total / 6078787584 bytes free
JSON output:Bash$ aisysinfo --json
{"sysname":"Linux","kernel":"7.0.0-34-generic","uptime_sec":49876,"total_ram":16176435200,"free_ram":6078787584}
```
