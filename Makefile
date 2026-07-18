# Compiler & flags
CC         = gcc
CFLAGS     = -O3 -MMD -MP -Wall -Wextra -Wno-parentheses -pedantic
LDFLAGS    =

# Directories
SDIR       = src
ODIR       = build

# Each subdirectory of src/ is its own program (src/<name> -> build/<name>/<name>.exe)
PROGRAMS   = server client

.DEFAULT_GOAL := all
.PHONY: all clean

# Per-program build rules.
# $(1) = program name (also its src/ subdir and build/ subdir)
define PROGRAM_template
$(1)_SRCS := $$(wildcard $(SDIR)/$(1)/*.c)
$(1)_OBJS := $$(patsubst $(SDIR)/%.c,$(ODIR)/%.o,$$($(1)_SRCS))
$(1)_EXE  := $(ODIR)/$(1)/$(1).exe

ALL_EXES  += $$($(1)_EXE)
ALL_OBJS  += $$($(1)_OBJS)

$$($(1)_EXE): $$($(1)_OBJS)
	@echo "[LD] $$^ --> $$@"
	@$(CC) $$^ -o $$@ $(LDFLAGS)
endef

$(foreach p,$(PROGRAMS),$(eval $(call PROGRAM_template,$(p))))

# Build all executables
all: $(ALL_EXES)

# Compile src/<name>/*.c -> build/<name>/*.o
$(ODIR)/%.o: $(SDIR)/%.c
	@mkdir -p $(dir $@)
	@echo "[CC] $< --> $@"
	@$(CC) $(CFLAGS) -c $< -o $@

# Clean build directory
clean:
	@echo Cleaning up...
	@rm -rf $(ODIR)

# Include dependency files for faster incremental builds
-include $(ALL_OBJS:.o=.d)
