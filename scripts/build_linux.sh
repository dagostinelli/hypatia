#!/bin/bash

pushd `pwd`

cd ..
rm -rf build
mkdir build
cd build

cmake -G "Unix Makefiles" .. -DCMAKE_INSTALL_PREFIX=../install -DHYPATIA_BUILD_STATIC=1 -DHYPATIA_BUILD_SHARED=1 && \
	make && \
	ctest && \
	make install && \
	make srpm && \
	make rpm

popd
