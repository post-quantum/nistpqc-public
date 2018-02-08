CC = gcc
AR = ar
RANLIB = ranlib
ARFLAGS = cr
APPENDPREFIX = ../scripts/append_prefix.sh
DEFINE = -D _OPTIMIZED_GENERIC_ -D _AMD64_ -D __LINUX__
INCLUDE = -I. -I../common
CFLAGS = -O3 -Wall -fPIC -fomit-frame-pointer -march=native $(DEFINE) $(INCLUDE)

CIPHER = sikep503

TARGETLIB = lib$(CIPHER).a

OBJDIR = .obj
SOURCES = P503.c generic/fp_generic.c sha3/fips202.c ../scripts/aux_api.c
OBJECTS = $(patsubst %.c,$(OBJDIR)/%.o,$(notdir $(SOURCES)))

all : $(TARGETLIB)

$(TARGETLIB) : $(OBJECTS)
	$(AR) $(ARFLAGS) $@ $^
	$(RANLIB) $@
	bash $(APPENDPREFIX) $(CIPHER)_ $@

$(OBJDIR)/P503.o : P503.c | makedir 
	$(CC) $(CFLAGS) $(INCLUDE) -c -o $@ $<

$(OBJDIR)/fp_generic.o : generic/fp_generic.c | makedir 
	$(CC) $(CFLAGS) $(INCLUDE) -c -o $@ $<

$(OBJDIR)/fips202.o : sha3/fips202.c | makedir 
	$(CC) $(CFLAGS) $(INCLUDE) -c -o $@ $<

$(OBJDIR)/aux_api.o : ../scripts/aux_api.c
	$(CC) $(CFLAGS) $(INCLUDE) -c -o $@ $<

makedir :
	@mkdir -p $(OBJDIR)

clean:
	@rm -rf $(OBJDIR) $(TARGETLIB) 

