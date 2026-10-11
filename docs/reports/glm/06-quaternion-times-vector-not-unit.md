# `q * v` does not rotate when q is not exactly unit length

| | |
|---|---|
| Library | GLM 1.0.1 (0af55cc) |
| Function | `operator*(qua, vec3)` (`detail/type_quat.inl`) |
| Kind | wrong result for non-unit q; precision for drifted q |
| Precision | double and float |
| Status | still present in GLM 1.0.3 (the latest release) and at master 6f14f47 (2026-04-07): the program prints the same |

## Summary

`q * v` uses `v + 2 (w (u x v) + u x (u x v))`, which is a rotation only when |q| = 1.
For any other length the result is not the rotation by q scaled, but a different linear map:
for twice the quarter turn about Z, X goes to (-3, 4, 0) instead of (0, 1, 0).
Quaternions that are composed repeatedly drift from unit length; at a drift of 1e-6 the
error is 2e-6 relative, 6e9 double epsilons in the mean over random inputs.
`glm::mat3_cast` makes the same assumption and gives the same (-3, 4, 0).

## Reproduction

```cpp
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <cmath>
#include <cstdio>

int main()
{
	glm::dquat quarter = glm::angleAxis(glm::pi<double>() / 2, glm::dvec3(0, 0, 1));
	glm::dquat twice = quarter * 2.0;
	glm::dvec3 r = twice * glm::dvec3(1, 0, 0);
	glm::dvec3 m = glm::mat3_cast(twice) * glm::dvec3(1, 0, 0);
	std::printf("2q * X            = (%g, %g, %g)\n", r.x, r.y, r.z);
	std::printf("mat3_cast(2q) * X = (%g, %g, %g)\n", m.x, m.y, m.z);

	glm::dquat drifted = quarter * (1 + 1e-6);
	glm::dvec3 d = drifted * glm::dvec3(1, 0, 0);
	std::printf("q (length 1 + 1e-6) * X = (%.17g, %.17g, %g), length %.17g\n", d.x, d.y, d.z, glm::length(d));
}
```

Output (x86-64, gcc 13.3, `-O2`):

```text
2q * X            = (-3, 4, 0)
mat3_cast(2q) * X = (-3, 4, 0)
q (length 1 + 1e-6) * X = (-2.0000009997023227e-06, 1.0000020000009999, 0), length 1.0000020000029999
```

Expected: (0, 1, 0) in every case: the rotation by the quaternion, whatever its length.

## Cause

[`detail/type_quat.inl` lines 359-366](https://github.com/g-truc/glm/blob/0af55ccecd98d4e5a8d1fad7de25ba429d60e863/glm/detail/type_quat.inl#L359-L366):

```
vec<3, T, Q> const QuatVector(q.x, q.y, q.z);
vec<3, T, Q> const uv(glm::cross(QuatVector, v));
vec<3, T, Q> const uuv(glm::cross(QuatVector, uv));
return v + ((uv * q.w) + uuv) * static_cast<T>(2);
```

This is `q v q*` expanded with `|q| = 1` substituted.  Without that substitution,
`q v q*` is `|q|^2` times the rotation; the formula is neither.

## Suggested fix

Divide by `dot(q, q)`: with `u` the vector part,
`(2 dot(u, v) u + (w*w - dot(u, u)) v + 2 w cross(u, v)) / dot(q, q)`.  It costs one
division and is the rotation for any non-zero q.  If the unit-length requirement is
intended, document it on `operator*` and assert it in debug builds.

## How hypatia does it

`vector3_rotate_by_quaternion` evaluates `(2 (u . v) u + (w^2 - u . u) v + 2 w (u x v)) /
|q|^2`, which is the rotation by `q / |q|` for any length of q, with no extra
normalization step.  When |q|^2 is not between 1e-30 and 1e30 it normalizes q first; the
zero quaternion leaves the vector unchanged.  It agrees with
`matrix4_set_from_quaternion`.

```c
#define HYPATIA_IMPLEMENTATION
#include <math.h>
#include <stdio.h>
#include "hypatia.h"

int main(void)
{
	struct quaternion quarter;
	struct quaternion q;
	struct vector3 v;

	quaternion_set_from_axis_anglev3(&quarter, HYP_VECTOR3_UNIT_Z, HYP_PI / 2);

	quaternion_multiplyf(quaternion_set(&q, &quarter), 2);
	vector3_rotate_by_quaternion(vector3_setf3(&v, 1, 0, 0), &q);
	printf("2q * X = (%g, %g, %g)\n", v.x, v.y, v.z);

	quaternion_multiplyf(quaternion_set(&q, &quarter), 1 + 1e-6);
	vector3_rotate_by_quaternion(vector3_setf3(&v, 1, 0, 0), &q);
	printf("q (length 1 + 1e-6) * X = (%.17g, %.17g, %g), length %.17g\n", v.x, v.y, v.z, vector3_magnitude(&v));
	return 0;
}
```

```text
2q * X = (2.22045e-16, 1, 0)
q (length 1 + 1e-6) * X = (1.1102208041824381e-16, 1, 0), length 1
```

## Checking

`compare/check_reports.py docs/reports/glm/06-quaternion-times-vector-not-unit.md` builds
both programs above and compares their output with this report.  The harness:
`compare/results/precision/now.double.md`, `vector3_rotate_by_quaternion`, "q of length 1
+- 1e-6" (GLM 6.05e9 epsilons in the mean, hypatia 0.66) and "unit q" (GLM 0.69, hypatia
0.62).
