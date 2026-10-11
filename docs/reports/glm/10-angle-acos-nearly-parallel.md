# `glm::angle(x, y)` loses its digits for nearly parallel and nearly opposite vectors

| | |
|---|---|
| Library | GLM 1.0.1 (0af55cc) |
| Function | `glm::angle(vec, vec)` (`gtx/vector_angle.hpp`) |
| Kind | precision |
| Precision | double and float |
| Status | still present in GLM 1.0.3 (the latest release) and at master 6f14f47 (2026-04-07): the program prints the same |

## Summary

`glm::angle` is `acos(clamp(dot(x, y), -1, 1))`.  Near 0 and pi the derivative of acos is
infinite.  Just below 1 the dot product is rounded to within half an ulp, eps/4, and for
an angle a near 0 that becomes an error of up to about eps/(4 a) in the angle: 5.6e-11 at
1e-6 rad in double, where the program below shows 4.4e-11 (a relative error of 4.4e-5).
Below about 1.05e-8 rad (2^-26.5) the cosine rounds to 1 and the angle is 0.  Vectors the
same distance from opposite give the same errors near pi.  In float the threshold is about
2.44e-4 rad (2^-12): vectors 2e-4 rad apart give 0.

## Reproduction

```cpp
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/vector_angle.hpp>
#include <cmath>
#include <cstdio>

int main()
{
	double angles[3] = {1e-3, 1e-6, 1e-8};
	long double pi = 3.14159265358979323846264338L;
	glm::dvec3 x(1, 0, 0);
	for (double a : angles) {
		double g = glm::angle(x, glm::dvec3(std::cos(a), std::sin(a), 0));
		std::printf("vectors %g rad apart: glm::angle = %.17g, relative error %.2g\n", a, g, std::fabs(g - a) / a);
	}
	for (double a : angles) {
		double g = glm::angle(x, glm::dvec3(-std::cos(a), std::sin(a), 0));
		std::printf("vectors %g rad from opposite: glm::angle = %.17g, error %.2Lg\n", a, g, fabsl(g - (pi - a)));
	}
	float af = 2e-4f;
	std::printf("float, vectors 2e-4 rad apart: glm::angle = %g\n",
	            glm::angle(glm::vec3(1, 0, 0), glm::vec3(std::cos(af), std::sin(af), 0)));
}
```

Output (x86-64, gcc 13.3, `-O2`):

```text
vectors 0.001 rad apart: glm::angle = 0.00099999999999216861, relative error 7.8e-12
vectors 1e-06 rad apart: glm::angle = 1.0000444493033419e-06, relative error 4.4e-05
vectors 1e-08 rad apart: glm::angle = 0, relative error 1
vectors 0.001 rad from opposite: glm::angle = 3.1405926535898012, error 8e-15
vectors 1e-06 rad from opposite: glm::angle = 3.1415916535453441, error 4.4e-11
vectors 1e-08 rad from opposite: glm::angle = 3.1415926535897931, error 1e-08
float, vectors 2e-4 rad apart: glm::angle = 0
```

Expected: a relative error of about 1e-16 near 0, and an error of about 1e-16 near pi.

## Cause

[`gtx/vector_angle.inl` line 20](https://github.com/g-truc/glm/blob/0af55ccecd98d4e5a8d1fad7de25ba429d60e863/glm/gtx/vector_angle.inl#L20):
`return acos(clamp(dot(x, y), T(-1), T(1)));`.  The cosine of a small angle a is
1 - a^2/2; the dot product carries it with an absolute error of up to eps/4, which leaves
a^2 with an absolute error of up to eps/2.

## Suggested fix

`return atan2(length(cross(x, y)), dot(x, y));` for 3D (and `atan2(abs(x.x y.y - x.y y.x),
dot(x, y))` for 2D).  It needs no clamp, and it is scale-invariant: x and y need not be
unit vectors.  Normalizing them first only guards against overflow and underflow in the
cross and dot products.

## How hypatia does it

`vector3_angle_between` (and `vector2_angle_between`) computes `atan2(|a x b|, a . b)` of
the unit vectors.  The cross product carries the sine, which is accurate for small angles,
and the dot product the cosine, accurate near pi/2; atan2 uses both.  The vectors do not
need to be unit length.

```c
#define HYPATIA_IMPLEMENTATION
#include <math.h>
#include <stdio.h>
#include "hypatia.h"

int main(void)
{
	double angles[3] = {1e-3, 1e-6, 1e-8};
	long double pi = 3.14159265358979323846264338L;
	struct vector3 y;
	double h;
	int i;
	for (i = 0; i < 3; i++) {
		h = vector3_angle_between(HYP_VECTOR3_UNIT_X, vector3_setf3(&y, cos(angles[i]), sin(angles[i]), 0));
		printf("vectors %g rad apart: vector3_angle_between = %.17g, relative error %.2g\n", angles[i], h, fabs(h - angles[i]) / angles[i]);
	}
	for (i = 0; i < 3; i++) {
		h = vector3_angle_between(HYP_VECTOR3_UNIT_X, vector3_setf3(&y, -cos(angles[i]), sin(angles[i]), 0));
		printf("vectors %g rad from opposite: vector3_angle_between = %.17g, error %.2Lg\n", angles[i], h, fabsl(h - (pi - angles[i])));
	}
	return 0;
}
```

```text
vectors 0.001 rad apart: vector3_angle_between = 0.001, relative error 0
vectors 1e-06 rad apart: vector3_angle_between = 1.0000000000000002e-06, relative error 2.1e-16
vectors 1e-08 rad apart: vector3_angle_between = 1e-08, relative error 0
vectors 0.001 rad from opposite: vector3_angle_between = 3.1405926535897932, error 1.2e-17
vectors 1e-06 rad from opposite: vector3_angle_between = 3.1415916535897934, error 1.8e-16
vectors 1e-08 rad from opposite: vector3_angle_between = 3.1415926435897932, error 6.2e-17
```

## Checking

`compare/check_reports.py docs/reports/glm/10-angle-acos-nearly-parallel.md` builds both
programs above and compares their output with this report.  The harness:
`compare/results/double.md`, "accuracy against long double", `glm::angle` rows (up to
4.5e-13 absolute in 3D and 8.8e-12 in 2D for vectors 1e-3 rad apart or from opposite;
hypatia 4.4e-16).
