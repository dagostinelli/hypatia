#!/bin/sh
# SPDX-License-Identifier: MIT
#
# For each bug fix on correctness-h: build and run the fix commit's own tests
# twice, once with its hypatia.h and once with the hypatia.h of the commit
# before it.  A fix is confirmed when the tests pass with the fix and fail
# without it.
#
# usage: verify_fixes.sh [commit ...]    (default: the fixes in docs/comparison.md)
#
# Needs git, cmake and a C compiler.  Prints one line per commit.
set -e
cd "$(dirname "$0")/.."
WORK=${WORK:-compare/build/verify}
FIXES=${*:-"2700d4b 8b69e46 3ff02c7 5ffe649 9ebeeb8 f5137e6 f1f9ad2 735a70e 87032b1 ed5c45a 082ad4a 860b94d"}

run() { # tree -> pass | fail | no build
	if ! cmake -S "$1" -B "$1/build" -DCMAKE_BUILD_TYPE=Release > "$1/log" 2>&1 ||
	   ! cmake --build "$1/build" -j 4 >> "$1/log" 2>&1; then
		echo "no build"
	elif (cd "$1/build" && ctest > ../ctest.log 2>&1); then
		echo pass
	else
		# the first failed assertion, from the log of the failing tests
		echo "fail: $(grep -h -m1 'assertion failed' "$1/build/Testing/Temporary/LastTest.log" |
			sed 's|^.*/test/|test/|; s| : (assertion failed) | |')"
	fi
}

rm -rf "$WORK"
for c in $FIXES; do
	for v in with without; do
		mkdir -p "$WORK/$c/$v"
		git archive "$c" | tar -x -C "$WORK/$c/$v"
	done
	git show "$c^:hypatia.h" > "$WORK/$c/without/hypatia.h"
	printf '%s  with the fix: %s;  hypatia.h of %s: %s  (%s)\n' "$c" "$(run "$WORK/$c/with")" \
		"$(git rev-parse --short "$c^")" "$(run "$WORK/$c/without")" "$(git log -1 --format=%s "$c")"
done
