#!/bin/bash

pushd `pwd`

cd ..
rm -rf build
mkdir build
cd build

cmake -G "Unix Makefiles" .. -DCMAKE_INSTALL_PREFIX=../install -DCMAKE_BUILD_TYPE=Debug -DHYPATIA_SINGLE_PRECISION_FLOATS=ON
make && ctest && make install && cpack -G "TGZ"

popd
