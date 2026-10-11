#!/bin/sh
# SPDX-License-Identifier: MIT
#
# The speed gate: the instructions each function executes per call, counted by
# valgrind (callgrind) in the bench_ functions of speed.c.  The count is exact
# and repeats on every run, which a time on a shared CI runner does not.
#
#   speed.sh PROGRAM                  prints the table
#   speed.sh PROGRAM --check FILE     compares with the table in FILE: fails when
#                                     a function takes more than 2% more
#                                     instructions, or a row is missing on
#                                     either side

set -e

program=$1
calls=1000
work=$(mktemp -d)
trap 'rm -rf "$work"' EXIT

valgrind --tool=callgrind --toggle-collect='bench_*' --callgrind-out-file="$work/out" "$program" 2>/dev/null
callgrind_annotate --inclusive=yes --threshold=100 "$work/out" |
	sed -n 's/^ *\([0-9,]*\) ([^)]*) *[^ ]*:bench_\([a-z0-9_]*\) .*$/\2 \1/p' |
	tr -d , |
	awk -v calls=$calls '{ printf "%s | %.1f\n", $1, $2 / calls }' |
	sort > "$work/table"

if [ "$2" != "--check" ]; then
	echo "# function | instructions per call ($(${CC:-cc} --version | head -1))"
	cat "$work/table"
	exit 0
fi

grep -v '^#' "$3" | sort > "$work/baseline"
awk -F ' [|] ' '
	NR == FNR { base[$1] = $2; next }
	{
		now[$1] = $2
		if (!($1 in base)) {
			print "FAIL  " $1 ": measured, not in the baseline"; failed = 1
		} else if ($2 > base[$1] * 1.02) {
			print "FAIL  " $1 ": " $2 " instructions per call; baseline " base[$1]; failed = 1
		} else if ($2 < base[$1] * 0.98) {
			print "better " $1 ": " $2 " instructions per call; baseline " base[$1] ": update the baseline"
		}
		count++
	}
	END {
		for (f in base) {
			if (!(f in now)) {
				print "FAIL  " f ": in the baseline, not measured"; failed = 1
			}
		}
		print count " functions, " (failed ? "FAILED" : "none slower than the baseline")
		exit failed
	}' "$work/baseline" "$work/table"
