# `glm::quatLookAt` returns a non-rotation when up is nearly parallel to the direction

| | |
|---|---|
| Library | GLM 1.0.1 (0af55cc) |
| Function | `glm::quatLookAtRH`, `glm::quatLookAtLH` (`gtc/quaternion.hpp`) |
| Kind | wrong result |
| Precision | double and float |
| Status | still present in GLM 1.0.3 (the latest release) and at master 6f14f47 (2026-04-07): the program prints the same |

## Summary

`quatLookAt` builds the camera's right axis as `cross(up, direction)` and divides it by
`sqrt(max(0.00001, dot(right, right)))` instead of normalizing it.  When `up` is within
about 0.003 rad of the view direction the clamp applies, the right axis is shorter than 1,
and the matrix passed to `quat_cast` is not a rotation.  For a direction 1e-4 rad from up
the result has length 0.72 and does not look along the direction.  In the comparison
harness one random input of 20000 gave an error of 5e14 epsilons.

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
	glm::dvec3 direction = glm::normalize(glm::dvec3(0, 1, 1e-4));  // 1e-4 rad from up
	glm::dvec3 up(0, 1, 0);
	glm::dquat q = glm::quatLookAtRH(direction, up);
	glm::dvec3 forward = q * glm::dvec3(0, 0, -1);  // where the camera looks

	std::printf("length of q = %.9g\n", glm::length(q));
	std::printf("looks along (%.6f, %.6f, %.6f), asked (%.6f, %.6f, %.6f)\n",
	            forward.x, forward.y, forward.z, direction.x, direction.y, direction.z);
}
```

Output (x86-64, gcc 13.3, `-O2`):

```text
length of q = 0.718200103
looks along (0.000000, 0.515811, -0.484137), asked (0.000000, 1.000000, 0.000100)
```

Expected: a unit quaternion that turns -Z into `direction`.

## Cause

[`gtc/quaternion.inl` lines 179-189](https://github.com/g-truc/glm/blob/0af55ccecd98d4e5a8d1fad7de25ba429d60e863/glm/gtc/quaternion.inl#L179-L189):

```
Result[2] = -direction;
vec<3, T, Q> const& Right = cross(up, Result[2]);
Result[0] = Right * inversesqrt(max(static_cast<T>(0.00001), dot(Right, Right)));
Result[1] = cross(Result[2], Result[0]);
return quat_cast(Result);
```

The clamp keeps the division finite when up and direction are parallel, but for any
`|Right| < 0.0032` it scales the right axis by the wrong factor, and the up axis built from
it is short too.

## How hypatia does it

`quaternion_set_look_rotation_rh` (and `_lh`) normalizes the right axis exactly (scaled
normalization, no clamp), so the matrix is a rotation for any up that is not exactly
parallel.  When up is zero or exactly parallel to the direction there is no right axis;
the result is then the shortest rotation from -Z to the direction
(`quaternion_get_rotation_tov3`), still a unit quaternion.

```c
#define HYPATIA_IMPLEMENTATION
#include <math.h>
#include <stdio.h>
#include "hypatia.h"

int main(void)
{
	struct vector3 direction;
	struct vector3 forward;
	struct quaternion q;

	vector3_setf3(&direction, 0, 1, 1e-4);
	quaternion_set_look_rotation_rh(&q, &direction, HYP_VECTOR3_UNIT_Y);
	vector3_setf3(&forward, 0, 0, -1);
	vector3_rotate_by_quaternion(&forward, &q);
	vector3_normalize(&direction);

	printf("length of q = %.9g\n", quaternion_magnitude(&q));
	printf("looks along (%.6f, %.6f, %.6f), asked (%.6f, %.6f, %.6f)\n",
	       forward.x, forward.y, forward.z, direction.x, direction.y, direction.z);
	return 0;
}
```

```text
length of q = 1
looks along (-0.000000, 1.000000, 0.000100), asked (0.000000, 1.000000, 0.000100)
```

## Suggested fix

Normalize `Right` (`Right / length(Right)`), and handle `length(Right) == 0` separately, for
example with `rotation(vec3(0, 0, -1), direction)` once that function is fixed
([01](01-rotation-opposite-vectors.md)).

## Checking

`compare/check_reports.py docs/reports/glm/05-quatlookat-up-nearly-parallel.md` builds both programs above and compares their output with this report. The harness: `results/precision/now.double.s7.md`, `quaternion_set_look_rotation_rh`
(GLM largest 5.36e14 epsilons; hypatia 18.9).
