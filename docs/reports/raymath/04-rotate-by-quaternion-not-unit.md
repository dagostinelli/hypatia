# `Vector3RotateByQuaternion` and `QuaternionToMatrix` disagree, and neither rotates, when q is not unit length

| | |
|---|---|
| Library | raymath (raylib 6.0, dbc56a8) |
| Function | `Vector3RotateByQuaternion`, `QuaternionToMatrix` |
| Kind | wrong result for non-unit q |
| Precision | float |
| Status | still present in raylib 6.0 (the latest release) and at master 453cc35 (2026-10-10): the program prints the same |

## Summary

Both functions build the rotation from the products of the components of q, assuming
|q| = 1, but in two different ways.  `Vector3RotateByQuaternion` uses `w*w + x*x - y*y -
z*z` on the diagonal, which is |q|^2 times the rotation; `QuaternionToMatrix` uses
`1 - 2 (y*y + z*z)`, which is a different, non-rotation matrix.  For twice the quarter
turn about Z, the first takes x to (0, 4, 0) and the second to (-3, 4, 0).  Quaternions
that are composed repeatedly drift from unit length; at a drift of 1e-3 the vector
rotated by `Vector3RotateByQuaternion` is 2e-3 longer than the input.

## Reproduction

```c
#define RAYMATH_STATIC_INLINE
#include <raymath.h>
#include <math.h>
#include <stdio.h>

int main(void)
{
	float h = sqrtf(0.5f);
	Quaternion twice = {0, 0, 2 * h, 2 * h}, drifted = {0, 0, 1.001f * h, 1.001f * h};
	Vector3 x = {1, 0, 0}, r = Vector3RotateByQuaternion(x, twice), m = Vector3Transform(x, QuaternionToMatrix(twice));

	printf("Vector3RotateByQuaternion(x, 2q) = (%g, %g, %g)\n", r.x, r.y, r.z);
	printf("QuaternionToMatrix(2q) * x       = (%g, %g, %g)\n", m.x, m.y, m.z);
	printf("Vector3RotateByQuaternion(x, 1.001 q): length %.7g\n", Vector3Length(Vector3RotateByQuaternion(x, drifted)));
	return 0;
}
```

Output (x86-64, gcc 13.3, `-O2`):

```text
Vector3RotateByQuaternion(x, 2q) = (0, 4, 0)
QuaternionToMatrix(2q) * x       = (-3, 4, 0)
Vector3RotateByQuaternion(x, 1.001 q): length 1.002001
```

Expected: (0, 1, 0) from both, and length 1 for the drifted q.

## Cause

[`raymath.h` lines 907-916](https://github.com/raysan5/raylib/blob/dbc56a87da87d973a9c5baa4e7438a9d20121d28/src/raymath.h#L907-L916):
`result.x = v.x*(q.x*q.x + q.w*q.w - q.y*q.y - q.z*q.z) + ...`, which is `q v q*`
(scaled by |q|^2);
[lines 2470-2500](https://github.com/raysan5/raylib/blob/dbc56a87da87d973a9c5baa4e7438a9d20121d28/src/raymath.h#L2470-L2500): `result.m0 = 1 - 2*(b2 + c2);` ..., the matrix of
`q v q*` with |q| = 1 substituted.

## Suggested fix

Divide both by `|q|^2` (the sum of the squares): in `Vector3RotateByQuaternion` divide
the result; in `QuaternionToMatrix` use `s = 2 / |q|^2` in place of 2.  If unit length is
a requirement, document it on both functions.

## How hypatia does it

`vector3_rotate_by_quaternion` evaluates `(2 (u . v) u + (w^2 - u . u) v + 2 w (u x v)) /
|q|^2` and `matrix4_set_from_quaternion` uses `2 / |q|^2`: both are the rotation by
`q / |q|` for any length of q, and they agree.

```c
#define HYPATIA_SINGLE_PRECISION_FLOATS
#define HYPATIA_IMPLEMENTATION
#include <math.h>
#include <stdio.h>
#include "hypatia.h"

int main(void)
{
	float h = sqrtf(0.5f);
	struct quaternion twice, drifted;
	struct vector3 r, x;
	struct matrix4 m;

	quaternion_setf4(&twice, 0, 0, 2 * h, 2 * h);
	quaternion_setf4(&drifted, 0, 0, 1.001f * h, 1.001f * h);
	vector3_rotate_by_quaternion(vector3_setf3(&r, 1, 0, 0), &twice);
	printf("vector3_rotate_by_quaternion(x, 2q) = (%g, %g, %g)\n", r.x, r.y, r.z);
	matrix4_multiplyv3(matrix4_set_from_quaternion(&m, &twice), vector3_setf3(&x, 1, 0, 0), &r);
	printf("matrix4_set_from_quaternion(2q) * x = (%g, %g, %g)\n", r.x, r.y, r.z);
	vector3_rotate_by_quaternion(vector3_setf3(&r, 1, 0, 0), &drifted);
	printf("vector3_rotate_by_quaternion(x, 1.001 q): length %.7g\n", vector3_magnitude(&r));
	return 0;
}
```

```text
vector3_rotate_by_quaternion(x, 2q) = (0, 1, 0)
matrix4_set_from_quaternion(2q) * x = (0, 1, 0)
vector3_rotate_by_quaternion(x, 1.001 q): length 1
```

## Checking

`compare/check_reports.py docs/reports/raymath/04-rotate-by-quaternion-not-unit.md` builds
both programs above and compares their output with this report.
