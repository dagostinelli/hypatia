# `QuaternionToEuler` loses half its digits near a y angle of +-pi/2, and gives wrong angles for a quaternion that is not unit length

| | |
|---|---|
| Library | raylib 6.0 (dbc56a8), raymath.h |
| Function | `QuaternionToEuler(Quaternion)` |
| Kind | precision; wrong result |
| Precision | float |
| Status | still present in raylib 6.0 (the latest release) and at master fdc8b37 (2026-10-12): the program prints the same |

## Summary

`QuaternionToEuler` takes the y angle as `asinf(2 (w y - z x))`, clamped to [-1, 1].  Near
+-1 the derivative of asin is infinite, so a y angle d below pi/2 has an error of about
eps/(2 d): the rotation the angles describe is off by 2.1e-4 for d = 1e-4, where about 1e-7
is possible.  The x and z angles use `1 - 2 (...)`, and the y angle the clamp, which all
assume a unit quaternion: for the same rotation as a quaternion of length 2, all three
angles are wrong.

## Reproduction

```c
#define RAYMATH_STATIC_INLINE
#include <raymath.h>
#include <math.h>
#include <stdio.h>

static float rotation_error(Quaternion a, Quaternion b)
{
	float x = QuaternionLength(QuaternionSubtract(a, b));
	float y = QuaternionLength(QuaternionAdd(a, b));
	return x < y ? x : y;
}

int main(void)
{
	float gaps[3] = {1e-2f, 1e-3f, 1e-4f};
	Quaternion q;
	Vector3 e;
	int i;

	for (i = 0; i < 3; i++) {
		/* QuaternionFromEuler(pitch, yaw, roll): about x, y, z */
		q = QuaternionFromEuler(0.3f, 1.5707963f - gaps[i], 0.7f);
		e = QuaternionToEuler(q);
		printf("y = pi/2 - %g: y = %.8g, rotation error %.2g\n", gaps[i], e.y, rotation_error(q, QuaternionFromEuler(e.x, e.y, e.z)));
	}
	q = QuaternionFromEuler(0.3f, -0.4f, 0.7f);
	e = QuaternionToEuler(q);
	printf("unit q: %.7g %.7g %.7g\n", e.x, e.y, e.z);
	e = QuaternionToEuler(QuaternionScale(q, 2.0f));
	printf("q * 2:  %.7g %.7g %.7g\n", e.x, e.y, e.z);
	return 0;
}
```

Output (x86-64, gcc 13.3, `-O2`):

```text
y = pi/2 - 0.01: y = 1.5607835, rotation error 6.4e-06
y = pi/2 - 0.001: y = 1.5697045, rotation error 5.1e-05
y = pi/2 - 0.0001: y = 1.570451, rotation error 0.00021
unit q: 0.3 -0.4 0.7
q * 2:  1.125455 -1.570796 1.647385
```

Expected: a rotation error of about 1e-7, and the same angles for q * 2.

## Cause

[`src/raymath.h` line 2615](https://github.com/raysan5/raylib/blob/dbc56a87da87d973a9c5baa4e7438a9d20121d28/src/raymath.h#L2615):
`result.y = asinf(y0);`, and lines 2607-2620, which assume |q| = 1.

## Suggested fix

Take the y angle from an atan2, for example
`atan2f(y0, sqrtf(x1 * x1 + x0 * x0))` with x0, x1 the arguments of the x angle written
homogeneously (`w*w - x*x - y*y + z*z` for x1), and write the other two homogeneously too, or
use the half-angle form below.

## How hypatia does it

`quaternion_get_euler_anglesf3` uses the same order (Z, then Y, then X: q = qz qy qx).  With
half angles a, b, c about X, Y and Z, the sums and differences of the components are

    w + y = (cos b + sin b) cos(c - a)    z - x = (cos b + sin b) sin(c - a)
    w - y = (cos b - sin b) cos(c + a)    x + z = (cos b - sin b) sin(c + a)

so the angle about X, (c + a) - (c - a), and the angle about Z, (c + a) + (c - a), are each
one atan2 of products of these, and the pitch is 2 atan2(|(w + y, z - x)|, |(w - y, x + z)|)
- pi/2.  There is no asin, the formulas do not change when q is scaled, and at gimbal lock
the undefined c + a is weighted by cos b - sin b, which is 0 there.

```c
#define HYPATIA_SINGLE_PRECISION_FLOATS
#define HYPATIA_IMPLEMENTATION
#include <math.h>
#include <stdio.h>
#include "hypatia.h"

static float rotation_error(const struct quaternion *a, const struct quaternion *b)
{
	struct quaternion d, s;
	float x, y;
	quaternion_subtract(quaternion_set(&d, a), b);
	quaternion_add(quaternion_set(&s, a), b);
	x = quaternion_magnitude(&d);
	y = quaternion_magnitude(&s);
	return x < y ? x : y;
}

int main(void)
{
	float gaps[3] = {1e-2f, 1e-3f, 1e-4f};
	struct quaternion q, back, scaled;
	float x, y, z;
	int i;
	for (i = 0; i < 3; i++) {
		quaternion_set_from_euler_anglesf3(&q, 0.3f, 1.5707963f - gaps[i], 0.7f);
		quaternion_get_euler_anglesf3(&q, &x, &y, &z);
		quaternion_set_from_euler_anglesf3(&back, x, y, z);
		printf("y = pi/2 - %g: y = %.8g, rotation error %.2g\n", (double)gaps[i], (double)y, (double)rotation_error(&q, &back));
	}
	quaternion_set_from_euler_anglesf3(&q, 0.3f, -0.4f, 0.7f);
	quaternion_get_euler_anglesf3(&q, &x, &y, &z);
	printf("unit q: %.7g %.7g %.7g\n", (double)x, (double)y, (double)z);
	quaternion_multiplyf(quaternion_set(&scaled, &q), 2.0f);
	quaternion_get_euler_anglesf3(&scaled, &x, &y, &z);
	printf("q * 2:  %.7g %.7g %.7g\n", (double)x, (double)y, (double)z);
	return 0;
}
```

```text
y = pi/2 - 0.01: y = 1.5607964, rotation error 6.1e-08
y = pi/2 - 0.001: y = 1.5697962, rotation error 8.7e-08
y = pi/2 - 0.0001: y = 1.5706962, rotation error 1.5e-08
unit q: 0.3000001 -0.4 0.7
q * 2:  0.3000001 -0.4 0.7
```

## Checking

`compare/check_reports.py docs/reports/raymath/11-quaternion-to-euler-asin.md` builds both
programs above and compares their output with this report.
