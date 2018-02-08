CC = gcc
AR = ar
RANLIB = ranlib
ARFLAGS = cr
APPENDPREFIX = ../scripts/append_prefix.sh
CFLAGS = -O3 -Wall -fPIC -fomit-frame-pointer -D _OPTIMIZED_GENERIC_ -D _AMD64_ -D __LINUX__
INCLUDE = -I. -I../common

CIPHER = sikep503

TARGETLIB = lib$(CIPHER).a

OBJDIR = .obj

all : prerequisites $(TARGETLIB)

prerequisites :
	@cp -f ../scripts/aux_api.h .
	@cp -f ../scripts/aux_api.c .

$(TARGETLIB) : $(OBJDIR)/P503.o $(OBJDIR)/fp_generic.o $(OBJDIR)/fips202.o $(OBJDIR)/aux_api.o
	$(AR) $(ARFLAGS) $@ $^
	$(RANLIB) $@
	bash $(APPENDPREFIX) $(CIPHER)_ $@

$(OBJDIR)/P503.o : P503.c makedir
	$(CC) $(CFLAGS) $(INCLUDE) -c -o $@ $<

$(OBJDIR)/fp_generic.o : generic/fp_generic.c 
	$(CC) $(CFLAGS) $(INCLUDE) -c -o $@ $<

$(OBJDIR)/fips202.o : sha3/fips202.c 
	$(CC) $(CFLAGS) $(INCLUDE) -c -o $@ $<

$(OBJDIR)/aux_api.o : aux_api.c
	$(CC) $(CFLAGS) $(INCLUDE) -c -o $@ $<

makedir :
	@mkdir -p $(OBJDIR)

clean:
	@rm -rf $(OBJDIR) $(TARGETLIB) aux_api.*
