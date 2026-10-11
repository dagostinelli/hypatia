# Reports for linmath.h

Tested: linmath.h master, commit [3eef828](https://github.com/datenwolf/linmath.h/tree/3eef82841046507e16a0f6194a61cee2eadd34b3).

| report | kind | status |
|---|---|---|
| [`quat_from_mat4x4` returns the wrong rotation: the identity becomes a half turn about x](01-quat-from-mat4x4-wrong.md) | wrong result | present at the latest commit (no releases) |
| [`vec3_norm` gives NaN, inf or zero for zero, tiny and huge vectors](02-norm-zero-tiny-huge.md) | NaN on degenerate input; wrong result | present at the latest commit (no releases) |
| [`quat_mul_vec3` and `mat4x4_from_quat` disagree, and neither rotates, when q is not unit length](03-quat-not-unit.md) | wrong result for non-unit q | present at the latest commit (no releases) |
| [`mat4x4_rotate` ignores the rotation when the axis is shorter than 1e-4, and `quat_rotate` is NaN for a zero axis](04-rotate-short-axis-ignored.md) | wrong result; NaN on degenerate input | present at the latest commit (no releases) |
| [`mat4x4_look_at` gives NaN when the eye is at the target or looks along up](05-look-at-degenerate-nan.md) | NaN on degenerate input | present at the latest commit (no releases) |
