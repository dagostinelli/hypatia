# `glm::axis` returns an axis that is not unit length for small rotations

| | |
|---|---|
| Library | GLM 1.0.1 (0af55cc) |
| Function | `glm::axis(q)` (`ext/quaternion_trigonometric.hpp`) |
| Kind | precision |
| Precision | double and float |
| Status | still present in GLM 1.0.3 (the latest release) and at master 6f14f47 (2026-04-07): the program prints the same |

## Summary

`glm::axis` divides the vector part of the quaternion by `sqrt(1 - w*w)`.  For a small
rotation `w` is close to 1, `1 - w*w` cancels, and the axis comes out with the wrong
length: 1.0000000026 for a rotation of 1e-4 rad in double, an error of 1.2e7 epsilons.  In
float `w` rounds to exactly 1, `1 - w*w` is 0, and `glm::axis` returns its fallback
(0, 0, 1) whatever the actual axis: for a rotation of 1e-4 rad about X it returns Z.
`axis * angle` (the rotation vector) inherits the error: 1.2e7 double epsilons in the mean
over random rotations of 1e-4 rad.  On random rotations of any size the largest error is 70
epsilons (mean 0.65), against 1.33 (mean 0.38) with hypatia.  `glm::angle` itself is accurate here (it switches to
asin), so the error is only in the axis.

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
	glm::dquat q = glm::angleAxis(1e-4, glm::dvec3(0, 0, 1));
	glm::dvec3 axis = glm::axis(q);
	std::printf("double: length of glm::axis = %.17g\n", glm::length(axis));

	glm::quat qf = glm::angleAxis(1e-4f, glm::vec3(1, 0, 0));
	glm::vec3 af = glm::axis(qf);
	std::printf("float, 1e-4 rad about X: glm::axis = (%g, %g, %g)\n", af.x, af.y, af.z);
}
```

Output (x86-64, gcc 13.3, `-O2`):

```text
double: length of glm::axis = 1.000000002622069
float, 1e-4 rad about X: glm::axis = (0, 0, 1)
```

Expected: a unit axis, (0, 0, 1) and (1, 0, 0).

## Cause

[`ext/quaternion_trigonometric.inl` lines 20-27](https://github.com/g-truc/glm/blob/0af55ccecd98d4e5a8d1fad7de25ba429d60e863/glm/ext/quaternion_trigonometric.inl#L20-L27):

```
T const tmp1 = static_cast<T>(1) - x.w * x.w;
...
T const tmp2 = static_cast<T>(1) / sqrt(tmp1);
return vec<3, T, Q>(x.x * tmp2, x.y * tmp2, x.z * tmp2);
```

For a rotation by a, `w = cos(a/2)` and `1 - w*w = sin(a/2)^2`; computed from `w` it keeps
only the digits of `w*w` that differ from 1, about half of them for a = 1e-4 in double and
none in float.

## How hypatia does it

`quaternion_get_axis_anglev3` normalizes the vector part directly (its length is
|q| sin(a/2), so no subtraction is involved) and takes the angle as `2 atan2(|v|, w)`,
which is accurate for every angle.  The quaternion does not need to be unit length.

```c
#define HYPATIA_IMPLEMENTATION
#include <math.h>
#include <stdio.h>
#include "hypatia.h"

int main(void)
{
	struct quaternion q;
	struct vector3 axis;
	HYP_FLOAT angle;

	quaternion_set_from_axis_anglev3(&q, HYP_VECTOR3_UNIT_Z, 1e-4);
	quaternion_get_axis_anglev3(&q, &axis, &angle);
	printf("length of the axis = %.17g, angle = %.17g\n", vector3_magnitude(&axis), angle);
	return 0;
}
```

```text
length of the axis = 1, angle = 0.0001
```

## Suggested fix

Normalize the vector part instead of dividing by `sqrt(1 - w*w)`:
`return v / length(v)` with `v = vec3(x.x, x.y, x.z)` (and the existing fallback for a zero
vector part).

## Checking

`compare/check_reports.py docs/reports/glm/04-axis-small-rotations.md` builds both programs above and compares their output with this report. The harness measures `axis * angle` over 20000 inputs
(`results/precision/now.double.md`, `quaternion_get_axis_anglev3`, "angle 1e-4": GLM
1.18e7 epsilons, hypatia 0.99).
