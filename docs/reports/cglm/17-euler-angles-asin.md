# `glm_euler_angles` loses half its digits near a y angle of +-pi/2

| | |
|---|---|
| Library | cglm 0.9.4 (1796cc5) |
| Function | `glm_euler_angles(mat4, vec3)` (`euler.h`) |
| Kind | precision |
| Precision | float |
| Status | still present in cglm 0.9.6 (the latest release) and at master 58d8c15 (2026-07-29): the program prints the same |

## Summary

`glm_euler_angles` takes the y angle as `asinf(m20)`.  Near +-1 the derivative of asin is
infinite, so a y angle d below pi/2 has an error of about eps/(2 d), and the matrix the
angles give back is off by up to 9.6e-5 for d = 1e-4, where about 1e-7 is possible.  The
x and z angles are atan2 of entries that are themselves about d, so they lose digits too.

## Reproduction

```c
#include <cglm/cglm.h>
#include <math.h>
#include <stdio.h>

int main(void)
{
	float gaps[3] = {1e-2f, 1e-3f, 1e-4f};
	int i;

	for (i = 0; i < 3; i++) {
		vec3 in = {0.3f, 1.5707963f - gaps[i], 0.7f};
		vec3 out;
		mat4 m, back;
		float worst = 0;
		int r, c;

		glm_euler(in, m); /* Rz Ry Rx */
		glm_euler_angles(m, out);
		glm_euler(out, back);
		for (c = 0; c < 3; c++) {
			for (r = 0; r < 3; r++) {
				float d = fabsf(back[c][r] - m[c][r]);
				worst = d > worst ? d : worst;
			}
		}
		printf("y = pi/2 - %g: y = %.8g, largest error in the matrix %.2g\n", gaps[i], out[1], worst);
	}
	return 0;
}
```

Output (x86-64, gcc 13.3, `-O2`):

```text
y = pi/2 - 0.01: y = 1.5607954, largest error in the matrix 8e-07
y = pi/2 - 0.001: y = 1.5698198, largest error in the matrix 2.3e-05
y = pi/2 - 0.0001: y = 1.5707964, largest error in the matrix 9.6e-05
```

Expected: errors of about 1e-7.

## Cause

[`include/cglm/euler.h` line 129](https://github.com/recp/cglm/blob/1796cc5ce298235b615dc7a4750b8c3ba56a05dd/include/cglm/euler.h#L129):
`thetaY = asinf(m20);`.

## Suggested fix

`thetaY = atan2f(m20, sqrtf(m00 * m00 + m10 * m10))` (or of the other column, `m21`
and `m22`), which is accurate near +-pi/2.  The x and z angles near gimbal lock are best
taken from a quaternion of the matrix, with the half-angle form below.

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

`compare/check_reports.py docs/reports/cglm/17-euler-angles-asin.md` builds both programs
above and compares their output with this report.
