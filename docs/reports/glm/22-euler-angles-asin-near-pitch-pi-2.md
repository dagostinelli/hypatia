# `glm::eulerAngles` loses half its digits near a pitch of +-pi/2, and gives a wrong pitch for a quaternion that is not unit length

| | |
|---|---|
| Library | GLM 1.0.1 (0af55cc) |
| Function | `glm::eulerAngles(qua)`, `glm::yaw(qua)` (`gtc/quaternion.inl`) |
| Kind | precision; wrong result |
| Precision | double and float |
| Status | still present in GLM 1.0.3 (the latest release) and at master 6f14f47 (2026-04-07): the program prints the same |

## Summary

`glm::eulerAngles` takes the middle angle (about Y) as `asin(clamp(-2 (x z - w y), -1, 1))`.
Near +-1 the derivative of asin is infinite: the argument is rounded to within half an ulp,
and for a pitch d below pi/2 that becomes an error of about eps/(2 d) in the angle.  The
rotation the angles describe is then off by 1.6e-10 in double for d = 1e-7, and by 1.5e-4 in
float for d = 1e-4, where about 1e-16 and 1e-7 are possible.  The formula also assumes a
unit quaternion: for the same rotation as a quaternion of length 2, the argument is 4 times
too large, the clamp turns it into -1, and the angle is -pi/2 instead of -0.4.

## Reproduction

```cpp
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <cmath>
#include <cstdio>

int main()
{
	/* rotations with a pitch just below pi/2 */
	double gaps[3] = {1e-3, 1e-5, 1e-7};
	for (double g : gaps) {
		glm::dvec3 in(0.3, 1.5707963267948966 - g, 0.7);
		glm::dquat q(in);
		glm::dvec3 out = glm::eulerAngles(q);
		glm::dquat back(out);
		double d = std::fmin(glm::length(back - q), glm::length(back + q));
		std::printf("pitch pi/2 - %g: glm::eulerAngles pitch = %.17g, rotation error %.2g\n", g, out.y, d);
	}
	/* the same rotation as a quaternion of length 2 */
	glm::dquat q(glm::dvec3(0.3, -0.4, 0.7));
	glm::dvec3 a = glm::eulerAngles(q);
	glm::dvec3 b = glm::eulerAngles(q * 2.0);
	std::printf("unit q:     %.17g %.17g %.17g\n", a.x, a.y, a.z);
	std::printf("q * 2:      %.17g %.17g %.17g\n", b.x, b.y, b.z);
	/* float */
	glm::quat qf(glm::vec3(0.3f, 1.5707963267948966f - 1e-4f, 0.7f));
	glm::vec3 of = glm::eulerAngles(qf);
	glm::quat bf(of);
	std::printf("float, pitch pi/2 - 1e-4: rotation error %.2g\n", std::fmin(glm::length(bf - qf), glm::length(bf + qf)));
}
```

Output (x86-64, gcc 13.3, `-O2`):

```text
pitch pi/2 - 0.001: glm::eulerAngles pitch = 1.5697963267947934, rotation error 5.4e-14
pitch pi/2 - 1e-05: glm::eulerAngles pitch = 1.5707863267722784, rotation error 1.1e-11
pitch pi/2 - 1e-07: glm::eulerAngles pitch = 1.5707962268348685, rotation error 1.6e-10
unit q:     0.29999999999999999 -0.39999999999999986 0.70000000000000007
q * 2:      0.29999999999999999 -1.5707963267948966 0.70000000000000007
float, pitch pi/2 - 1e-4: rotation error 0.00015
```

Expected: a rotation error of about 1e-16 (1e-7 in float), and the same angles for q * 2.

## Cause

[`gtc/quaternion.inl` line 43](https://github.com/g-truc/glm/blob/0af55ccecd98d4e5a8d1fad7de25ba429d60e863/glm/gtc/quaternion.inl#L43):
`return asin(clamp(static_cast<T>(-2) * (q.x * q.z - q.w * q.y), static_cast<T>(-1), static_cast<T>(1)));`.
The other two angles are atan2 of homogeneous expressions and do not depend on the length.

## Suggested fix

Take the middle angle from an atan2 as well, for example
`atan2(-2 (x z - w y), sqrt((w w + x x - y y - z z)^2 + (2 (x y + w z))^2))`, which is
homogeneous of degree 2 like the other two angles; or use the half-angle form below.

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
#define HYPATIA_IMPLEMENTATION
#include <math.h>
#include <stdio.h>
#include "hypatia.h"

static double rotation_error(const struct quaternion *a, const struct quaternion *b)
{
	struct quaternion d, s;
	double x, y;
	quaternion_subtract(quaternion_set(&d, a), b);
	quaternion_add(quaternion_set(&s, a), b);
	x = quaternion_magnitude(&d);
	y = quaternion_magnitude(&s);
	return x < y ? x : y;
}

int main(void)
{
	double gaps[3] = {1e-3, 1e-5, 1e-7};
	struct quaternion q, back, scaled;
	double x, y, z;
	int i;
	for (i = 0; i < 3; i++) {
		quaternion_set_from_euler_anglesf3(&q, 0.3, 1.5707963267948966 - gaps[i], 0.7);
		quaternion_get_euler_anglesf3(&q, &x, &y, &z);
		quaternion_set_from_euler_anglesf3(&back, x, y, z);
		printf("pitch pi/2 - %g: pitch = %.17g, rotation error %.2g\n", gaps[i], y, rotation_error(&q, &back));
	}
	quaternion_set_from_euler_anglesf3(&q, 0.3, -0.4, 0.7);
	quaternion_get_euler_anglesf3(&q, &x, &y, &z);
	printf("unit q:     %.17g %.17g %.17g\n", x, y, z);
	quaternion_multiplyf(quaternion_set(&scaled, &q), 2.0);
	quaternion_get_euler_anglesf3(&scaled, &x, &y, &z);
	printf("q * 2:      %.17g %.17g %.17g\n", x, y, z);
	return 0;
}
```

```text
pitch pi/2 - 0.001: pitch = 1.5697963267948967, rotation error 1.3e-16
pitch pi/2 - 1e-05: pitch = 1.5707863267948965, rotation error 1.1e-16
pitch pi/2 - 1e-07: pitch = 1.5707962267948967, rotation error 1.2e-16
unit q:     0.30000000000000004 -0.40000000000000013 0.69999999999999996
q * 2:      0.30000000000000004 -0.40000000000000013 0.69999999999999996
```

## Checking

`compare/check_reports.py docs/reports/glm/22-euler-angles-asin-near-pitch-pi-2.md` builds both
programs above and compares their output with this report.  hypatia's precision gate
(`test/precision`) measures the same in its `quaternion_get_euler_anglesf3` rows: at most
1.2 epsilon in single and double, near gimbal lock and for quaternions of length 0.1 to 10.
