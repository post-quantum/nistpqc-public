CC = gcc
CFLAGS = -O3 -Wall -fPIC -fomit-frame-pointer
INCLUDE = -I../ -I../common

OBJDIR = .obj
TARGETEXE = nistpqc_test
LIBRARY = ../libnistpqc.a -lcrypto

all : test

test : $(patsubst %.c,$(OBJDIR)/%.o,$(wildcard *.c)) $(OBJDIR)/rng.o
	$(CC) -o $(TARGETEXE) $^ $(LIBRARY)

$(OBJDIR)/%.o : %.c makedir
	$(CC) $(CFLAGS) $(INCLUDE) -c -o $@ $<

$(OBJDIR)/rng.o : ../common/rng.c makedir
	$(CC) $(CFLAGS) $(INCLUDE) -c -o $@ $<

makedir :
	@mkdir -p $(OBJDIR)

clean:
	@rm -rf $(OBJDIR) $(TARGETEXE) 
