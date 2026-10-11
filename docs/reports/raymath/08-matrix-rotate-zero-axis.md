# `MatrixRotate` with a zero axis returns cos(angle) times the identity

| | |
|---|---|
| Library | raymath (raylib 6.0, dbc56a8) |
| Function | `MatrixRotate` |
| Kind | wrong result on degenerate input |
| Precision | float |
| Status | still present in raylib 6.0 (the latest release) and at master 453cc35 (2026-10-10): the program prints the same |

## Summary

`MatrixRotate` normalizes the axis only when its squared length is not 0 (and not 1).  For
the zero axis it goes on with x = y = z = 0, and the matrix is `cos(angle)` on the
diagonal: a uniform scaling, not a rotation.  For a quarter turn it maps every vector to
(almost) zero.  An axis of length 1e-25, whose squared length underflows to 0, gives the
same scaling instead of the rotation about its direction.

## Reproduction

```c
#define RAYMATH_STATIC_INLINE
#include <raymath.h>
#include <stdio.h>

int main(void)
{
	Matrix m = MatrixRotate((Vector3){0, 0, 0}, PI / 2), t = MatrixRotate((Vector3){0, 0, 1e-25f}, 1);

	printf("zero axis, pi/2: diagonal (%g, %g, %g), determinant %g\n", m.m0, m.m5, m.m10, MatrixDeterminant(m));
	Vector3 x = Vector3Transform((Vector3){1, 0, 0}, t);

	printf("axis (0, 0, 1e-25), 1 rad: x goes to (%g, %g, %g), expected (%g, %g, 0)\n", x.x, x.y, x.z, cosf(1), sinf(1));
	return 0;
}
```

Output (x86-64, gcc 13.3, `-O2`):

```text
zero axis, pi/2: diagonal (-4.37114e-08, -4.37114e-08, -4.37114e-08), determinant -8.35187e-23
axis (0, 0, 1e-25), 1 rad: x goes to (0.540302, 8.41471e-26, 0), expected (0.540302, 0.841471, 0)
```

Expected: the identity for the zero axis (a rotation about no axis), and the rotation by
1 rad about Z for the axis (0, 0, 1e-25).

## Cause

[`raymath.h` lines 1791-1832](https://github.com/raysan5/raylib/blob/dbc56a87da87d973a9c5baa4e7438a9d20121d28/src/raymath.h#L1791-L1832):
`if ((lengthSquared != 1.0f) && (lengthSquared != 0.0f)) { ... normalize ... }`, and the
matrix is built from the unnormalized axis otherwise.

## Suggested fix

Return the identity for a zero axis; normalize with scaling when the squared length is
outside a safe range such as 1e-30 to 1e30.

## How hypatia does it

`matrix4_set_from_axisv3_angle` normalizes the axis with scaled normalization, which
leaves the zero vector unchanged, and builds the identity for a zero axis.

```c
#define HYPATIA_SINGLE_PRECISION_FLOATS
#define HYPATIA_IMPLEMENTATION
#include <math.h>
#include <stdio.h>
#include "hypatia.h"

int main(void)
{
	struct matrix4 m, t;
	struct vector3 zero, tiny, x, r;

	matrix4_set_from_axisv3_angle(&m, vector3_setf3(&zero, 0, 0, 0), HYP_PI / 2);
	matrix4_set_from_axisv3_angle(&t, vector3_setf3(&tiny, 0, 0, 1e-25f), 1);
	printf("zero axis, pi/2: diagonal (%g, %g, %g), determinant %g\n", m.m[0], m.m[5], m.m[10], matrix4_determinant(&m));
	matrix4_multiplyv3(&t, vector3_setf3(&x, 1, 0, 0), &r);
	printf("axis (0, 0, 1e-25), 1 rad: x goes to (%g, %g, %g), expected (%g, %g, 0)\n", r.x, r.y, r.z, cosf(1), sinf(1));
	return 0;
}
```

```text
zero axis, pi/2: diagonal (1, 1, 1), determinant 1
axis (0, 0, 1e-25), 1 rad: x goes to (0.540302, 0.841471, 0), expected (0.540302, 0.841471, 0)
```

## Checking

`compare/check_reports.py docs/reports/raymath/08-matrix-rotate-zero-axis.md` builds both
programs above and compares their output with this report.
