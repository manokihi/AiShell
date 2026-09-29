CC ?= gcc
CFLAGS ?= -Wall -Wextra -Werror -std=c11 -pedantic -Isrc
TARGET = aishell

SRCS = src/main.c src/tools/sysinfo.c
OBJS = $(SRCS:.c=.o)

.PHONY: all clean test

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

test: $(TARGET)
	@chmod +x scripts/test.sh
	@./scripts/test.sh