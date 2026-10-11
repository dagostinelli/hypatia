# `QuaternionToAxisAngle` is inaccurate for small rotations: 2% error at 1e-3 rad

| | |
|---|---|
| Library | raymath (raylib 6.0, dbc56a8) |
| Function | `QuaternionToAxisAngle` |
| Kind | precision |
| Precision | float |
| Status | still present in raylib 6.0 (the latest release) and at master 453cc35 (2026-10-10): the program prints the same |

## Summary

`QuaternionToAxisAngle` takes the angle from `2 acosf(w)` and the axis from
`(x, y, z) / sqrtf(1 - w*w)`.  Near w = 1 both lose digits: `acosf` because its slope is
infinite there, and `1 - w*w` by cancellation.  For a rotation by 1e-3 rad about Z the
program below gets the angle 0.0009766 (2.3% off) and an axis of length 1.024.  At
4e-4 rad, w rounds to 1: the angle is 0 and the axis an arbitrary (1, 0, 0).

## Reproduction

```c
#define RAYMATH_STATIC_INLINE
#include <raymath.h>
#include <math.h>
#include <stdio.h>

int main(void)
{
	double angles[] = {1e-3, 4e-4};
	int i;

	for (i = 0; i < 2; i++) {
		Quaternion q = {0, 0, (float)sin(angles[i] / 2), (float)cos(angles[i] / 2)};
		Vector3 axis;
		float angle;

		QuaternionToAxisAngle(q, &axis, &angle);
		printf("%g rad about z: angle %.7g, axis (%g, %g, %g)\n", angles[i], angle, axis.x, axis.y, axis.z);
	}
	return 0;
}
```

Output (x86-64, gcc 13.3, `-O2`):

```text
0.001 rad about z: angle 0.0009765625, axis (0, 0, 1.024)
0.0004 rad about z: angle 0, axis (1, 0, 0)
```

Expected: the angle to float precision and the axis (0, 0, 1).

## Cause

[`raymath.h` lines 2543-2577](https://github.com/raysan5/raylib/blob/dbc56a87da87d973a9c5baa4e7438a9d20121d28/src/raymath.h#L2543-L2577):
`float resAngle = 2.0f*acosf(q.w); float den = sqrtf(1.0f - q.w*q.w);`.

## Suggested fix

Take the angle from `2 atan2(|(x, y, z)|, w)` and the axis from `(x, y, z) / |(x, y, z)|`;
neither cancels near w = 1.

## How hypatia does it

`quaternion_get_axis_anglev3` computes the angle as `2 atan2(|(x, y, z)|, w)` and the axis
by normalizing the vector part.

```c
#define HYPATIA_SINGLE_PRECISION_FLOATS
#define HYPATIA_IMPLEMENTATION
#include <math.h>
#include <stdio.h>
#include "hypatia.h"

int main(void)
{
	double angles[] = {1e-3, 4e-4};
	int i;

	for (i = 0; i < 2; i++) {
		struct quaternion q;
		struct vector3 axis;
		float angle;

		quaternion_setf4(&q, 0, 0, (float)sin(angles[i] / 2), (float)cos(angles[i] / 2));
		quaternion_get_axis_anglev3(&q, &axis, &angle);
		printf("%g rad about z: angle %.7g, axis (%g, %g, %g)\n", angles[i], angle, axis.x, axis.y, axis.z);
	}
	return 0;
}
```

```text
0.001 rad about z: angle 0.0009999999, axis (0, 0, 1)
0.0004 rad about z: angle 0.0004, axis (0, 0, 1)
```

## Checking

`compare/check_reports.py docs/reports/raymath/07-to-axis-angle-small-rotations.md` builds
both programs above and compares their output with this report.
