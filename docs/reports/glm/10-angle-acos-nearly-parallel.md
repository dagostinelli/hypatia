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
infinite, so the rounding of the dot product (one epsilon) becomes an error of about
about epsilon / angle in the angle: for unit vectors 1e-6 rad apart the result is off by
4.4e-11 (a relative error of 4.4e-5), and below 1.5e-8 rad apart the dot product rounds to
1 and the angle is 0.  In float the threshold is 4.9e-4 rad.

## Reproduction

```cpp
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <cmath>
#include <cstdio>

#include <glm/gtx/vector_angle.hpp>

int main()
{
	double angles[3] = {1e-3, 1e-6, 1e-8};
	for (double a : angles) {
		glm::dvec3 x(1, 0, 0);
		glm::dvec3 y(std::cos(a), std::sin(a), 0);
		double g = glm::angle(x, y);
		std::printf("vectors %g rad apart: glm::angle = %.17g, relative error %.2g\n", a, g, std::fabs(g - a) / a);
	}
}
```

Output (x86-64, gcc 13.3, `-O2`):

```text
vectors 0.001 rad apart: glm::angle = 0.00099999999999216861, relative error 7.8e-12
vectors 1e-06 rad apart: glm::angle = 1.0000444493033419e-06, relative error 4.4e-05
vectors 1e-08 rad apart: glm::angle = 0, relative error 1
```

Expected: the angle to within a few epsilons (relative error about 1e-16).

## Cause

[`gtx/vector_angle.inl` line 20](https://github.com/g-truc/glm/blob/0af55ccecd98d4e5a8d1fad7de25ba429d60e863/glm/gtx/vector_angle.inl#L20):
`return acos(clamp(dot(x, y), T(-1), T(1)));`.  The cosine of a small angle a is
1 - a^2/2; the dot product can only carry it to one epsilon, which leaves a^2 with an
absolute error of 2 epsilon.

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
	int i;
	for (i = 0; i < 3; i++) {
		struct vector3 x;
		struct vector3 y;
		double a = angles[i];
		double h;
		vector3_setf3(&x, 1, 0, 0);
		vector3_setf3(&y, cos(a), sin(a), 0);
		h = vector3_angle_between(&x, &y);
		printf("vectors %g rad apart: vector3_angle_between = %.17g, relative error %.2g\n", a, h, fabs(h - a) / a);
	}
	return 0;
}
```

```text
vectors 0.001 rad apart: vector3_angle_between = 0.001, relative error 0
vectors 1e-06 rad apart: vector3_angle_between = 1.0000000000000002e-06, relative error 2.1e-16
vectors 1e-08 rad apart: vector3_angle_between = 1e-08, relative error 0
```

## Suggested fix

`return atan2(length(cross(x, y)), dot(x, y));` for 3D (and `atan2(abs(x.x y.y - x.y y.x),
dot(x, y))` for 2D).  It needs no clamp and does not require unit vectors when both are
normalized first.

## Checking

`compare/check_reports.py docs/reports/glm/10-angle-acos-nearly-parallel.md` builds both programs above and compares their output with this report. The harness: `results/double.md`, "accuracy against long double", `glm::angle`
rows (up to 4.5e-13 absolute in 3D and 8.8e-12 in 2D for vectors 1e-3 rad apart or from
opposite; hypatia 4.4e-16).
