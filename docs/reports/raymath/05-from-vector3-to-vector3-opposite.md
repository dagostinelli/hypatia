# `QuaternionFromVector3ToVector3` returns the zero quaternion for opposite vectors and loses digits for nearly opposite ones

| | |
|---|---|
| Library | raymath (raylib 6.0, dbc56a8) |
| Function | `QuaternionFromVector3ToVector3` |
| Kind | wrong result; precision |
| Precision | float |
| Status | present in raylib 6.0 (the latest release); at master 453cc35 (2026-10-10) opposite vectors give a half turn (commit 0ed3570), but vectors within about 1.4e-3 rad of opposite also get that fixed half turn: the second line prints (-0, 1, 0, 0), 0.001 from the target |

## Summary

The function builds (from x to, |from| |to| + from . to) and normalizes it.  For opposite
vectors that is (0, 0, 0, 0), and the normalization leaves it unchanged: the result is the
zero quaternion, not a rotation.  For nearly opposite vectors `|from| |to| + from . to`
cancels: from x to (-1, 1e-3, 0) the w component comes out as 0.000477 instead of 0.0005,
and the quaternion takes x to a direction 4.6e-5 away from the target, about 390 float
epsilons.

## Reproduction

```c
#define RAYMATH_STATIC_INLINE
#include <raymath.h>
#include <stdio.h>

int main(void)
{
	Vector3 x = {1, 0, 0}, to = Vector3Normalize((Vector3){-1, 1e-3f, 0}), r;
	Quaternion q = QuaternionFromVector3ToVector3(x, (Vector3){-1, 0, 0});

	printf("x to -x: (%g, %g, %g, %g)\n", q.x, q.y, q.z, q.w);
	q = QuaternionFromVector3ToVector3(x, to);
	r = Vector3RotateByQuaternion(x, q);
	printf("x to (-1, 1e-3, 0): (%g, %g, %g, %.6g), takes x to (%.7g, %.7g, %g), %.2g from the target\n",
	       q.x, q.y, q.z, q.w, r.x, r.y, r.z, Vector3Distance(r, to));
	return 0;
}
```

Output (x86-64, gcc 13.3, `-O2`):

```text
x to -x: (0, -0, 0, 0)
x to (-1, 1e-3, 0): (0, -0, 1, 0.000476837), takes x to (-0.9999995, 0.0009536745, 0), 4.6e-05 from the target
```

Expected: a half turn about an axis perpendicular to x, such as (0, 0, 1, 0), for the first;
w = 0.0005 for the second.

## Cause

[`raymath.h` lines 2378-2403](https://github.com/raysan5/raylib/blob/dbc56a87da87d973a9c5baa4e7438a9d20121d28/src/raymath.h#L2378-L2403):
`result.w = sqrtf(cross.x*cross.x + cross.y*cross.y + cross.z*cross.z + cos2Theta*cos2Theta) + cos2Theta;`.

## Suggested fix

Compute the half angle from `atan2(|a - b|, |a + b|)` of the normalized vectors, which has
no cancellation, and return a half turn about a perpendicular axis when `|a + b|` is zero.

## How hypatia does it

`quaternion_get_rotation_tov3` takes the half angle from `atan2(|f - t|, |f + t|)` of the
normalized vectors, and for opposite vectors returns a half turn about an axis
perpendicular to the first.

```c
#define HYPATIA_SINGLE_PRECISION_FLOATS
#define HYPATIA_IMPLEMENTATION
#include <stdio.h>
#include "hypatia.h"

int main(void)
{
	struct vector3 x, to, r;
	struct quaternion q;

	vector3_setf3(&x, 1, 0, 0);
	quaternion_get_rotation_tov3(&x, vector3_setf3(&to, -1, 0, 0), &q);
	printf("x to -x: (%g, %g, %g, %g)\n", q.x, q.y, q.z, q.w);
	vector3_normalize(vector3_setf3(&to, -1, 1e-3f, 0));
	quaternion_get_rotation_tov3(&x, &to, &q);
	vector3_rotate_by_quaternion(vector3_set(&r, &x), &q);
	printf("x to (-1, 1e-3, 0): (%g, %g, %g, %.6g), takes x to (%.7g, %.7g, %g), %.2g from the target\n",
	       q.x, q.y, q.z, q.w, r.x, r.y, r.z, vector3_distance(&r, &to));
	return 0;
}
```

```text
x to -x: (0, 0, 1, 0)
x to (-1, 1e-3, 0): (0, 0, 1, 0.0005), takes x to (-0.9999995, 0.0009999996, 0), 0 from the target
```

## Checking

`compare/check_reports.py docs/reports/raymath/05-from-vector3-to-vector3-opposite.md`
builds both programs above and compares their output with this report.
