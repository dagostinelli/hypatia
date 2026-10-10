# `glm::rotation` with a zero vector is NaN

| | |
|---|---|
| Library | GLM 1.0.1 (0af55cc) |
| Function | `glm::rotation(orig, dest)` (`gtx/quaternion.hpp`) |
| Kind | NaN on degenerate input |
| Precision | double and float |
| Status | still present in GLM 1.0.3 (the latest release) and at master 6f14f47 (2026-04-07): the program prints the same |

## Summary

`glm::rotation` takes unit vectors; the usual call is `rotation(normalize(a),
normalize(b))`, which gives NaN when either vector is zero (`normalize` returns NaN, see
[09](09-normalize-zero-vector.md)).  Called directly with a zero vector it divides
`cross = 0` by `sqrt(2 (1 + 0))` and returns the non-unit quaternion (w 0.71, x 0, y 0, z 0).  A
zero direction has no rotation to it; the identity is the defined answer.

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
	glm::dvec3 a(1, 2, 3), zero(0, 0, 0);
	glm::dquat n = glm::rotation(glm::normalize(a), glm::normalize(zero));
	glm::dquat d = glm::rotation(glm::normalize(a), zero);
	std::printf("rotation(normalize(a), normalize(0)) = (w %g, x %g, y %g, z %g)\n", n.w, n.x, n.y, n.z);
	std::printf("rotation(normalize(a), 0)            = (w %g, x %g, y %g, z %g), length %g\n", d.w, d.x, d.y, d.z, glm::length(d));
}
```

Output (x86-64, gcc 13.3, `-O2`):

```text
rotation(normalize(a), normalize(0)) = (w -nan, x -nan, y -nan, z -nan)
rotation(normalize(a), 0)            = (w 0.707107, x 0, y 0, z 0), length 0.707107
```

Expected: the identity, or a documented error.

## Cause

[`gtx/quaternion.inl` lines 148-157](https://github.com/g-truc/glm/blob/0af55ccecd98d4e5a8d1fad7de25ba429d60e863/glm/gtx/quaternion.inl#L148-L157): with `dest = 0`,
`cosTheta = 0` and the general formula returns w = sqrt(2)/2 with a zero axis.

## How hypatia does it

`quaternion_get_rotation_tov3` normalizes both vectors itself; if either has zero length
there is no direction to rotate, and it returns the identity.

```c
#define HYPATIA_IMPLEMENTATION
#include <math.h>
#include <stdio.h>
#include "hypatia.h"

int main(void)
{
	struct vector3 a;
	struct quaternion q;

	vector3_setf3(&a, 1, 2, 3);
	quaternion_get_rotation_tov3(&a, HYP_VECTOR3_ZERO, &q);
	printf("get_rotation_tov3(a, 0) = (w %g, x %g, y %g, z %g)\n", q.w, q.x, q.y, q.z);
	return 0;
}
```

```text
get_rotation_tov3(a, 0) = (w 1, x 0, y 0, z 0)
```

## Suggested fix

Return the identity when either length is zero; with the accurate formula of
[01](01-rotation-opposite-vectors.md) this falls out of the normalization step.

## Checking

`compare/check_reports.py docs/reports/glm/14-rotation-zero-vector.md` builds both programs above and compares their output with this report.
