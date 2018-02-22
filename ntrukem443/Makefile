CC = gcc
AR = ar
RANLIB = ranlib
LIBTOOL = libtool
ARFLAGS = cr
UPDATE_LIB = ../scripts/update_library.sh
DEFINE = 
INCLUDE = -I. -I../common
CFLAGS = -O3 -Wall -fPIC -fomit-frame-pointer $(DEFINE) $(INCLUDE)

UNAME := $(shell uname -s)
CIPHER = ntrukem443

TARGETLIB = lib$(CIPHER).a

OBJDIR = .obj
SOURCES = $(wildcard *.c) ../scripts/aux_api.c
OBJECTS = $(patsubst %.c,$(OBJDIR)/%.o,$(notdir $(SOURCES)))

all : $(TARGETLIB)

$(TARGETLIB) : $(OBJECTS)
ifeq ($(UNAME),Linux)
	$(AR) $(ARFLAGS) $@ $^
	$(RANLIB) $@
else ifeq ($(UNAME),Darwin)
	$(LIBTOOL) -static -o $@ $^
else
	@echo "Unsupported platform $(UNAME)"
endif
	bash $(UPDATE_LIB) $(CIPHER) $@

$(OBJDIR)/%.o : %.c | makedir 
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJDIR)/aux_api.o : ../scripts/aux_api.c | makedir
	$(CC) $(CFLAGS) -c -o $@ $<

makedir :
	@mkdir -p $(OBJDIR)

clean:
	@rm -rf $(OBJDIR) $(TARGETLIB) 

