# `glm_rotate_make` with a zero (or short) axis returns a matrix that is not a rotation

| | |
|---|---|
| Library | cglm 0.9.4 (1796cc5) |
| Function | `glm_rotate_make`, `glm_rotate` (`affine.h`) |
| Kind | wrong result on degenerate input |
| Precision | float |
| Status | still present in cglm 0.9.6 (the latest release) and at master 58d8c15 (2026-07-29): the program prints the same |

## Summary

`glm_rotate_make` normalizes the axis with `glm_vec3_normalize_to`, which returns the zero
vector for any axis shorter than `FLT_EPSILON` ([04](04-normalize-zeroes-short-vectors.md)).
With a zero axis the matrix is `cos(angle)` times the identity: for a quarter turn it maps
every vector to (almost) zero.  An axis of length 1e-8 (for example a cross product of two
nearly parallel vectors) has a direction but gives the same scaled identity.

## Reproduction

```c
#include <cglm/cglm.h>
#include <math.h>
#include <stdio.h>

int main(void)
{
	mat4 m;
	vec3 zero = {0, 0, 0}, shortaxis = {0, 0, 1e-8f}, x = {1, 0, 0}, r;

	glm_rotate_make(m, GLM_PI_2f, zero);
	glm_mat4_mulv3(m, x, 0, r);
	printf("quarter turn about (0, 0, 0):    X -> (%g, %g, %g)\n", r[0], r[1], r[2]);
	glm_rotate_make(m, GLM_PI_2f, shortaxis);
	glm_mat4_mulv3(m, x, 0, r);
	printf("quarter turn about (0, 0, 1e-8): X -> (%g, %g, %g)\n", r[0], r[1], r[2]);
	return 0;
}
```

Output (x86-64, gcc 13.3, `-O2`):

```text
quarter turn about (0, 0, 0):    X -> (-4.37114e-08, 0, 0)
quarter turn about (0, 0, 1e-8): X -> (-4.37114e-08, 0, 0)
```

Expected: X unchanged for the zero axis (no rotation), and (0, 1, 0) for the axis (0, 0, 1e-8).

## Cause

[`affine.h` lines 128-148](https://github.com/recp/cglm/blob/1796cc5ce298235b615dc7a4750b8c3ba56a05dd/include/cglm/affine.h#L128-L148): `glm_vec3_normalize_to(axis, axisn)`
followed by the Rodrigues terms, which reduce to `c I` when `axisn` is zero.

## How hypatia does it

`matrix4_set_from_axisv3_angle` normalizes the axis with scaling, so any non-zero axis
gives the rotation about its direction, and a zero axis gives the identity.

```c
#define HYPATIA_SINGLE_PRECISION_FLOATS
#define HYPATIA_IMPLEMENTATION
#include <float.h>
#include <math.h>
#include <stdio.h>
#include "hypatia.h"

int main(void)
{
	struct matrix4 m;
	struct vector3 axis;
	struct vector3 r;

	matrix4_set_from_axisv3_angle(&m, HYP_VECTOR3_ZERO, HYP_PI / 2);
	matrix4_multiplyv3(&m, HYP_VECTOR3_UNIT_X, &r);
	printf("quarter turn about (0, 0, 0):    X -> (%g, %g, %g)\n", r.x, r.y, r.z);
	matrix4_set_from_axisv3_angle(&m, vector3_setf3(&axis, 0, 0, 1e-8f), HYP_PI / 2);
	matrix4_multiplyv3(&m, HYP_VECTOR3_UNIT_X, &r);
	printf("quarter turn about (0, 0, 1e-8): X -> (%g, %g, %g)\n", r.x, r.y, r.z);
	return 0;
}
```

```text
quarter turn about (0, 0, 0):    X -> (1, 0, 0)
quarter turn about (0, 0, 1e-8): X -> (-4.37114e-08, 1, 0)
```

## Suggested fix

Fix the normalization threshold ([04](04-normalize-zeroes-short-vectors.md)) and return
the identity for an exactly zero axis.

## Checking

`compare/check_reports.py docs/reports/cglm/06-rotate-make-zero-axis.md` builds both programs above and compares their output with this report.
