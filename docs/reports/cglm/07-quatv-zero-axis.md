# `glm_quatv` with a zero (or short) axis returns a quaternion of length cos(angle/2)

| | |
|---|---|
| Library | cglm 0.9.4 (1796cc5) |
| Function | `glm_quatv`, `glm_quat` (`quat.h`) |
| Kind | wrong result on degenerate input |
| Precision | float |
| Status | still present in cglm 0.9.6 (the latest release) and at master 58d8c15 (2026-07-29): the program prints the same |

## Summary

`glm_quatv` normalizes the axis with `glm_normalize_to`, which gives the zero vector for
axes shorter than `FLT_EPSILON`, and returns `(0, 0, 0, cos(angle/2))`: for a quarter turn a
quaternion of length 0.707, which is not a rotation.  An axis of length 1e-8 has a direction
and gives the same result.

## Reproduction

```c
#include <cglm/cglm.h>
#include <math.h>
#include <stdio.h>

int main(void)
{
	versor q;
	vec3 zero = {0, 0, 0}, shortaxis = {0, 0, 1e-8f};

	glm_quatv(q, GLM_PI_2f, zero);
	printf("quarter turn about (0, 0, 0):    (%g, %g, %g, %g), length %g\n", q[0], q[1], q[2], q[3], glm_quat_norm(q));
	glm_quatv(q, GLM_PI_2f, shortaxis);
	printf("quarter turn about (0, 0, 1e-8): (%g, %g, %g, %g), length %g\n", q[0], q[1], q[2], q[3], glm_quat_norm(q));
	return 0;
}
```

Output (x86-64, gcc 13.3, `-O2`):

```text
quarter turn about (0, 0, 0):    (0, 0, 0, 0.707107), length 0.707107
quarter turn about (0, 0, 1e-8): (0, 0, 0, 0.707107), length 0.707107
```

Expected: the identity (0, 0, 0, 1) for the zero axis, and (0, 0, 0.707, 0.707) for the axis (0, 0, 1e-8).

## Cause

[`quat.h` lines 151-165](https://github.com/recp/cglm/blob/1796cc5ce298235b615dc7a4750b8c3ba56a05dd/include/cglm/quat.h#L151-L165): `glm_normalize_to(axis, k)` and then
`q = (s k, c)`.

## Suggested fix

Return the identity when the axis is exactly zero, and fix the normalization threshold
([04](04-normalize-zeroes-short-vectors.md)).

## How hypatia does it

`quaternion_set_from_axis_anglev3` normalizes the axis with scaling; a zero axis gives the
identity, and any other axis the unit quaternion of the rotation about it.

```c
#define HYPATIA_SINGLE_PRECISION_FLOATS
#define HYPATIA_IMPLEMENTATION
#include <float.h>
#include <math.h>
#include <stdio.h>
#include "hypatia.h"

int main(void)
{
	struct quaternion q;
	struct vector3 axis;

	quaternion_set_from_axis_anglev3(&q, HYP_VECTOR3_ZERO, HYP_PI / 2);
	printf("quarter turn about (0, 0, 0):    (%g, %g, %g, %g), length %g\n", q.x, q.y, q.z, q.w, quaternion_magnitude(&q));
	quaternion_set_from_axis_anglev3(&q, vector3_setf3(&axis, 0, 0, 1e-8f), HYP_PI / 2);
	printf("quarter turn about (0, 0, 1e-8): (%g, %g, %g, %g), length %g\n", q.x, q.y, q.z, q.w, quaternion_magnitude(&q));
	return 0;
}
```

```text
quarter turn about (0, 0, 0):    (0, 0, 0, 1), length 1
quarter turn about (0, 0, 1e-8): (0, 0, 0.707107, 0.707107), length 1
```

## Checking

`compare/check_reports.py docs/reports/cglm/07-quatv-zero-axis.md` builds both programs
above and compares their output with this report.
