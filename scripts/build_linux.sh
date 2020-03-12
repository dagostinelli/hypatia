#!/bin/bash

pushd `pwd`

cd $(dirname $(realpath -s "$BASH_SOURCE"))

cd ..

cmake -Bbuild -DCMAKE_INSTALL_PREFIX=../install -DCMAKE_BUILD_TYPE=Debug -DHYPATIA_SINGLE_PRECISION_FLOATS=ON && \
	cmake --build build --clean-first -- -j8
	cd build && ctest && cd ..

popd
