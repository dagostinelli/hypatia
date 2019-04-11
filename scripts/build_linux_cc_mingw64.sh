#!/bin/bash

pushd `pwd`

cd $(dirname $(realpath -s "$BASH_SOURCE"))

cd ..
rm -rf build
mkdir build
cd build

cmake -G "Unix Makefiles" .. -DCMAKE_INSTALL_PREFIX=../install -DHYPATIA_BUILD_STATIC=ON -DHYPATIA_BUILD_SHARED=ON -DCMAKE_BUILD_TYPE=Debug  -DCMAKE_TOOLCHAIN_FILE=/usr/share/mingw/toolchain-mingw32.cmake
make && ctest && make install && cpack -G "TGZ"

popd
