# Directories
ASMDIR = asm
INCDIR = include
LIBDIR = lib
OBJDIR = obj
DEPDIR = deps
SRCDIR = src

# Toggle debug mode
DEBUG = TRUE

# Name of executable
FILENAME = a.out

# Macro flags
MACROFLAGS =

# Linker flags
LDFLAGS = -L$(LIBDIR) -Wl,-rpath=$(LIBDIR)

# Dependency flags
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.d

# Library flags
LIBFLAGS = -fPIC -m64

# Performance/debug flags
ifeq ($(DEBUG), TRUE)
CFLAGS_BUILD = -O0 -g3
else
CFLAGS_BUILD = -O3
endif

# Compilation flags
CFLAGS = -I$(INCDIR) $(CFLAGS_BUILD) $(MACROFLAGS) $(LIBFLAGS)
CFLAGS += -ansi
CFLAGS += -pedantic -pedantic-errors
CFLAGS += -Werror
CFLAGS += -Wall -Wextra
CFLAGS += -Wcast-align
CFLAGS += -Wcast-qual
CFLAGS += -Wfloat-equal
CFLAGS += -Wformat=2
CFLAGS += -Wmissing-include-dirs
CFLAGS += -Wredundant-decls
CFLAGS += -Wsequence-point
CFLAGS += -Wswitch
CFLAGS += -Wundef
CFLAGS += -Wshadow
CFLAGS += -Wpointer-arith
CFLAGS += -Wbad-function-cast
CFLAGS += -Wwrite-strings
CFLAGS += -Waggregate-return
CFLAGS += -Wmissing-declarations
CFLAGS += -Wmissing-field-initializers
CFLAGS += -Wunreachable-code

ifneq ($(DEBUG), TRUE)
# Enable inline warning, which is broken in debug mode due to assertions.
CFLAGS += -Winline
# Disable assertions and other debug features
CFLAGS += -DNDEBUG
endif

# Compiler
CC = gcc

# Source files
SRC = $(shell find $(SRCDIR) -type f -name "*.c")
SRC := $(patsubst $(SRCDIR)/%,%,$(SRC))
ASMSRC = $(shell find $(SRCDIR) -type f -name "*.s")
ASMSRC := $(patsubst $(SRCDIR)/%,%,$(ASMSRC))

# Object files
OBJ = $(patsubst %.c,$(OBJDIR)/%.o,$(SRC))
OBJ += $(patsubst %.s,$(OBJDIR)/%.o,$(ASMSRC))

# Dependency files
DEPS = $(patsubst %.c,$(DEPDIR)/%.d,$(SRC))

# Assembly files
ASM = $(patsubst %.c,$(ASMDIR)/%.s,$(SRC))

# Object directories
$(shell mkdir -p $(dir $(OBJ)))

# Dependency directories
$(shell mkdir -p $(dir $(DEPS)))

# Assembly directories
$(shell mkdir -p $(dir $(ASM)))

# Rules

.PHONY: all
all: $(FILENAME)

# Documentation
documentation:
	doxygen Doxyfile

# Library
library: $(OBJ)
	$(CC) -shared -o $(LIBDIR)/libjkthread.so $^ $(CFLAGS) $(LDFLAGS)

# Executable
$(FILENAME): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

# Assembly
$(ASMDIR)/%.s: $(SRCDIR)/%.c $(DEPDIR)/%.d Makefile
	$(CC) $(DEPFLAGS) -S -o $@ $< $(CFLAGS)

# Objects
$(OBJDIR)/%.o: $(ASMDIR)/%.s $(DEPDIR)/%.d Makefile
	$(CC) $(DEPFLAGS) -c -o $@ $< $(CFLAGS)
$(OBJDIR)/%.o: $(SRCDIR)/%.s Makefile
	$(CC) $(DEPFLAGS) -c -o $@ $< $(CFLAGS)

$(DEPS):
include $(wildcard $(DEPS))

.PHONY: clean
clean:
	find $(OBJDIR) -type f -name '*.o' -exec rm {} +
	find $(DEPDIR) -type f -name '*.d' -exec rm {} +
	find $(ASMDIR) -type f -name '*.s' -exec rm {} +
