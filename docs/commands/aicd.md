```markdown
# aicd

Change the current working directory and synchronize shell environment variables (`$PWD` and `$OLDPWD`).

## Synopsis

```text
aicd [-LP] [dir] [-h] [-v] [--json]
OptionsFlag / ArgLong OptionDescription[dir]Target directory (defaults to $HOME if omitted; - changes to $OLDPWD)-L--logicalForce logical directory structure [default]-P--physicalResolve physical directory structure-h--helpShow usage message and exit-v--versionShow version information and exit--jsonOutput updated path state in JSON formatSpecial Path ExpansionNo arguments: Changes directory to $HOME.- (Hyphen): Changes directory to $OLDPWD and prints the new directory to stdout.~ / ~/path: Expands tilde prefix to user's $HOME path.ExamplesChange to target directory:Bash$ aicd /tmp
Return to previous directory ($OLDPWD):Bash$ aicd -
/home/user/projects/AiShell
JSON output:Bash$ aicd --json /tmp
{"pwd":"/tmp","oldpwd":"/home/user/projects/AiShell"}
```
