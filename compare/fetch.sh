#!/bin/sh
# Downloads the libraries the comparison builds against, at pinned versions,
# into compare/ext (not committed).
set -e
cd "$(dirname "$0")"
mkdir -p ext
cd ext
[ -d glm ] || git -c advice.detachedHead=false clone -q --depth 1 --branch 1.0.1 https://github.com/g-truc/glm glm
[ -d cglm ] || git -c advice.detachedHead=false clone -q --depth 1 --branch v0.9.4 https://github.com/recp/cglm cglm
[ -d eigen ] || git -c advice.detachedHead=false clone -q --depth 1 --branch 3.4.0 https://gitlab.com/libeigen/eigen.git eigen
# the commits the results were measured with: a moved tag is an error
check() {
	[ "$(git -C "$1" rev-parse HEAD)" = "$2" ] || { echo "$1: not at $2" >&2; exit 1; }
}
check glm 0af55ccecd98d4e5a8d1fad7de25ba429d60e863
check cglm 1796cc5ce298235b615dc7a4750b8c3ba56a05dd
check eigen 3147391d946bb4b6c68edd901f2add6ac1f31f8c
echo "glm 1.0.1, cglm 0.9.4, eigen 3.4.0 in $(pwd)"
