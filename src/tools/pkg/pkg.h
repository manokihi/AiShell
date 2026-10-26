//
// Created by Kamil Synek on 10/24/26.
//

#ifndef PKG_H
#define PKG_H

#include "../../registry/registry.h"

#define PKG_VERSION "1.0.0"

/**
 * Command specification for the package manager tool.
 */
extern cmd_spec_t cmd_pkg_spec;

/**
 * Registers the pkg command into the shell command registry.
 */
void register_pkg_command(void);

#endif /* PKG_H */