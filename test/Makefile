CC = gcc
CFLAGS = -O3 -Wall -fPIC -fomit-frame-pointer
INCLUDE = -I../ -I../common
OPENSSLDIR =

OBJDIR = .obj
TARGETEXE = nistpqc_test

UNAME := $(shell uname -s)
ifeq ($(UNAME),Darwin)
OPENSSLDIR = /usr/local/opt/openssl
OPENSSLLIBDIR = $(OPENSSLDIR)/lib
LIBRARY = ../libnistpqc.a -L$(OPENSSLLIBDIR) -lcrypto
else
LIBRARY = ../libnistpqc.a -lcrypto
endif

all : test

test : $(patsubst %.c,$(OBJDIR)/%.o,$(wildcard *.c))
	$(CC) -o $(TARGETEXE) $^ $(LIBRARY)

$(OBJDIR)/%.o : %.c makedir
	$(CC) $(CFLAGS) $(INCLUDE) -c -o $@ $<

makedir :
	@mkdir -p $(OBJDIR)

clean:
	@rm -rf $(OBJDIR) $(TARGETEXE) 
