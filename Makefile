# Convenience targets for local development of hypatia.
# They only run cmake and ctest; the build itself is defined in the CMake files.

BUILD ?= build

default: test

include build.mk
include check.mk

clean:
	rm -rf $(BUILD) $(BUILD)-mingw32 $(BUILD)-mingw64 $(BUILD)-nmake

.PHONY: default clean
