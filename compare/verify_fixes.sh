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
FIXES=${*:-"96c072f fb163c4 d68e63b 1588eb7 f2b1f22 9b67752 8e6eea3 b7ff6bd 79cd6d9"}

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
