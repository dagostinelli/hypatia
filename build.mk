# Convenience targets for local development of hypatia.
# They only run cmake and ctest; the build itself is defined in the CMake files.
# Included by Makefile.

test:
	cmake -S . -B $(BUILD) -DCMAKE_BUILD_TYPE=Debug
	cmake --build $(BUILD)
	ctest --test-dir $(BUILD) --output-on-failure

mingw32:
	cmake -S . -B $(BUILD)-mingw32 -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=/usr/share/mingw/toolchain-mingw32.cmake
	cmake --build $(BUILD)-mingw32
	ctest --test-dir $(BUILD)-mingw32 --output-on-failure

mingw64:
	cmake -S . -B $(BUILD)-mingw64 -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=/usr/share/mingw/toolchain-mingw64.cmake
	cmake --build $(BUILD)-mingw64
	ctest --test-dir $(BUILD)-mingw64 --output-on-failure

nmake:
	cmake -S . -B $(BUILD)-nmake -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=Release -DHYPATIA_BUILD_DOCS=OFF
	cmake --build $(BUILD)-nmake
	ctest --test-dir $(BUILD)-nmake --output-on-failure

.PHONY: test mingw32 mingw64 nmake
