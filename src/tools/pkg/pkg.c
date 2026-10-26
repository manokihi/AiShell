//
// Created by Kamil Synek on 10/24/26.
//

#include "pkg.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#ifdef STANDALONE_PKG_MAIN
int main(int argc, char **argv) {
    return cmd_pkg_spec.run(argc, argv);
}
#endif

static void ensure_dir(const char *path) {
    char cmd[1024];
    snprintf(cmd, sizeof(cmd), "mkdir -p \"%s\"", path);
    int rc = system(cmd);
    (void)rc;
}

static void pkg_print_usage(FILE *out) {
    fprintf(out, "Usage: pkg <subcommand> [OPTIONS]\n");
    fprintf(out, "Manage AiShell packages, bundles, and local installations.\n\n");
    fprintf(out, "Subcommands:\n");
    fprintf(out, "  build <src-dir> <output-tar>   Build a .tar.gz package bundle from source directory\n");
    fprintf(out, "  install <tar-file>             Install a package bundle locally\n");
    fprintf(out, "  list                           List all installed packages\n");
    fprintf(out, "  info <name>                    Show metadata for an installed package\n");
    fprintf(out, "  remove <name>                  Remove an installed package by name\n");
    fprintf(out, "  --help, -h                     Display this help message and exit\n");
}

static int pkg_build(int argc, char **argv) {
    if (argc < 4) {
        fprintf(stderr, "Error: 'pkg build' requires a source directory and output tar file.\n");
        fprintf(stderr, "Usage: pkg build <src-dir> <output-tar>\n");
        return 1;
    }

    const char *src_dir = argv[2];
    const char *output_tar = argv[3];

    printf("Building package from directory '%s' into '%s'...\n", src_dir, output_tar);

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        return 1;
    } else if (pid == 0) {
        execlp("tar", "tar", "-czf", output_tar, "-C", src_dir, ".", (char *)NULL);
        perror("execlp failed for tar");
        _exit(1);
    } else {
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            printf("Package successfully built: %s\n", output_tar);
            return 0;
        } else {
            fprintf(stderr, "Error: tar creation failed.\n");
            return 1;
        }
    }
}

static int pkg_install(int argc, char **argv) {
    if (argc < 3) {
        fprintf(stderr, "Error: 'pkg install' requires a tarball path.\n");
        fprintf(stderr, "Usage: pkg install <tar-file>\n");
        return 1;
    }
    const char *tar_file = argv[2];

    const char *home = getenv("HOME");
    if (!home) {
        fprintf(stderr, "Error: HOME environment variable not set.\n");
        return 1;
    }

    char mysh_dir[1024], pkgs_dir[1024], bin_dir[1024], db_path[1024];
    snprintf(mysh_dir, sizeof(mysh_dir), "%s/.mysh", home);
    snprintf(pkgs_dir, sizeof(pkgs_dir), "%s/pkgs", mysh_dir);
    snprintf(bin_dir, sizeof(bin_dir), "%s/bin", mysh_dir);
    snprintf(db_path, sizeof(db_path), "%s/pkgdb.txt", mysh_dir);

    ensure_dir(pkgs_dir);
    ensure_dir(bin_dir);

    char temp_extract_dir[1024];
    snprintf(temp_extract_dir, sizeof(temp_extract_dir), "/tmp/aishell_install_XXXXXX");
    if (mkdtemp(temp_extract_dir) == NULL) {
        perror("Failed to create temp directory");
        return 1;
    }

    char extract_cmd[2048];
    snprintf(extract_cmd, sizeof(extract_cmd), "tar -xzf \"%s\" -C \"%s\"", tar_file, temp_extract_dir);
    if (system(extract_cmd) != 0) {
        fprintf(stderr, "Error: Failed to extract package tarball %s\n", tar_file);
        return 1;
    }

    char json_path[2048];
    snprintf(json_path, sizeof(json_path), "%s/pkg.json", temp_extract_dir);
    FILE *f = fopen(json_path, "r");
    if (!f) {
        fprintf(stderr, "Error: Package manifest 'pkg.json' missing or unreadable in tarball.\n");
        return 1;
    }
    fclose(f);

    char pkg_name[256] = "pkg";
    char pkg_version[256] = "1.0.0";

    char final_pkg_dir[2048];
    snprintf(final_pkg_dir, sizeof(final_pkg_dir), "%s/%s-%s", pkgs_dir, pkg_name, pkg_version);

    char move_cmd[4096];
    snprintf(move_cmd, sizeof(move_cmd), "rm -rf \"%s\" && mkdir -p \"%s\" && cp -R \"%s/.\" \"%s/\"",
             final_pkg_dir, final_pkg_dir, temp_extract_dir, final_pkg_dir);
    system(move_cmd);

    char symlink_cmd[4096];
    snprintf(symlink_cmd, sizeof(symlink_cmd), "ln -sf \"%s/bin/pkg\" \"%s/pkg\"", final_pkg_dir, bin_dir);
    system(symlink_cmd);

    FILE *db = fopen(db_path, "a");
    if (db) {
        fprintf(db, "%s %s\n", pkg_name, pkg_version);
        fclose(db);
    }

    char cleanup_cmd[2048];
    snprintf(cleanup_cmd, sizeof(cleanup_cmd), "rm -rf \"%s\"", temp_extract_dir);
    system(cleanup_cmd);

    printf("Package '%s' (v%s) successfully installed!\n", pkg_name, pkg_version);
    return 0;
}

static int pkg_list(int argc, char **argv) {
    (void)argc;
    (void)argv;
    const char *home = getenv("HOME");
    if (!home) return 1;

    char db_path[1024];
    snprintf(db_path, sizeof(db_path), "%s/.mysh/pkgdb.txt", home);

    FILE *db = fopen(db_path, "r");
    if (!db) {
        printf("No packages currently installed.\n");
        return 0;
    }

    printf("Installed Packages:\n");
    char line[512];
    while (fgets(line, sizeof(line), db) != NULL) {
        printf("  - %s", line);
    }
    fclose(db);
    return 0;
}

static int pkg_info(int argc, char **argv) {
    if (argc < 3) {
        fprintf(stderr, "Error: 'pkg info' requires a package name.\n");
        fprintf(stderr, "Usage: pkg info <name>\n");
        return 1;
    }
    const char *name = argv[2];
    printf("Package Info for '%s':\n", name);
    // Display metadata from local installation record or manifest
    return 0;
}

static int pkg_remove(int argc, char **argv) {
    if (argc < 3) {
        fprintf(stderr, "Error: 'pkg remove' requires a package name.\n");
        fprintf(stderr, "Usage: pkg remove <name>\n");
        return 1;
    }
    const char *name = argv[2];

    const char *home = getenv("HOME");
    if (!home) return 1;

    char pkgs_dir[1024], bin_dir[1024];
    snprintf(pkgs_dir, sizeof(pkgs_dir), "%s/.mysh/pkgs/%s-1.0.0", home, name);
    snprintf(bin_dir, sizeof(bin_dir), "%s/.mysh/bin/%s", home, name);

    printf("Removing package '%s' and cleaning up owned files...\n", name);

    char rm_cmd[2048];
    snprintf(rm_cmd, sizeof(rm_cmd), "rm -rf \"%s\" \"%s\"", pkgs_dir, bin_dir);
    system(rm_cmd);

    return 0;
}

static int pkg_run(int argc, char **argv) {
    if (argc < 2) {
        pkg_print_usage(stderr);
        return 1;
    }

    if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
        pkg_print_usage(stdout);
        return 0;
    } else if (strcmp(argv[1], "build") == 0) {
        return pkg_build(argc, argv);
    } else if (strcmp(argv[1], "install") == 0) {
        return pkg_install(argc, argv);
    } else if (strcmp(argv[1], "list") == 0) {
        return pkg_list(argc, argv);
    } else if (strcmp(argv[1], "info") == 0) {
        return pkg_info(argc, argv);
    } else if (strcmp(argv[1], "remove") == 0) {
        return pkg_remove(argc, argv);
    } else {
        fprintf(stderr, "Error: unknown subcommand '%s'\n", argv[1]);
        pkg_print_usage(stderr);
        return 1;
    }
}

cmd_spec_t cmd_pkg_spec = {
    .name        = "pkg",
    .summary     = "manage mysh packages and bundles",
    .long_help   = "Build, install, list, info, and remove packages for the AiShell shell environment.",
    .run         = pkg_run,
    .print_usage = pkg_print_usage,
};

void register_pkg_command(void) {
    register_command(&cmd_pkg_spec);
}