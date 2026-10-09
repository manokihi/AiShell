```markdown
# ails

List information about files and directory contents.

## Synopsis

```text
ails [-al] [path] [-h] [-v] [--json]
Options
Flag / Arg	Long Option	Description
[path]		Target directory or file path (defaults to .)
-a	--all	Do not ignore entries starting with .
-l	--long	Use long listing format showing file type, permissions, size, and name
-h	--help	Show usage message and exit
-v	--version	Show version information and exit
--json		Output directory entry array in JSON format
Examples
Standard listing:

Bash
$ ails
src
Makefile
README.md
Combined short flags for detailed hidden file listing:

Bash
$ ails -al
drwx 4096 .
drwx 4096 ..
-rwx 1024 Makefile
drwx 4096 src
JSON output:

Bash
$ ails --json
{"target":".","entries":["src","Makefile","README.md"]}
```
