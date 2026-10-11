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
# HandmadeMath at master (v2.0.0 lacks HMM_QFromVecPair and HMM_RotateV3Q); linmath.h has no tags
[ -d HandmadeMath ] || { git clone -q https://github.com/HandmadeMath/HandmadeMath HandmadeMath &&
	git -C HandmadeMath -c advice.detachedHead=false checkout -q 661fef0893bccfe30342049e848b8d54e7430234; }
[ -d raylib ] || git -c advice.detachedHead=false clone -q --depth 1 --branch 6.0 https://github.com/raysan5/raylib raylib
[ -d linmath.h ] || { git clone -q https://github.com/datenwolf/linmath.h linmath.h &&
	git -C linmath.h -c advice.detachedHead=false checkout -q 3eef82841046507e16a0f6194a61cee2eadd34b3; }
# the commits the results were measured with: a moved tag is an error
check() {
	[ "$(git -C "$1" rev-parse HEAD)" = "$2" ] || { echo "$1: not at $2" >&2; exit 1; }
}
check glm 0af55ccecd98d4e5a8d1fad7de25ba429d60e863
check cglm 1796cc5ce298235b615dc7a4750b8c3ba56a05dd
check eigen 3147391d946bb4b6c68edd901f2add6ac1f31f8c
check HandmadeMath 661fef0893bccfe30342049e848b8d54e7430234
check raylib dbc56a87da87d973a9c5baa4e7438a9d20121d28
check linmath.h 3eef82841046507e16a0f6194a61cee2eadd34b3
echo "glm 1.0.1, cglm 0.9.4, eigen 3.4.0, HandmadeMath 661fef0, raylib 6.0, linmath.h 3eef828 in $(pwd)"
