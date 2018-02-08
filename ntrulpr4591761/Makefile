CC = gcc
AR = ar
RANLIB = ranlib
ARFLAGS = cr
APPENDPREFIX = ../scripts/append_prefix.sh
CFLAGS = -O3 -Wall -fPIC -fomit-frame-pointer
INCLUDE = -I. -I../common

CIPHER = ntrulpr4591761

TARGETLIB = lib$(CIPHER).a

OBJDIR = .obj

all : prerequisites $(TARGETLIB)

prerequisites :
	@cp -f ../scripts/aux_api.h .
	@cp -f ../scripts/aux_api.c .

$(TARGETLIB) : $(patsubst %.c,$(OBJDIR)/%.o,$(wildcard *.c)) $(OBJDIR)/aux_api.o
	$(AR) $(ARFLAGS) $@ $^
	$(RANLIB) $@
	bash $(APPENDPREFIX) $(CIPHER)_ $@

$(OBJDIR)/%.o : %.c makedir
	$(CC) $(CFLAGS) $(INCLUDE) -c -o $@ $<

$(OBJDIR)/aux_api.o : aux_api.c
	$(CC) $(CFLAGS) $(INCLUDE) -c -o $@ $<

makedir :
	@mkdir -p $(OBJDIR)

clean:
	@rm -rf $(OBJDIR) $(TARGETLIB) aux_api.*
