# ==============================================================================
# AiShell Build System
# ==============================================================================

CC ?= gcc
CFLAGS ?= -Wall -Wextra -Werror -std=c11 -pedantic \
 		  -g \
 		  -Isrc -Isrc/registry -Isrc/tools/pkg

PREFIX ?= $(HOME)
TARGET = aishell
BUILD_DIR = build
BIN_DIR = $(BUILD_DIR)/bin
LIB_DIR = $(BUILD_DIR)/lib
OBJ_DIR = $(BUILD_DIR)/obj

all: directories $(BIN_DIR)/$(TARGET) $(LIB_DIR)/libcore.a $(LIB_DIR)/libpkg.so $(BIN_DIR)/pkg dist_tarballs

directories:
	mkdir -p $(BIN_DIR) $(LIB_DIR) $(OBJ_DIR)/src/registry $(OBJ_DIR)/src/tools/pkg

# 1. Build core registry object and static library
$(OBJ_DIR)/src/registry/registry.o: src/registry/registry.c src/registry/registry.h | directories
	$(CC) $(CFLAGS) -c src/registry/registry.c -o $@

$(LIB_DIR)/libcore.a: $(OBJ_DIR)/src/registry/registry.o | directories
	ar rcs $@ $<

# 2. Build main shell binary in build/bin/
$(BIN_DIR)/$(TARGET): src/main.c $(LIB_DIR)/libcore.a | directories
	$(CC) $(CFLAGS) src/main.c -L$(LIB_DIR) -lcore -o $@ -ldl

# 3. Build tool shared object (.so) in build/lib/
$(LIB_DIR)/libpkg.so: src/tools/pkg/pkg.c src/tools/pkg/pkg.h $(LIB_DIR)/libcore.a | directories
	$(CC) $(CFLAGS) -fPIC -shared src/tools/pkg/pkg.c -L$(LIB_DIR) -lcore -o $@

# 4. Build standalone tool binary in build/bin/
$(BIN_DIR)/pkg: src/tools/pkg/pkg.c src/tools/pkg/pkg.h $(LIB_DIR)/libcore.a | directories
	$(CC) $(CFLAGS) -DSTANDALONE_PKG_MAIN src/tools/pkg/pkg.c -L$(LIB_DIR) -lcore -o $@

# 5. Build distribution tarball bundle (pkg.tar.gz)
dist_tarballs: $(BIN_DIR)/pkg $(LIB_DIR)/libpkg.so
	mkdir -p dist/pkgs
	mkdir -p build_pkg/bin build_pkg/lib build_pkg/docs
	cp $(BIN_DIR)/pkg build_pkg/bin/pkg
	cp $(LIB_DIR)/libpkg.so build_pkg/lib/libpkg.so

	# Create pkg.json manifest
	echo '{\
	  "name": "pkg",\
	  "version": "1.0.0",\
	  "description": "AiShell Package Manager",\
	  "commands": [{"name": "pkg", "tool": "pkg", "scope": "internal_external"}],\
	  "files": ["bin/pkg", "lib/libpkg.so"],\
	  "docs": [],\
	  "dependencies": []\
	}' > build_pkg/pkg.json

	# Create pkg.tar.gz in dist/pkgs/
	tar -czf dist/pkgs/pkg.tar.gz -C build_pkg .
	rm -rf build_pkg

# 6. Simulate system-wide installation and user environment setup
install: all
	@echo "Installing aishell to $(PREFIX)/bin..."
	mkdir -p $(PREFIX)/bin
	cp $(BIN_DIR)/$(TARGET) $(PREFIX)/bin/aishell

	@echo "Setting up user runtime environment in ~/.mysh..."
	mkdir -p ~/.mysh/installed
	mkdir -p ~/.mysh/dist

	@echo "Copying distribution package to ~/.mysh/dist/..."
	cp dist/pkgs/pkg.tar.gz ~/.mysh/dist/pkg.tar.gz

	@echo "Installation complete! Run 'aishell' from anywhere."

clean:
	rm -rf $(BUILD_DIR) dist build_pkg $(TARGET)

.PHONY: all directories dist_tarballs install clean