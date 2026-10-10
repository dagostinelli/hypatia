#!/bin/sh
# SPDX-License-Identifier: MIT
#
# Regenerates every number in docs/comparison.md:
#   1. downloads GLM, cglm and Eigen at pinned versions (fetch.sh);
#   2. builds the comparison against three versions of hypatia.h, taken from
#      git: master, before the precision work, and now;
#   3. runs each in double and single precision with two random seeds;
#   4. runs probe.c, the known-answer checks, against each version;
#   5. runs the full comparison (agreement, edge cases) for now;
#   6. writes results/precision/summary.md with summarize.py, and
#      results/environment.txt.
# verify_fixes.sh, run separately, checks each bug fix against its tests.
#
# Needs git, cmake, a C and C++17 compiler, python3 and LAPACK (liblapack.so).
# Override the versions with MASTER=, BEFORE= and NOW= (git revisions).
set -e
cd "$(dirname "$0")"
MASTER=${MASTER:-125ab87}
BEFORE=${BEFORE:-35049bd}
NOW=${NOW:-HEAD}
WORK=${WORK:-build/reproduce}

./fetch.sh
mkdir -p "$WORK" results/precision results/probe

for version in master before now; do
	case $version in
	master) rev=$MASTER; defs="PRECISION_ONLY;HYP_MASTER;HYP_BEFORE_ADDITIONS" ;;
	before) rev=$BEFORE; defs="PRECISION_ONLY;HYP_BEFORE_ADDITIONS" ;;
	now) rev=$NOW; defs="PRECISION_ONLY" ;;
	esac
	mkdir -p "$WORK/$version/src"
	git show "$rev:hypatia.h" > "$WORK/$version/src/hypatia.h"
	cmake -S . -B "$WORK/$version/build" -DCMAKE_BUILD_TYPE=Release \
		-DHYPATIA_DIR="$(pwd)/$WORK/$version/src" "-DCOMPARE_DEFS=$defs" > /dev/null
	cmake --build "$WORK/$version/build" --target compare_double compare_single > /dev/null
	for precision in double single; do
		"$WORK/$version/build/compare_$precision" > "results/precision/$version.$precision.md"
		COMPARE_SEED=7 "$WORK/$version/build/compare_$precision" > "results/precision/$version.$precision.s7.md"
	done
	# known-answer checks with no other library; master fails some, so no set -e here
	cc -std=c90 -Wall -Wextra -Werror -O2 -I"$WORK/$version/src" $([ $version = master ] && echo -DMASTER) \
		probe.c -lm -o "$WORK/$version/probe"
	"$WORK/$version/probe" > "results/probe/$version.txt" || true
	echo "$version: $(git rev-parse --short "$rev")"
done

# the full comparison of now: agreement, edge cases, accuracy and precision
cmake -S . -B "$WORK/full" -DCMAKE_BUILD_TYPE=Release -DHYPATIA_DIR="$(pwd)/$WORK/now/src" -DCOMPARE_DEFS= > /dev/null
cmake --build "$WORK/full" > /dev/null
for program in double single double_depth_no; do
	"$WORK/full/compare_$program" > "results/$program.md"
done

python3 summarize.py results/precision > results/precision/summary.md

{
	echo "hypatia: master $(git rev-parse "$MASTER"), before $(git rev-parse "$BEFORE"), now $(git rev-parse "$NOW")"
	for library in glm cglm eigen; do
		echo "$library: $(git -C "ext/$library" rev-parse HEAD) ($(git -C "ext/$library" describe --tags 2>/dev/null || echo untagged))"
	done
	echo "lapack: $(readlink -f "$(grep LAPACK_LIB "$WORK/full/CMakeCache.txt" | cut -d= -f2)")"
	echo "system: $(uname -srm)"
	echo "cpu: $(grep -m1 'model name' /proc/cpuinfo 2>/dev/null | cut -d: -f2 | sed 's/^ //')"
	echo "c compiler: $(grep CMAKE_C_COMPILER: "$WORK/full/CMakeCache.txt" | cut -d= -f2) $($(grep CMAKE_C_COMPILER: "$WORK/full/CMakeCache.txt" | cut -d= -f2) --version | head -1)"
	echo "c++ compiler: $(grep CMAKE_CXX_COMPILER: "$WORK/full/CMakeCache.txt" | cut -d= -f2) $($(grep CMAKE_CXX_COMPILER: "$WORK/full/CMakeCache.txt" | cut -d= -f2) --version | head -1)"
	echo "c++ flags (Release): $(grep CMAKE_CXX_FLAGS_RELEASE: "$WORK/full/CMakeCache.txt" | cut -d= -f2)"
	echo "c flags (Release): $(grep CMAKE_C_FLAGS_RELEASE: "$WORK/full/CMakeCache.txt" | cut -d= -f2)"
	echo "floating point: $(echo | $(grep CMAKE_CXX_COMPILER: "$WORK/full/CMakeCache.txt" | cut -d= -f2) -O3 -dM -E -x c++ - | grep -E '__FLT_EVAL_METHOD__|__LDBL_MANT_DIG__|__FMA__|__SSE2__' | sed 's/#define //' | tr '\n' ' ')"
} > results/environment.txt

echo "done: results/precision/summary.md, results/environment.txt"
