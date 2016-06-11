@ECHO ON
PUSHD "%~dp0"

rmdir /s /q build
mkdir build
cd build
cmake -trace --warn-unitialized --warn-unused_vars -G "NMake Makefiles" .. -DCMAKE_INSTALL_PREFIX=../install -DHYPATIA_BUILD_STATIC=1 -DHYPATIA_BUILD_DOCS=0 -DHYPATIA_USE_SINGLE=0 -DCMAKE_BUILD_TYPE=Release
nmake
ctest
cpack -G "ZIP"

POPD
