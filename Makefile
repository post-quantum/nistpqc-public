
# 'make' to build the native static library, shared library
# 'make native' to build just the static and shared libs
# 'make test' to build the test suite
# 'make android' to build static library for Android apps
# 'make ios' to build static library for iOS apps
# 'make mac' to build static library for Mac apps
# 'sudo make install' to install everything you built.


default: native

LIBNAME = nistpqc
VERSION = 0.3
ifeq ($(PREFIX),)
  PREFIX := /usr/local
endif
DIRS = $(notdir $(wildcard crypto/*))
export UNAME=$(shell uname -s)
OBJDIR = $(BUILDDIR)/.obj
STATICLIB = $(BUILDDIR)/lib$(LIBNAME).a
ARCHIVES = $(foreach dir,$(DIRS),$(BUILDDIR)/lib$(dir).a)
OBJECTS = $(patsubst %.c,$(OBJDIR)/%.o,$(wildcard *.c)) $(OBJDIR)/rng.o
CFLAGS += -O3 -Wall -fPIC -fomit-frame-pointer -Icommon

# Some cipher-specific options
frodokem640_FLAGS  = -std=gnu11 -DNIX -D_AMD64_ -D_OPTIMIZED_GENERIC_ -D_AES128_FOR_A_ -DNO_OPENSSL
frodokem976_FLAGS  = -std=gnu11 -DNIX -D_AMD64_ -D_OPTIMIZED_GENERIC_ -D_AES128_FOR_A_ -DNO_OPENSSL
frodokem1344_FLAGS = -std=gnu11 -DNIX -D_AMD64_ -D_OPTIMIZED_GENERIC_ -D_AES128_FOR_A_ -DNO_OPENSSL
saber_FLAGS = -DSABER_TYPE=Saber -Wno-format
firesaber_FLAGS = -DSABER_TYPE=FireSaber -Wno-format
lightsaber_FLAGS = -DSABER_TYPE=LightSaber -Wno-format
sikep434_FLAGS = -D _AMD64_ -D _GENERIC_ -D __LINUX__ -Wno-missing-braces
sikep503_FLAGS = -D _AMD64_ -D _GENERIC_ -D __LINUX__ -Wno-missing-braces
sikep610_FLAGS = -D _AMD64_ -D _GENERIC_ -D __LINUX__ -Wno-missing-braces
sikep751_FLAGS = -D _AMD64_ -D _GENERIC_ -D __LINUX__ -Wno-missing-braces

# If building for Android then we use a special 'standalone' toolchain rather than the system one for
# native builds. The Android NDK will build this toolchain for us
ifeq ($(MAKECMDGOALS),android)
ifndef ANDROID_SDK
$(error Please set ANDROID_SDK to point to your Android SDK)
endif
ifndef OPENSSL
$(error Please set OPENSSL to point to your BoringSSL or OpenSSL source as per https://wiki.strongswan.org/projects/strongswan/wiki/AndroidVPNClientBuild#The-openssl-Directory)
endif
BUILDDIR:=build/android
TOOLCHAIN:=$(BUILDDIR)/toolchain
target_host=$(TOOLCHAIN)/bin/aarch64-linux-android
export AR=$(target_host)-ar
export AS=$(target_host)-clang
export CC=$(target_host)-clang
export CXX=$(target_host)-clang++
export LD=$(target_host)-ld
export NM=$(target_host)-nm
export OBJCOPY=$(target_host)-objcopy
export RANLIB=$(target_host)-ranlib
export CFLAGS:=$(CFLAGS) -fPIE -fPIC -I$(OPENSSL)/include
export LDFLAGS:=$(LDFLAGS) -pie
export UNAME=Linux

$(TOOLCHAIN):
	$(ANDROID_SDK)/ndk-bundle/build/tools/make_standalone_toolchain.py --arch arm64 --api 26 --install-dir=$(TOOLCHAIN)

# Mac
else ifeq ($(findstring mac,$(MAKECMDGOALS)),mac)
BUILDDIR:=build/mac
XCODE_SDK:=macosx
MACOS_MIN_SDK_VERSION=10.15
TOOLCHAIN:=$(shell xcode-select -p)/Toolchains/XcodeDefault.xctoolchain/usr/bin
export AR=$(TOOLCHAIN)/ar
export AS=$(TOOLCHAIN)/as
export CC=$(TOOLCHAIN)/clang
export CXX=$(TOOLCHAIN)/clang++
export LD=$(TOOLCHAIN)/ld
export NM=$(TOOLCHAIN)/nm
export OBJCOPY=objcopy
export RANLIB=$(TOOLCHAIN)/ranlib
export LIBTOOL=$(TOOLCHAIN)/libtool

ifndef IOS_MIN_SDK_VERSION
IOS_MIN_SDK_VERSION=13.0
endif

ifeq ($(MAKECMDGOALS),macos64_x86_64)
export ARCHTYPE=osx
export ARCH=x86_64
CFLAGS+= -mmacosx-version-min=$(MACOS_MIN_SDK_VERSION)
else ifeq ($(MAKECMDGOALS),macos64_arm64)
export ARCHTYPE=osx
export ARCH=arm64
CFLAGS+= -mmacosx-version-min=$(MACOS_MIN_SDK_VERSION)
else ifeq ($(MAKECMDGOALS),mac_catalyst_x86_64)
export ARCHTYPE=catalyst
export ARCH=x86_64
CFLAGS+= -target arm64-apple-ios13.0-macabi -mios-version-min=13.0
else ifeq ($(MAKECMDGOALS),mac_catalyst_arm64)
export ARCHTYPE=catalyst
export ARCH=arm64
CFLAGS+= -target arm64-apple-ios13.0-macabi -mios-version-min=13.0
endif
CFLAGS+= -std=gnu99 -arch $(ARCH) -isysroot $(shell xcrun --sdk $(XCODE_SDK) --show-sdk-path) -fembed-bitcode
BUILDDIR:=$(BUILDDIR)/$(ARCHTYPE)/$(ARCH)

# iOS
else ifeq ($(findstring ios,$(MAKECMDGOALS)),ios)
ifndef IOS_MIN_SDK_VERSION
IOS_MIN_SDK_VERSION=12.0
endif
BUILDDIR:=build/ios
TOOLCHAIN:=$(shell xcode-select -p)/Toolchains/XcodeDefault.xctoolchain/usr/bin
export AR=$(TOOLCHAIN)/ar
export AS=$(TOOLCHAIN)/as
export CC=$(TOOLCHAIN)/clang
export CXX=$(TOOLCHAIN)/clang++
export LD=$(TOOLCHAIN)/ld
export NM=$(TOOLCHAIN)/nm
export OBJCOPY=objcopy
export RANLIB=$(TOOLCHAIN)/ranlib
export LIBTOOL=$(TOOLCHAIN)/libtool

ifeq ($(MAKECMDGOALS),ios_sim_cross_x86_64)
export ARCH=x86_64
XCODE_SDK:=iphonesimulator
CFLAGS+= -miphoneos-version-min=$(IOS_MIN_SDK_VERSION)
else ifeq ($(MAKECMDGOALS),ios_sim_cross_arm64)
export ARCH=arm64
XCODE_SDK:=iphonesimulator
CFLAGS+= -mios-version-min=$(IOS_MIN_SDK_VERSION)
CFLAGS+= -target arm64-apple-ios13.0-macabi -mios-version-min=13.0
else ifeq ($(MAKECMDGOALS),ios64_cross_arm64)
export ARCH=arm64
XCODE_SDK:=iphoneos
CFLAGS+= -miphoneos-version-min=$(IOS_MIN_SDK_VERSION)
else ifeq ($(MAKECMDGOALS),ios64_cross_arm64e)
export ARCH=arm64e
XCODE_SDK:=iphoneos
CFLAGS+= -miphoneos-version-min=$(IOS_MIN_SDK_VERSION)
endif
CFLAGS+= -std=gnu99 -arch $(ARCH) -isysroot $(shell xcrun --sdk $(XCODE_SDK) --show-sdk-path) -fembed-bitcode
BUILDDIR:=$(BUILDDIR)/$(XCODE_SDK)/$(ARCH)

# WatchOS
else ifeq ($(findstring watchos,$(MAKECMDGOALS)),watchos)
ifndef WATCHOS_MIN_SDK_VERSION
WATCHOS_MIN_SDK_VERSION=4.0
endif
BUILDDIR:=build/watchos
TOOLCHAIN:=$(shell xcode-select -p)/Toolchains/XcodeDefault.xctoolchain/usr/bin
export AR=$(TOOLCHAIN)/ar
export AS=$(TOOLCHAIN)/as
export CC=$(TOOLCHAIN)/clang
export CXX=$(TOOLCHAIN)/clang++
export LD=$(TOOLCHAIN)/ld
export NM=$(TOOLCHAIN)/nm
export OBJCOPY=objcopy
export RANLIB=$(TOOLCHAIN)/ranlib
export LIBTOOL=$(TOOLCHAIN)/libtool

ifeq ($(MAKECMDGOALS),watchos_sim_cross_x86_64)
export ARCH=x86_64
XCODE_SDK:=watchsimulator
else ifeq ($(MAKECMDGOALS),watchos_sim_cross_i386)
export ARCH=i386
XCODE_SDK:=watchsimulator
else ifeq ($(MAKECMDGOALS),watchos_cross_arm64_32)
export ARCH=arm64_32
XCODE_SDK:=watchos
else ifeq ($(MAKECMDGOALS),watchos_cross_armv7k)
export ARCH=armv7k
XCODE_SDK:=watchos
endif
CFLAGS+= -std=gnu99 -arch $(ARCH) -isysroot $(shell xcrun --sdk $(XCODE_SDK) --show-sdk-path) -fembed-bitcode
CFLAGS+= -mwatchos-version-min=$(WATCHOS_MIN_SDK_VERSION)
BUILDDIR:=$(BUILDDIR)/$(ARCH)

# TVOS
else ifeq ($(findstring tvos,$(MAKECMDGOALS)),tvos)
ifndef TVOS_MIN_SDK_VERSION
TVOS_MIN_SDK_VERSION=12.0
endif
BUILDDIR:=build/tvos
TOOLCHAIN:=$(shell xcode-select -p)/Toolchains/XcodeDefault.xctoolchain/usr/bin
export AR=$(TOOLCHAIN)/ar
export AS=$(TOOLCHAIN)/as
export CC=$(TOOLCHAIN)/clang
export CXX=$(TOOLCHAIN)/clang++
export LD=$(TOOLCHAIN)/ld
export NM=$(TOOLCHAIN)/nm
export OBJCOPY=objcopy
export RANLIB=$(TOOLCHAIN)/ranlib
export LIBTOOL=$(TOOLCHAIN)/libtool

ifeq ($(MAKECMDGOALS),tvos_sim_cross_x86_64)
export ARCH=x86_64
XCODE_SDK:=appletvsimulator
CFLAGS+= -mtvos-version-min=$(TVOS_MIN_SDK_VERSION)
else ifeq ($(MAKECMDGOALS),tvos64_cross_arm64)
export ARCH=arm64
XCODE_SDK:=appletvos
CFLAGS+= -mtvos-version-min=$(TVOS_MIN_SDK_VERSION)
endif
CFLAGS+= -std=gnu99 -arch $(ARCH) -isysroot $(shell xcrun --sdk $(XCODE_SDK) --show-sdk-path) -fembed-bitcode
BUILDDIR:=$(BUILDDIR)/$(ARCH)

# Native builds
else
BUILDDIR:=build/native
export LD=ld
export NM=nm
export OBJCOPY=objcopy
export RANLIB=ranlib
LIBTOOL=libtool
endif

ifeq ($(UNAME),Darwin)
ifndef OPENSSLDIR
OPENSSLDIR:=/usr/local/opt/openssl
endif
LDFLAGS += -dynamiclib -Wl,-undefined,dynamic_lookup
LDFLAGS += -current_version $(VERSION) -compatibility_version $(VERSION)
LDFLAGS += -install_name $(PREFIX)/lib/lib$(LIBNAME).dylib
SHAREDLIB = $(BUILDDIR)/lib$(LIBNAME).A.dylib
SHAREDLIB_EXT:=dylib
STATIC_INPUTS=$(filter %.a,$^) $(filter %.o,$^)
TEST_LIBS = $(STATICLIB) -L$(PREFIX)/lib -lcrypto

else ifeq ($(UNAME),Linux)
LDFLAGS += -shared
SHAREDLIB = $(BUILDDIR)/lib$(LIBNAME).so.$(VERSION)
SONAME = lib$(LIBNAME).so.$(shell echo $(VERSION) | cut -f1 -d'.')
SHAREDLIB_EXT:=so
STATIC_INPUTS=-Wl,--whole-archive $(filter %.a,$^) -Wl,--no-whole-archive $(filter %.o,$^)
TEST_LIBS = $(STATICLIB) -lcrypto
else
$(error Unsupported platform $(UNAME))
endif

ifneq ($(OPENSSLDIR),)
CFLAGS += -I$(OPENSSLDIR)/include
LDFLAGS += -L$(OPENSSLDIR)/lib
endif


native : $(SHAREDLIB) $(STATICLIB)

android: $(TOOLCHAIN) $(STATICLIB)

mac:
	$(MAKE) macos64_x86_64
	$(MAKE) macos64_arm64
	$(MAKE) mac_catalyst_x86_64
	$(MAKE) mac_catalyst_arm64
	lipo -create \
		build/mac/osx/x86_64/libnistpqc.a \
		build/mac/osx/arm64/libnistpqc.a \
		-output build/mac/osx/libnistpqc.a
	lipo -create \
		build/mac/catalyst/x86_64/libnistpqc.a \
		build/mac/catalyst/arm64/libnistpqc.a \
		-output build/mac/catalyst/libnistpqc.a

macos64_x86_64: $(STATICLIB)

macos64_arm64: $(STATICLIB)

mac_catalyst_x86_64: $(STATICLIB)

mac_catalyst_arm64: $(STATICLIB)

ios: 
	$(MAKE) ios_sim_cross_arm64
	$(MAKE) ios_sim_cross_x86_64
	$(MAKE) ios64_cross_arm64
	$(MAKE) ios64_cross_arm64e
	lipo -create \
		build/ios/iphonesimulator/arm64/libnistpqc.a \
		build/ios/iphonesimulator/x86_64/libnistpqc.a \
		-output build/ios/iphonesimulator/libnistpqc.a
	lipo -create \
		build/ios/iphoneos/arm64/libnistpqc.a \
		build/ios/iphoneos/arm64e/libnistpqc.a \
		-output build/ios/iphoneos/libnistpqc.a

ios_sim_cross_arm64: $(STATICLIB)

ios_sim_cross_x86_64: $(STATICLIB)

ios64_cross_arm64: $(STATICLIB)

ios64_cross_arm64e: $(STATICLIB)

watchos:
	$(MAKE) watchos_cross_armv7k
	$(MAKE) watchos_cross_arm64_32
	$(MAKE) watchos_sim_cross_x86_64
	$(MAKE) watchos_sim_cross_i386
	lipo -create \
		build/watchos/armv7k/libnistpqc.a \
		build/watchos/arm64_32/libnistpqc.a \
		build/watchos/x86_64/libnistpqc.a \
		build/watchos/i386/libnistpqc.a \
		-output build/watchos/libnistpqc.a

watchos_cross_armv7k: $(STATICLIB)

watchos_cross_arm64_32: $(STATICLIB)

watchos_sim_cross_x86_64: $(STATICLIB)

watchos_sim_cross_i386: $(STATICLIB)

tvos:
	$(MAKE) tvos_sim_cross_x86_64
	$(MAKE) tvos64_cross_arm64
	lipo -create \
		build/tvos/arm64/libnistpqc.a \
		build/tvos/x86_64/libnistpqc.a \
		-output build/tvos/libnistpqc.a

tvos_sim_cross_x86_64: $(STATICLIB)

tvos64_cross_arm64: $(STATICLIB)

# Shared library (libnistpqc.dylib or libnistpqc.so)
$(SHAREDLIB) : $(ARCHIVES) $(OBJECTS)
ifeq ($(UNAME),Darwin)
	$(CC) $(LDFLAGS) $(STATIC_INPUTS) -o $@ -L$(PREFIX)/lib
else ifeq ($(UNAME),Linux)
	$(CC) $(LDFLAGS) $(STATIC_INPUTS) -Wl,-soname,$(SONAME) -o $@
else
	@echo "Unsupported platform $(UNAME)"
	@exit -1
endif

# Static library (libnistpqc.a)
make-archive = "create $(1)\n $(foreach lib,$(2),addlib $(lib)\n) $(foreach obj,$(3),addmod $(obj)\n) save\n end\n"
$(STATICLIB) : $(ARCHIVES) $(OBJECTS)
ifeq ($(UNAME),Darwin)
	$(LIBTOOL) -static -o $@ $(STATIC_INPUTS)
else ifeq ($(UNAME),Linux)
	printf $(call make-archive,$@,$(filter %.a,$^),$(filter %.o,$^)) | $(AR) -M
else
	@echo "Unsupported platform $(UNAME)"
	@exit -1
endif

# Pattern rules for compiling source code
$(OBJDIR)/%.o : %.c | makedir
	$(CC) $(CFLAGS) -c -o $@ $<
$(OBJDIR)/rng.o : common/rng.c | makedir
	$(CC) $(CFLAGS) -c -o $@ $<


# Function that generates the targets for one cipher subdirectory
define build_archive
$(1)_ARCHIVE=$(BUILDDIR)/lib$(1).a
$(1)_OBJDIR=$(OBJDIR)/$(1)
ifneq (,$$(wildcard crypto/$(1)/SOURCES))
$(1)_SOURCES=$$(patsubst %,crypto/$(1)/%,$$(shell cat crypto/$(1)/SOURCES))
else
$(1)_SOURCES=$$(wildcard crypto/$(1)/*.c)
endif
$(1)_SOURCES+= scripts/aux_api.c scripts/defunc_pk_from_sk.c
$(1)_OBJS_C = $$(patsubst %.c,$$($(1)_OBJDIR)/%.o, $$(filter %.c, $$($(1)_SOURCES)))
$(1)_OBJS_S = $$(patsubst %.S,$$($(1)_OBJDIR)/%.o, $$(filter %.S, $$($(1)_SOURCES)))
$(1)_OBJS = $$($(1)_OBJS_C) $$($(1)_OBJS_S)

$$($(1)_ARCHIVE) : $$($(1)_OBJS)
ifeq ($(UNAME),Linux)
	$(AR) cr $$@ $$^
	$(RANLIB) $$@
else ifeq ($(UNAME),Darwin)
	$(LIBTOOL) -static -o $$@ $$^
endif
	bash scripts/update_library.sh $(1) $$@

$$($(1)_OBJS_C) : $$($(1)_OBJDIR)/%.o : %.c
	@mkdir -p $$(dir $$@)
	$$(CC) $$(CFLAGS) $$($(1)_FLAGS) -Icrypto/$(1) -c -o $$@ $$<

$$($(1)_OBJS_S) : $$($(1)_OBJDIR)/%.o : %.S
	@mkdir -p $$(dir $$@)
	$$(CC) $$(CFLAGS) -D__ASM_UNDER__ -E $$^ | $$(AS) -O3 -o $$@

endef

# Generate targets for all the cipher subdirectories
$(foreach cipher,$(DIRS),$(eval $(call build_archive,$(cipher))))


# Tests
TEST_EXE = $(BUILDDIR)/nistpqc_test
test: $(STATICLIB) test/nistpqc_test.c
	$(CC) $(CFLAGS) $(INCLUDE) -I. -c -o $(OBJDIR)/nistpqc_test.o test/nistpqc_test.c
ifneq ($(OPENSSLDIR),)
	$(CC) -o $(TEST_EXE) $(OBJDIR)/nistpqc_test.o -L$(OPENSSLDIR)/lib $(TEST_LIBS)
else
	$(CC) -o $(TEST_EXE) $(OBJDIR)/nistpqc_test.o $(TEST_LIBS)
endif

# Installation
INSTALL = install
install: $(STATICLIB) $(SHAREDLIB)
	@echo "Installing static library $(notdir $(STATICLIB))"
	@$(INSTALL) -d $(PREFIX)/lib
	@$(INSTALL) -m 644 $(STATICLIB) $(PREFIX)/lib/
ifeq ($(UNAME),Darwin)
	@ln -sf $(PREFIX)/lib/$(notdir $(SHAREDLIB)) $(PREFIX)/lib/lib$(LIBNAME).dylib
else ifeq ($(UNAME),Linux)
	@ln -sf $(PREFIX)/lib/$(notdir $(SHAREDLIB)) $(PREFIX)/lib/$(SONAME)
	@ln -sf $(PREFIX)/lib/$(notdir $(SHAREDLIB)) $(PREFIX)/lib/lib$(LIBNAME).so
else
	@echo "Unsupported platform $(UNAME)"
	@exit -1
endif
	@echo "Installing shared library $(notdir $(SHAREDLIB))"
	@$(INSTALL) -d $(PREFIX)/lib
	@$(INSTALL) -m 644 $(SHAREDLIB) $(PREFIX)/lib/
	@echo "Installing header files"
	@$(INSTALL) -d $(PREFIX)/include/nistpqc
	@$(INSTALL) -m 644 nistpqc_api.h $(PREFIX)/include/nistpqc/api.h
ifeq ($(UNAME),Linux)
	@ldconfig $(PREFIX)
endif

# Uninstall
uninstall:
	@echo "Uninstalling libraries and header files"
	@rm -f $(PREFIX)/include/nistpqc_api.h
	@rm -f $(PREFIX)/lib/$(notdir $(STATICLIB))
	@rm -f $(PREFIX)/lib/$(notdir $(SHAREDLIB))
ifeq ($(UNAME),Darwin)
	@rm -f $(PREFIX)/lib/lib$(LIBNAME).dylib
else ifeq ($(UNAME),Linux)
	@rm -f $(PREFIX)/lib/$(SONAME)
	@rm -f $(PREFIX)/lib/lib$(LIBNAME).so
endif

makedir :
	@mkdir -p $(OBJDIR)

clean :
	@rm -rf build



.PHONY : subdirs $(DIRS)
.PHONY : subdirs $(TESTDIRS)
.PHONY : all test install uninstall clean
