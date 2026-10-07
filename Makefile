# ==============================================================================
# AiShell Build System
# ==============================================================================

CC      ?= gcc
CFLAGS := -Wall -Wextra -std=c99 -pedantic -g \
          -Isrc \
          -Isrc/third-party/argtable3
LDFLAGS := -lm

# Directories
BIN_DIR       := bin
SRC_DIR       := src
TP_DIR        := src/third-party/argtable3
BUILD_DIR     := build

# Third-party Dependency (argtable3)
TP_SRC        := $(TP_DIR)/argtable3.c
TP_OBJ        := $(BUILD_DIR)/argtable3.o

# Core Framework Sources
CORE_SRCS     := $(SRC_DIR)/ai_common.c \
                 $(SRC_DIR)/registry/cmd_registry.c
CORE_OBJS     := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(CORE_SRCS))

# Command Module Sources (excluding standalone main wrappers)
CMD_SRCS      := $(SRC_DIR)/commands/aisysinfo/cmd_aisysinfo.c
# Step 2-4 command sources will be appended here as implemented:
# CMD_SRCS    += $(SRC_DIR)/commands/aipwd/cmd_aipwd.c
# CMD_SRCS    += $(SRC_DIR)/commands/aicd/cmd_aicd.c
# CMD_SRCS    += $(SRC_DIR)/commands/ails/cmd_ails.c

CMD_OBJS      := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(CMD_SRCS))

# Standalone Executables
ALL_BINS      := $(BIN_DIR)/aishell \
                 $(BIN_DIR)/aisysinfo
# Step 2-4 binary targets will be appended here as implemented:
# ALL_BINS    += $(BIN_DIR)/aipwd
# ALL_BINS    += $(BIN_DIR)/aicd
# ALL_BINS    += $(BIN_DIR)/ails

# Default Target
.PHONY: all
all: $(ALL_BINS)

# ==============================================================================
# Executable Targets
# ==============================================================================

# Interactive Shell REPL (bin/aishell)
$(BIN_DIR)/aishell: $(BUILD_DIR)/main.o $(CORE_OBJS) $(CMD_OBJS) $(TP_OBJ)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# Standalone: aisysinfo
$(BIN_DIR)/aisysinfo: $(BUILD_DIR)/commands/aisysinfo/aisysinfo_main.o \
                       $(BUILD_DIR)/commands/aisysinfo/cmd_aisysinfo.o \
                       $(CORE_OBJS) $(TP_OBJ)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# Placeholder templates for Step 2-4 standalone binaries:
# $(BIN_DIR)/aipwd: $(BUILD_DIR)/commands/aipwd/aipwd_main.o \
#                    $(BUILD_DIR)/commands/aipwd/cmd_aipwd.o \
#                    $(CORE_OBJS) $(TP_OBJ)
# 	@mkdir -p $(BIN_DIR)
# 	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# ==============================================================================
# Object Compilation Rules
# ==============================================================================

# Compile Third-Party Libraries
$(TP_OBJ): $(TP_SRC)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile Core Framework & Command Modules
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# ==============================================================================
# Utility Rules
# ==============================================================================

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

.PHONY: test
test: all
	@echo "Running Step 1 verification checks..."
	./$(BIN_DIR)/aisysinfo --version
	./$(BIN_DIR)/aisysinfo --json
	./$(BIN_DIR)/aisysinfo -h