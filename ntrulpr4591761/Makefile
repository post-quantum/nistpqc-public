CC = gcc
AR = ar
RANLIB = ranlib
ARFLAGS = cr
APPENDPREFIX = ../scripts/append_prefix.sh
DEFINE = 
INCLUDE = -I. -I../common
CFLAGS = -O3 -Wall -fPIC -fomit-frame-pointer -march=native $(DEFINE) $(INCLUDE)

CIPHER = ntrulpr4591761

TARGETLIB = lib$(CIPHER).a

OBJDIR = .obj
SOURCES = $(wildcard *.c) ../scripts/aux_api.c
OBJECTS = $(patsubst %.c,$(OBJDIR)/%.o,$(notdir $(SOURCES)))

all : $(TARGETLIB)

$(TARGETLIB) : $(OBJECTS)
	$(AR) $(ARFLAGS) $@ $^
	$(RANLIB) $@
	bash $(APPENDPREFIX) $(CIPHER)_ $@

$(OBJDIR)/%.o : %.c | makedir 
	$(CC) $(CFLAGS) $(INCLUDE) -c -o $@ $<

$(OBJDIR)/aux_api.o : ../scripts/aux_api.c | makedir
	$(CC) $(CFLAGS) $(INCLUDE) -c -o $@ $<

makedir :
	@mkdir -p $(OBJDIR)

clean:
	@rm -rf $(OBJDIR) $(TARGETLIB) 

