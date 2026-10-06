# Convenience targets for local development of hypatia.
# They only run cmake and ctest; the build itself is defined in the CMake files.
# Included by Makefile.

check:
	@echo try:
	@echo  make checksyntax
	@echo  make checksparse
	@echo  make checkpatch

configure:
	@cmake -S . -B $(BUILD)

checksyntax: configure
	@echo checksyntax
	@cmake --build $(BUILD)

checksparse: configure
	@echo checksparse
	@cmake --build $(BUILD) --target checksparse

checkpatch: configure
	@echo checkpatch
	@cmake --build $(BUILD) --target checkpatch
