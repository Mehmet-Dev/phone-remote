# Compiler and Flags
CC      := gcc
CFLAGS  := -Wall -Wextra -g -Iinc -lpthread
# -Wall -Wextra: Show all warnings (crucial for C)
# -g: Adds debug symbols for Valgrind/GDB
# -Iinc: Tells the compiler to look in /inc for your .h files

# Project Folders
SRCDIR  := src
OBJDIR  := obj
BIN     := remote_listener_app

# File Discovery
SRCS    := $(wildcard $(SRCDIR)/*.c)
OBJS    := $(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

# Default Rule (Type 'make' to run this)
all: $(BIN)

# Link the executable
$(BIN): $(OBJS)
	$(CC) $(OBJS) -o $@
	@echo "Build complete: ./${BIN}"

# Compile .c files into .o files
$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Create the obj directory if it's missing
$(OBJDIR):
	mkdir -p $(OBJDIR)

# Clean up build files
clean:
	rm -rf $(OBJDIR) $(BIN)
	@echo "Cleaned up."

.PHONY: all clean