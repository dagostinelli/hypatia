#!/bin/bash

pushd `pwd`

mkdir build
cd build

cmake -G "Unix Makefiles" .. -DCMAKE_INSTALL_PREFIX=../install -DHYPATIA_BUILD_STATIC=1 -DHYPATIA_BUILD_SHARED=1 -DCMAKE_BUILD_TYPE=Release
make && ctest && make install && cpack -G "TGZ"

popd
