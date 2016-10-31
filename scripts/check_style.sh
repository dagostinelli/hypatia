#! /bin/sh

pushd `pwd`

cd ..

find . -type f | grep -v -e ".txt" -e ".git" -e "build/" -e "install/" | xargs /usr/src/kernels/`uname -r`/scripts/checkpatch.pl -f --no-tree --no-summary --terse --show-types --subjective --ignore LONG_LINE,OPEN_BRACE,ELSE_AFTER_BRACE,MACRO_WITH_FLOW_CONTROL,CAMELCASE,LINE_SPACING,SPACING,PREFER_KERNEL_TYPES

popd
