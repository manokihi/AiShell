#ifndef SYSINFO_H
#define SYSINFO_H

#define SYSINFO_VERSION "1.0.0"

/**
 * Executes the sysinfo CLI tool.
 *
 * @param argc Argument count passed to the tool.
 * @param argv Argument string array passed to the tool.
 * @return 0 on success, non-zero on error or invalid flag.
 */
int run_sysinfo(int argc, char *argv[]);

#endif /* SYSINFO_H */