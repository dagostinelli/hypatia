# Convenience targets for local development of hypatia.
# They only run cmake and ctest; the build itself is defined in the CMake files.
#
# make help lists the targets.  Run make configure (or a machine target) first;
# build, test and the checks use whatever is configured in the build directory,
# until make clean.

BUILD ?= build

# MACHINE starts a fresh build directory for another machine or generator
ifeq ($(MACHINE),mingw32)
MACHINE_OPTIONS = -DCMAKE_TOOLCHAIN_FILE=/usr/share/mingw/toolchain-mingw32.cmake
else ifeq ($(MACHINE),mingw64)
MACHINE_OPTIONS = -DCMAKE_TOOLCHAIN_FILE=/usr/share/mingw/toolchain-mingw64.cmake
else ifeq ($(MACHINE),nmake)
MACHINE_OPTIONS = -G "NMake Makefiles" -DHYPATIA_BUILD_DOCS=OFF
else ifneq ($(MACHINE),)
$(error unknown MACHINE '$(MACHINE)': use mingw32, mingw64 or nmake)
endif

# BUILD_TYPE defaults to Debug for a fresh build directory; otherwise cmake keeps the
# build type it has
ifneq ($(MACHINE),)
BUILD_TYPE ?= Debug
else ifeq ($(wildcard $(BUILD)/CMakeCache.txt),)
BUILD_TYPE ?= Debug
endif

default: test                   ## same as make test

help:                           ## list the targets
	@awk -F ':.*## ' '/^[a-zA-Z0-9_-]+:.*## /{printf "  %-20s %s\n", $$1, $$2}' $(MAKEFILE_LIST)
	@echo "  configure takes MACHINE=mingw32|mingw64|nmake and BUILD_TYPE=Debug|Release;"
	@echo "  the configure-* and machine targets take BUILD_TYPE"

configure:                      ## configure the build directory (MACHINE=, BUILD_TYPE=)
	$(if $(MACHINE),rm -rf $(BUILD))
	cmake -S . -B $(BUILD) $(MACHINE_OPTIONS) $(if $(BUILD_TYPE),-DCMAKE_BUILD_TYPE=$(BUILD_TYPE))

configure-mingw32:              ## configure a fresh build directory for mingw32
	$(MAKE) configure MACHINE=mingw32

configure-mingw64:              ## configure a fresh build directory for mingw64
	$(MAKE) configure MACHINE=mingw64

configure-nmake:                ## configure a fresh build directory for NMake
	$(MAKE) configure MACHINE=nmake

mingw32:                        ## clean, configure for mingw32, build
	$(MAKE) clean
	$(MAKE) configure MACHINE=mingw32
	$(MAKE) build

mingw64:                        ## clean, configure for mingw64, build
	$(MAKE) clean
	$(MAKE) configure MACHINE=mingw64
	$(MAKE) build

nmake:                          ## clean, configure for NMake, build
	$(MAKE) clean
	$(MAKE) configure MACHINE=nmake
	$(MAKE) build

build:                          ## build what is configured (warnings are errors)
	cmake --build $(BUILD)

test: build                     ## build, then run the tests
	ctest --test-dir $(BUILD) --output-on-failure

docs:                           ## build the Doxygen docs (warnings are errors)
	cmake --build $(BUILD) --target hypatia_docs

checksparse:                    ## experimental: run sparse (CI does not run it)
	cmake --build $(BUILD) --target checksparse

checkpatch:                     ## experimental: run checkpatch.pl (CI does not run it)
	cmake --build $(BUILD) --target checkpatch

clean:                          ## remove the build directory
	rm -rf $(BUILD)

.PHONY: default help configure configure-mingw32 configure-mingw64 configure-nmake mingw32 mingw64 nmake build test docs checksparse checkpatch clean
