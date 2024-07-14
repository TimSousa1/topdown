SHELL := /bin/sh
.SUFFIXES: 
.SUFFIXES: .c .o .h

CC = gcc
CFLAGS = -O3

ifdef DEBUG
	CFLAGS = -g
endif

WARN := -Wall -Wextra
LDLIBS := `pkg-config raylib --libs` -lm

SRCDIR := src
BUILDDIR := build

SRC := $(shell find $(SRCDIR) -name *.c)

_OBJ := $(SRC:%.c=%.o)
OBJ := $(subst $(SRCDIR),$(BUILDDIR),$(_OBJ))

DEPS := $(shell find src -name *.h)
OUT_NAME := topdown


$(BUILDDIR)/%.o : $(SRCDIR)/%.c $(DEPS) | $(BUILDDIR)
	$(CC) -c $< $(CFLAGS) $(WARN) $(LDLIBS) -o $@ 


$(BUILDDIR)/$(OUT_NAME) : $(OBJ)
	$(CC) $^ $(CFLAGS) $(WARN) $(LDLIBS) -o $@


.PHONY: $(BUILDDIR)

$(BUILDDIR) :
	mkdir -p $@
	
.PHONY: clean
clean:
	rm -r $(BUILDDIR)
