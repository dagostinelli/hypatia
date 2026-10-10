# `glm::angleAxis` and `glm::rotate` disagree on axes that are not unit length, and fail on a zero axis

| | |
|---|---|
| Library | GLM 1.0.1 (0af55cc) |
| Function | `glm::angleAxis(angle, axis)` (`ext/quaternion_trigonometric.hpp`), `glm::rotate(m, angle, axis)` (`ext/matrix_transform.hpp`) |
| Kind | inconsistent API; NaN on degenerate input |
| Precision | double and float |
| Status | still present in GLM 1.0.3 (the latest release) and at master 6f14f47 (2026-04-07): the program prints the same |

## Summary

`glm::rotate(m, angle, axis)` normalizes the axis; `glm::angleAxis(angle, axis)` does not.
The same call with the axis (0, 0, 10) gives a matrix that turns X into Y and a quaternion
of length 7.1 that turns X into (-99, 10, 0).  `angleAxis` documents that the axis must be
normalized, but nothing checks it, and the matrix function suggests that it does not need
to be.  For the zero axis, `rotate` returns NaN (`normalize` of zero) and `angleAxis`
returns the non-unit (w 0.71, x 0, y 0, z 0).

## Reproduction

```cpp
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <cmath>
#include <cstdio>

#include <glm/ext/matrix_transform.hpp>

int main()
{
	double quarter = glm::pi<double>() / 2;
	glm::dvec3 axis(0, 0, 10), x(1, 0, 0);
	glm::dvec4 m = glm::rotate(glm::dmat4(1), quarter, axis) * glm::dvec4(x, 0);
	glm::dquat q = glm::angleAxis(quarter, axis);
	glm::dvec3 r = q * x;
	std::printf("rotate(quarter, (0, 0, 10)) * X    = (%.3g, %.3g, %.3g)\n", m.x, m.y, m.z);
	std::printf("angleAxis(quarter, (0, 0, 10)) * X = (%.3g, %.3g, %.3g), |q| = %.3g\n", r.x, r.y, r.z, glm::length(q));

	glm::dvec4 mz = glm::rotate(glm::dmat4(1), quarter, glm::dvec3(0, 0, 0)) * glm::dvec4(x, 0);
	glm::dquat qz = glm::angleAxis(quarter, glm::dvec3(0, 0, 0));
	std::printf("rotate(quarter, 0) * X = (%g, %g, %g); angleAxis(quarter, 0) = (w %.3g, x %g, y %g, z %g)\n",
	            mz.x, mz.y, mz.z, qz.w, qz.x, qz.y, qz.z);
}
```

Output (x86-64, gcc 13.3, `-O2`):

```text
rotate(quarter, (0, 0, 10)) * X    = (6.12e-17, 1, 0)
angleAxis(quarter, (0, 0, 10)) * X = (-99, 10, 0), |q| = 7.11
rotate(quarter, 0) * X = (-nan, -nan, -nan); angleAxis(quarter, 0) = (w 0.707, x 0, y 0, z 0)
```

Expected: the same rotation from both functions, and a defined result (the identity) for the zero axis.

## Cause

[`ext/quaternion_trigonometric.inl` lines 30-36](https://github.com/g-truc/glm/blob/0af55ccecd98d4e5a8d1fad7de25ba429d60e863/glm/ext/quaternion_trigonometric.inl#L30-L36)
uses the axis as given: `qua(cos(a/2), v * sin(a/2))`;
[`ext/matrix_transform.inl` line 24](https://github.com/g-truc/glm/blob/0af55ccecd98d4e5a8d1fad7de25ba429d60e863/glm/ext/matrix_transform.inl#L24) normalizes it.

## How hypatia does it

`quaternion_set_from_axis_anglev3` and `matrix4_set_from_axisv3_angle` both normalize the
axis (scaled, without overflow) and give the same rotation for any length; a zero axis
gives the identity in both.

```c
#define HYPATIA_IMPLEMENTATION
#include <math.h>
#include <stdio.h>
#include "hypatia.h"

int main(void)
{
	struct vector3 axis;
	struct vector3 v;
	struct quaternion q;
	struct matrix4 m;

	vector3_setf3(&axis, 0, 0, 10);
	matrix4_set_from_axisv3_angle(&m, &axis, HYP_PI / 2);
	matrix4_multiplyv3(&m, HYP_VECTOR3_UNIT_X, &v);
	printf("matrix4_set_from_axisv3_angle(quarter, (0, 0, 10)) * X = (%.3g, %.3g, %.3g)\n", v.x, v.y, v.z);
	quaternion_set_from_axis_anglev3(&q, &axis, HYP_PI / 2);
	vector3_rotate_by_quaternion(vector3_setf3(&v, 1, 0, 0), &q);
	printf("quaternion_set_from_axis_anglev3(quarter, (0, 0, 10)) * X = (%.3g, %.3g, %.3g), |q| = %.3g\n", v.x, v.y, v.z, quaternion_magnitude(&q));
	quaternion_set_from_axis_anglev3(&q, HYP_VECTOR3_ZERO, HYP_PI / 2);
	printf("quaternion_set_from_axis_anglev3(quarter, 0) = (w %g, x %g, y %g, z %g)\n", q.w, q.x, q.y, q.z);
	return 0;
}
```

```text
matrix4_set_from_axisv3_angle(quarter, (0, 0, 10)) * X = (6.12e-17, 1, 0)
quaternion_set_from_axis_anglev3(quarter, (0, 0, 10)) * X = (2.22e-16, 1, 0), |q| = 1
quaternion_set_from_axis_anglev3(quarter, 0) = (w 1, x 0, y 0, z 0)
```

## Suggested fix

Normalize the axis in `angleAxis` as `rotate` does (one inverse square root), and return
the identity for the zero axis in both.  Or assert `length(axis) == 1` in debug builds.

## Checking

`compare/check_reports.py docs/reports/glm/15-axis-angle-zero-or-not-unit-axis.md` builds both programs above and compares their output with this report.
