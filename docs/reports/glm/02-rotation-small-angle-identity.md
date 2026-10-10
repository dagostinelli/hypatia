# `glm::rotation` returns the identity for vectors less than sqrt(2 epsilon) apart

| | |
|---|---|
| Library | GLM 1.0.1 (0af55cc) |
| Function | `glm::rotation(orig, dest)` (`gtx/quaternion.hpp`) |
| Kind | wrong result (accuracy) |
| Precision | float (and double, below 2.1e-8 rad) |
| Status | still present in GLM 1.0.3 (the latest release) and at master 6f14f47 (2026-04-07): the program prints the same |

## Summary

`glm::rotation` returns the identity whenever `dot(orig, dest) >= 1 - epsilon`.  For unit
vectors that is every pair less than sqrt(2 epsilon) apart: 4.9e-4 rad (0.028 degrees) in
float, 2.1e-8 rad in double.  The rotation it should return is perfectly representable;
the result is off by the whole angle: about 4000 float epsilons for vectors 4.8e-4 rad
apart.

## Reproduction

```cpp
// glm::rotation for two vectors 4.8e-4 rad apart, in float
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <cmath>
#include <cstdio>

int main()
{
	float a = 4.8e-4f;
	glm::vec3 from(1, 0, 0);
	glm::vec3 to(std::cos(a), std::sin(a), 0);
	glm::quat q = glm::rotation(from, to);
	glm::vec3 r = q * from;

	std::printf("q = (w %.9g, x %g, y %g, z %.9g)\n", q.w, q.x, q.y, q.z);
	std::printf("q * from = (%.9g, %.9g, %g)\n", r.x, r.y, r.z);
	std::printf("to       = (%.9g, %.9g, %g)\n", to.x, to.y, to.z);
	std::printf("angle missed: %.3g rad = %.0f epsilons\n", std::atan2(to.y, to.x) - std::atan2(r.y, r.x),
	            (std::atan2(to.y, to.x) - std::atan2(r.y, r.x)) / 1.1920929e-7);
}
```

Output (x86-64, gcc 13.3, `-O2`):

```text
q = (w 1, x 0, y 0, z 0)
q * from = (1, 0, 0)
to       = (0.999999881, 0.000479999959, 0)
angle missed: 0.00048 rad = 4027 epsilons
```

Expected: q = (cos(a/2), 0, 0, sin(a/2)), with z = 0.00024, which turns `from` onto `to`.

## Cause

[`gtx/quaternion.inl` lines 127-130](https://github.com/g-truc/glm/blob/0af55ccecd98d4e5a8d1fad7de25ba429d60e863/glm/gtx/quaternion.inl#L127-L130):

```
if(cosTheta >= static_cast<T>(1) - epsilon<T>()) {
	// orig and dest point in the same direction
	return quat_identity<T,Q>();
}
```

cos(a) >= 1 - epsilon holds for a up to sqrt(2 epsilon).  The test is there to avoid a
division by zero further down, but it treats small rotations as no rotation.

## How hypatia does it

`quaternion_get_rotation_tov3` normalizes both vectors and takes the half angle from two
lengths that stay accurate for any angle: for unit vectors at angle a,
|f + t| = 2 cos(a/2) and |f - t| = 2 sin(a/2).  The axis is f x (f + t), which has the
direction of f x t but stays accurate as the vectors become opposite.  There is no
threshold: only an exactly zero axis (vectors exactly parallel or opposite after rounding)
is a special case.  The result is normalized, so it is always a unit quaternion.

```c
#define HYPATIA_SINGLE_PRECISION_FLOATS
#define HYPATIA_IMPLEMENTATION
#include <math.h>
#include <stdio.h>
#include "hypatia.h"

int main(void)
{
	float a = 4.8e-4f;
	struct vector3 from;
	struct vector3 to;
	struct vector3 r;
	struct quaternion q;

	vector3_setf3(&from, 1, 0, 0);
	vector3_setf3(&to, cosf(a), sinf(a), 0);
	quaternion_get_rotation_tov3(&from, &to, &q);
	vector3_rotate_by_quaternion(vector3_set(&r, &from), &q);

	printf("q = (w %.9g, x %g, y %g, z %.9g)\n", q.w, q.x, q.y, q.z);
	printf("q * from = (%.9g, %.9g, %g)\n", r.x, r.y, r.z);
	printf("angle missed: %.3g rad\n", atan2(to.y, to.x) - atan2(r.y, r.x));
	return 0;
}
```

```text
q = (w 1, x 0, y 0, z 0.000240000008)
q * from = (0.99999994, 0.000480000017, 0)
angle missed: -2.96e-11 rad
```

## Suggested fix

Remove the threshold.  The formula below it only divides by `sqrt(2 (1 + cosTheta))`,
which is not zero for vectors in the same direction; the cross product is then zero and
the result is the identity anyway.  The accurate form in
[01-rotation-opposite-vectors.md](01-rotation-opposite-vectors.md) has no division at all.

## Checking

`compare/check_reports.py docs/reports/glm/02-rotation-small-angle-identity.md` builds both programs above and compares their output with this report. The harness measures it over 20000 inputs (`results/precision/now.single.md`,
`quaternion_get_rotation_tov3`, "1e-6 rad apart": GLM 8.39 ulps in the mean, every input
off by the whole angle; hypatia 0.26).
