# `glm::mix` of q and -q returns the zero quaternion

| | |
|---|---|
| Library | GLM 1.0.1 (0af55cc) |
| Function | `glm::mix(qua, qua, a)` (`ext/quaternion_common.hpp`) |
| Kind | wrong result |
| Precision | double and float |
| Status | still present in GLM 1.0.3 (the latest release) and at master 6f14f47 (2026-04-07): the program prints the same |

## Summary

`glm::mix` interpolates on the 4D sphere without taking the shorter arc.  For `q` and `-q`
(the same rotation) the dot product is -1, the angle is `acos(-1)`, the double nearest pi,
and `sin(angle)` is 1.2e-16 (the program below prints both).  Halfway, the numerator
`sin(angle / 2) x + sin(angle / 2) y` cancels to 0 because `y = -x`: the result is the
zero quaternion, which is not a rotation.

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
	glm::dquat q = glm::angleAxis(1.0, glm::dvec3(0, 0, 1));
	double c = glm::dot(q, -q);
	std::printf("dot(q, -q) = %.17g, sin(acos(dot)) = %g\n", c, std::sin(std::acos(c)));
	glm::dquat r = glm::mix(q, -q, 0.5);
	std::printf("mix(q, -q, 0.5) = (w %g, x %g, y %g, z %g), length %g\n", r.w, r.x, r.y, r.z, glm::length(r));

	glm::dquat s = glm::slerp(q, -q, 0.5);
	std::printf("slerp(q, -q, 0.5) = (w %.9g, x %g, y %g, z %.9g)\n", s.w, s.x, s.y, s.z);
}
```

Output (x86-64, gcc 13.3, `-O2`):

```text
dot(q, -q) = -1, sin(acos(dot)) = 1.22465e-16
mix(q, -q, 0.5) = (w 0, x 0, y 0, z 0), length 0
slerp(q, -q, 0.5) = (w 0.877582562, x 0, y 0, z 0.479425539)
```

Expected: q or -q: both ends are the same rotation.  `glm::slerp` gets this right.

## Cause

[`ext/quaternion_common.inl` lines 4-26](https://github.com/g-truc/glm/blob/0af55ccecd98d4e5a8d1fad7de25ba429d60e863/glm/ext/quaternion_common.inl#L4-L26): only
`cosTheta > 1 - epsilon` is special-cased; `cosTheta` near -1 goes to
`(sin((1 - a) angle) x + sin(a angle) y) / sin(angle)` with `angle = acos(cosTheta)`
close to pi.  The documentation says `mix` does not take the shortest path, but the
result at a = 0.5 for opposite inputs is not a unit quaternion at all.

## How hypatia does it

`quaternion_slerp` always takes the shorter arc (it moves toward `-end` when the dot
product is negative), and measures the angle as `2 atan2(|s - t|, |s + t|)` of the unit
quaternions, which is accurate for every angle.  It divides by `sin(theta)` for every
nonzero angle; that is safe because theta is accurate.  For `q` and `-q` the sign flip
makes the target equal to `q`, theta is exactly 0, and the lerp branch runs.
`quaternion_slerp` has no long-arc interpolation.

```c
#define HYPATIA_IMPLEMENTATION
#include <math.h>
#include <stdio.h>
#include "hypatia.h"

int main(void)
{
	struct quaternion q;
	struct quaternion minus;
	struct quaternion r;

	quaternion_set_from_axis_anglev3(&q, HYP_VECTOR3_UNIT_Z, 1);
	quaternion_negate(quaternion_set(&minus, &q));
	quaternion_slerp(&q, &minus, 0.5, &r);
	printf("slerp(q, -q, 0.5) = (w %.9g, x %g, y %g, z %.9g), length %g\n", r.w, r.x, r.y, r.z, quaternion_magnitude(&r));
	return 0;
}
```

```text
slerp(q, -q, 0.5) = (w 0.877582562, x 0, y 0, z 0.479425539), length 1
```

## Suggested fix

Special-case `cosTheta < -1 + epsilon` (the quaternions are the same rotation; return `x`,
or rotate about any axis perpendicular in 4D if a long-arc path is wanted), or point users
of `mix` to `slerp`.  At least document that `mix` of opposite quaternions returns zero.

## Checking

`compare/check_reports.py docs/reports/glm/07-mix-opposite-quaternions.md` builds both programs above and compares their output with this report.
