SHELL := /bin/sh
.SUFFIXES: 
.SUFFIXES: .c .o .h

# compiler
CC = gcc

# flags
CFLAGS = -O3

ifdef DEBUG
	CFLAGS = -ggdb
endif

WARN := -Wall -Wextra
LDLIBS := `pkg-config raylib --libs` -lm

# dirs
SRCDIR := src
CLIENTDIR := $(SRCDIR)/client
COMMONDIR := $(SRCDIR)/common
SERVERDIR := $(SRCDIR)/server

BUILDDIR := build
CLIENTBUILDDIR := $(BUILDDIR)/client
COMMONBUILDDIR := $(BUILDDIR)/common
SERVERBUILDDIR := $(BUILDDIR)/server

INCDIR := include
CLIENTINCDIR := $(INCDIR)/client
COMMONINCDIR := $(INCDIR)/common
SERVERINCDIR := $(INCDIR)/server

# serverdir shouldn't be required by the client. 
# most utilities are server-only and including all of them is a waste.
CLIENT := $(shell find $(CLIENTDIR) -name *.c)
COMMON := $(shell find $(COMMONDIR) -name *.c)
SERVER := $(shell find $(SERVERDIR) -name *.c)


_CLIENTOBJ := $(CLIENT:%.c=%.o)
CLIENTOBJ := $(subst $(CLIENTDIR),$(CLIENTBUILDDIR),$(_CLIENTOBJ))

_COMMONOBJ := $(COMMON:%.c=%.o)
COMMONOBJ := $(subst $(COMMONDIR),$(COMMONBUILDDIR),$(_COMMONOBJ))

_SERVEROBJ := $(SERVER:%.c=%.o)
SERVEROBJ := $(subst $(SERVERDIR),$(SERVERBUILDDIR),$(_SERVEROBJ))


CLIENTDEPS := $(shell find $(CLIENTINCDIR) $(COMMONINCDIR) -name *.h)
COMMONDEPS := $(shell find $(COMMONINCDIR) -name *.h)
SERVERDEPS := $(shell find $(SERVERINCDIR) $(COMMONINCDIR )-name *.h)

CLIENT_OUT_NAME := client
SERVER_OUT_NAME := server

#### object files ####
# client
$(CLIENTBUILDDIR)/%.o : $(CLIENTDIR)/%.c $(CLIENTDEPS) $(COMMONDEPS) | $(CLIENTBUILDDIR)
	$(CC) -c $< -I$(CLIENTINCDIR) -I$(COMMONINCDIR) $(CFLAGS) $(WARN) $(LDLIBS) -o $@ 

# common
$(COMMONBUILDDIR)/%.o : $(COMMONDIR)/%.c $(COMMONDEPS) | $(COMMONBUILDDIR)
	$(CC) -c $< -I$(COMMONINCDIR) $(CFLAGS) $(WARN) $(LDLIBS) -o $@ 

# server
$(SERVERBUILDDIR)/%.o : $(SERVERDIR)/%.c $(SERVERDEPS) $(COMMONDEPS) | $(SERVERBUILDDIR)
	$(CC) -c $< -I$(SERVERINCDIR) -I$(COMMONINCDIR) $(CFLAGS) $(WARN) $(LDLIBS) -o $@ 


#### executables ####
# client
client : $(CLIENTOBJ) $(COMMONOBJ)
	$(CC) $^ -I$(INCDIR) $(CFLAGS) $(WARN) $(LDLIBS) -o $(CLIENTBUILDDIR)/$(CLIENT_OUT_NAME)

# server
server : $(SERVEROBJ) $(COMMONOBJ)
	$(CC) $^ -I$(INCDIR) $(CFLAGS) $(WARN) $(LDLIBS) -o $(SERVERBUILDDIR)/$(SERVER_OUT_NAME)

# builddir
.PHONY: $(CLIENTBUILDDIR)
.PHONY: $(COMMONBUILDDIR)
.PHONY: $(SERVERBUILDDIR)

$(CLIENTBUILDDIR) :
	mkdir -p $@

$(COMMONBUILDDIR) :
	mkdir -p $@

$(SERVERBUILDDIR) :
	mkdir -p $@
	
# clean
.PHONY: clean
clean:
	rm -r $(BUILDDIR)
