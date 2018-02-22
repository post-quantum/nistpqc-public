CC = gcc
AR = ar
RANLIB = ranlib
LIBTOOL = libtool
ARFLAGS = cr
UPDATE_LIB = ../scripts/update_library.sh
DEFINE = -D _OPTIMIZED_GENERIC_ -D _AMD64_ -D __LINUX__
INCLUDE = -I. -I../common
CFLAGS = -O3 -Wall -fPIC -fomit-frame-pointer $(DEFINE) $(INCLUDE)

CIPHER = sikep503
UNAME := $(shell uname -s)

TARGETLIB = lib$(CIPHER).a

OBJDIR = .obj
SOURCES = P503.c generic/fp_generic.c sha3/fips202.c ../scripts/aux_api.c
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

$(OBJDIR)/P503.o : P503.c | makedir 
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJDIR)/fp_generic.o : generic/fp_generic.c | makedir 
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJDIR)/fips202.o : sha3/fips202.c | makedir 
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJDIR)/aux_api.o : ../scripts/aux_api.c
	$(CC) $(CFLAGS) -c -o $@ $<

makedir :
	@mkdir -p $(OBJDIR)

clean:
	@rm -rf $(OBJDIR) $(TARGETLIB) 

