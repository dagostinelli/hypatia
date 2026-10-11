# `QuaternionSlerp` uses a normalized lerp up to 0.64 rad: angle errors up to 9e-4 rad

| | |
|---|---|
| Library | raymath (raylib 6.0, dbc56a8) |
| Function | `QuaternionSlerp` |
| Kind | wrong result (accuracy) |
| Precision | float |
| Status | still present in raylib 6.0 (the latest release) and at master 453cc35 (2026-10-10): the program prints the same |

## Summary

`QuaternionSlerp` switches to `QuaternionNlerp` when the dot product of the quaternions
is above 0.95, that is for rotations up to 2 acos(0.95) = 0.64 rad (36 degrees) apart.  A
normalized lerp does not move at constant angular speed, so the angle it produces at
parameter t is not t times the angle.  For rotations 0.6 rad apart the program below finds
an angle error of 8.7e-4 rad (0.05 degrees); at 0.3 rad, 1.1e-4 rad.  hypatia's slerp, on
the same inputs, stays below 6e-8 rad.

## Reproduction

```c
#define RAYMATH_STATIC_INLINE
#include <raymath.h>
#include <math.h>
#include <stdio.h>

int main(void)
{
	double angles[] = {0.06, 0.3, 0.6};
	int i;

	for (i = 0; i < 3; i++) {
		double angle = angles[i], t, largest = 0;
		Quaternion a = {0, 0, 0, 1}, b = {0, 0, (float)sin(angle / 2), (float)cos(angle / 2)};

		for (t = 0.05; t < 1; t += 0.05) {
			Quaternion r = QuaternionSlerp(a, b, (float)t);
			double e = fabs(2 * atan2(r.z, r.w) - t * angle);
			largest = e > largest ? e : largest;
		}
		printf("QuaternionSlerp, %g rad apart: largest angle error %.2g rad\n", angle, largest);
	}
	return 0;
}
```

Output (x86-64, gcc 13.3, `-O2`):

```text
QuaternionSlerp, 0.06 rad apart: largest angle error 8.7e-07 rad
QuaternionSlerp, 0.3 rad apart: largest angle error 0.00011 rad
QuaternionSlerp, 0.6 rad apart: largest angle error 0.00087 rad
```

Expected: an angle error of the order of the float rounding of the angle (0.6 x 1.2e-7 =
7e-8 rad).

## Cause

[`raymath.h` lines 2306-2349](https://github.com/raysan5/raylib/blob/dbc56a87da87d973a9c5baa4e7438a9d20121d28/src/raymath.h#L2306-L2349):
`else if (cosHalfTheta > 0.95f) result = QuaternionNlerp(q1, q2, amount);`.

## Suggested fix

Compute theta as `2 atan2(|a - b|, |a + b|)` (accurate for small angles, unlike `acosf`),
use the slerp weights for every theta above zero, and fall back to the lerp only when
theta is 0.

## How hypatia does it

`quaternion_slerp` takes the angle from `2 atan2(|q - r|, |q + r|)` and uses the slerp
weights for every nonzero angle.

```c
#define HYPATIA_SINGLE_PRECISION_FLOATS
#define HYPATIA_IMPLEMENTATION
#include <math.h>
#include <stdio.h>
#include "hypatia.h"

int main(void)
{
	double angles[] = {0.06, 0.3, 0.6};
	int i;

	for (i = 0; i < 3; i++) {
		double angle = angles[i], t, largest = 0;
		struct quaternion a, b, r;

		quaternion_setf4(&a, 0, 0, 0, 1);
		quaternion_setf4(&b, 0, 0, (float)sin(angle / 2), (float)cos(angle / 2));
		for (t = 0.05; t < 1; t += 0.05) {
			double e;

			quaternion_slerp(&a, &b, (float)t, &r);
			e = fabs(2 * atan2(r.z, r.w) - t * angle);
			largest = e > largest ? e : largest;
		}
		printf("quaternion_slerp, %g rad apart: largest angle error %.2g rad\n", angle, largest);
	}
	return 0;
}
```

```text
quaternion_slerp, 0.06 rad apart: largest angle error 5e-09 rad
quaternion_slerp, 0.3 rad apart: largest angle error 1.9e-08 rad
quaternion_slerp, 0.6 rad apart: largest angle error 5.3e-08 rad
```

## Checking

`compare/check_reports.py docs/reports/raymath/06-slerp-nlerp-threshold.md` builds both
programs above and compares their output with this report.
