CC = gcc
AR = ar
LIBTOOL = libtool
INSTALL = install
CFLAGS = -O3 -Wall -fPIC -fomit-frame-pointer
ARFLAGS = cr
LDFLAGS =
OPENSSLDIR =
VERSION = 0.1

ifeq ($(PREFIX),)
	PREFIX := /usr/local
endif

UNAME := $(shell uname -s)

LIBNAME = nistpqc
ifeq ($(UNAME),Darwin)
OPENSSLDIR = /usr/local/opt/openssl
OPENSSLLIBDIR = $(OPENSSLDIR)/lib
LDFLAGS += -dynamiclib -Wl,-undefined,dynamic_lookup 
LDFLAGS += -current_version $(VERSION) -compatibility_version $(VERSION)
SHAREDLIBRARY = lib$(LIBNAME).A.dylib
else ifeq ($(UNAME),Linux)
LDFLAGS += -shared
SHAREDLIBRARY = lib$(LIBNAME).so.$(VERSION)
SONAME = lib$(LIBNAME).so.$(shell echo $(VERSION) | cut -f1 -d'.')
else
	@echo "Unsupported platform $(UNAME)"
	@exit -1
endif
STATICLIBRARY = lib$(LIBNAME).a

DIRS = newhope512cca kyber512 ntrulpr4591761 ntrukem443 sikep503 ledakem128sln02

OBJDIR = .obj
ARCHIVES = $(foreach dir,$(DIRS),$(dir)/lib$(dir).a) 
OBJECTS = $(patsubst %.c,$(OBJDIR)/%.o,$(wildcard *.c)) $(OBJDIR)/rng.o

BUILDDIRS = $(DIRS:%=build-%)
TESTDIRS  = $(DIRS:%=test-%)
CLEANDIRS = $(DIRS:%=clean-%)

all : $(BUILDDIRS) $(SHAREDLIBRARY) $(STATICLIBRARY)

$(DIRS) : $(BUILDDIRS)

$(BUILDDIRS) :
	$(MAKE) -C $(@:build-%=%)

$(SHAREDLIBRARY) : $(ARCHIVES) $(OBJECTS)
ifeq ($(UNAME),Darwin)
	$(CC) $(LDFLAGS) -o $@ $(filter %.a,$^) $(filter %.o,$^) -L$(OPENSSLLIBDIR) -lcrypto
else ifeq ($(UNAME),Linux)
	$(CC) $(LDFLAGS) -Wl,-soname,$(SONAME) -o $@ -Wl,--whole-archive $(filter %.a,$^) -Wl,--no-whole-archive $(filter %.o,$^) -lcrypto
else
	@echo "Unsupported platform $(UNAME)"
	@exit -1
endif

make-archive = "create $(1)\n $(foreach lib,$(2),addlib $(lib)\n) $(foreach obj,$(3),addmod $(obj)\n) save\n end\n"

$(STATICLIBRARY) : $(ARCHIVES) $(OBJECTS)
ifeq ($(UNAME),Darwin)
	$(LIBTOOL) -static -o $@ $(filter %.a,$^) $(filter %.o,$^)
else ifeq ($(UNAME),Linux)
	echo $(call make-archive,$@,$(filter %.a,$^),$(filter %.o,$^)) | $(AR) -M
else
	@echo "Unsupported platform $(UNAME)"
	@exit -1
endif

$(OBJDIR)/%.o : %.c | makedir
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJDIR)/rng.o : common/rng.c | makedir
	$(CC) $(CFLAGS) -c -o $@ $<

install: $(STATICLIBRARY) $(SHAREDLIBRARY) 
	@echo "Installing static library $(STATICLIBRARY)"
	@$(INSTALL) -d $(PREFIX)/lib
	@$(INSTALL) -m 644 $(STATICLIBRARY) $(PREFIX)/lib/
	@echo "Installing shared library $(SHAREDLIBRARY)"	
	@$(INSTALL) -d $(PREFIX)/lib
	@$(INSTALL) -m 644 $(SHAREDLIBRARY) $(PREFIX)/lib/
ifeq ($(UNAME),Darwin)
	@ln -sf $(PREFIX)/lib/$(SHAREDLIBRARY) $(PREFIX)/lib/lib$(LIBNAME).dylib
else ifeq ($(UNAME),Linux)
	@ln -sf $(PREFIX)/lib/$(SHAREDLIBRARY) $(PREFIX)/lib/$(SONAME)
	@ln -sf $(PREFIX)/lib/$(SHAREDLIBRARY) $(PREFIX)/lib/lib$(LIBNAME).so
else
	@echo "Unsupported platform $(UNAME)"
	@exit -1
endif
	@echo "Installing header files"
	@$(INSTALL) -d $(PREFIX)/include/nistpqc
	@$(INSTALL) -m 644 nistpqc_api.h $(PREFIX)/include/nistpqc/api.h
ifeq ($(UNAME),Linux)
	@ldconfig $(PREFIX)
endif

uninstall:
	@echo "Uninstalling libraries and header files"
	@rm -f $(PREFIX)/include/nistpqc_api.h
	@rm -f $(PREFIX)/lib/$(STATICLIBRARY)
	@rm -f $(PREFIX)/lib/$(SHAREDLIBRARY)
ifeq ($(UNAME),Darwin)
	@rm -f $(PREFIX)/lib/lib$(LIBNAME).dylib
else ifeq ($(UNAME),Linux)
	@rm -f $(PREFIX)/lib/$(SONAME)
	@rm -f $(PREFIX)/lib/lib$(LIBNAME).so
endif

test : $(TESTDIRS) all
	$(MAKE) -C $(@:test-%=%) test

makedir :
	@mkdir -p $(OBJDIR)

clean : $(CLEANDIRS) clean-library clean-dir 

$(CLEANDIRS) :
	$(MAKE) -C $(@:clean-%=%) clean
	$(MAKE) -C test clean

$(TESTCLEANDIRS) : $(TESTDIRS)
	$(MAKE) -C $(@:test-%=%) clean

clean-library :
	@rm -f $(SHAREDLIBRARY) $(STATICLIBRARY)

clean-dir :
	@rm -rf $(OBJDIR)

.PHONY : subdirs $(DIRS)
.PHONY : subdirs $(BUILDDIRS)
.PHONY : subdirs $(TESTDIRS)
.PHONY : subdirs $(CLEANDIRS)
.PHONY : all test install uninstall clean

