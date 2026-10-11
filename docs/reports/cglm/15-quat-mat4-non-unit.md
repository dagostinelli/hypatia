# `glm_quat_mat4` and `glm_quat_mat3` scale by 2 / |q| instead of 2 / |q|^2

| | |
|---|---|
| Library | cglm 0.9.4 (1796cc5) |
| Function | `glm_quat_mat4`, `glm_quat_mat4t`, `glm_quat_mat3`, `glm_quat_mat3t` (`quat.h`) |
| Kind | wrong result |
| Precision | float |
| Status | still present in cglm 0.9.6 (the latest release) and at master 58d8c15 (2026-07-29): the program prints the same |

## Summary

The four quaternion-to-matrix functions compute `s = 2 / glm_quat_norm(q)` and build the
matrix from `s * x * x`, `s * x * y`, ...  `glm_quat_norm` is the length |q|, not the
squared length, so the products are scaled by 2 / |q| where the rotation matrix needs
2 / |q|^2.  The result is a rotation only when |q| = 1.  For any other length the matrix
scales and shears: the program below takes q of length 2 (90 degrees about z) and gets a
matrix that maps x to (-1, 2, 0) with determinant 5, and q of length 0.5 gives one that
maps x to (0.5, 0.5, 0) with determinant 0.5.  The division by the norm shows the functions are
meant to accept quaternions that are not of unit length; with the squared norm they would.

## Reproduction

```c
#include <cglm/cglm.h>
#include <math.h>
#include <stdio.h>

static void show(float k)
{
	float h = sqrtf(0.5f) * k;
	versor q = {0, 0, h, h}; /* 90 degrees about z, length k */
	vec3 x = {1, 0, 0}, r;
	mat4 m4;
	mat3 m3;

	glm_quat_mat4(q, m4);
	glm_mat4_mulv3(m4, x, 1.0f, r);
	glm_quat_mat3(q, m3);
	printf("|q| = %g: glm_quat_mat4 takes x to (%g, %g, %g); det of glm_quat_mat3 %g\n",
	       glm_quat_norm(q), r[0], r[1], r[2], glm_mat3_det(m3));
}

int main(void)
{
	show(1.0f);
	show(2.0f);
	show(0.5f);
	return 0;
}
```

Output (x86-64, gcc 13.3, `-O2`):

```text
|q| = 1: glm_quat_mat4 takes x to (0, 1, 0); det of glm_quat_mat3 1
|q| = 2: glm_quat_mat4 takes x to (-1, 2, 0); det of glm_quat_mat3 5
|q| = 0.5: glm_quat_mat4 takes x to (0.5, 0.5, 0); det of glm_quat_mat3 0.5
```

Expected: x goes to (0, 1, 0) and the determinant is 1 for every length of q, as for the
unit quaternion in the first line.

## Cause

[`quat.h` lines 489-490](https://github.com/recp/cglm/blob/1796cc5ce298235b615dc7a4750b8c3ba56a05dd/include/cglm/quat.h#L489-L490):
`norm = glm_quat_norm(q);` and `s = norm > 0.0f ? 2.0f / norm : 0.0f;`, the same at
[lines 537](https://github.com/recp/cglm/blob/1796cc5ce298235b615dc7a4750b8c3ba56a05dd/include/cglm/quat.h#L537),
[584](https://github.com/recp/cglm/blob/1796cc5ce298235b615dc7a4750b8c3ba56a05dd/include/cglm/quat.h#L584)
and [623](https://github.com/recp/cglm/blob/1796cc5ce298235b615dc7a4750b8c3ba56a05dd/include/cglm/quat.h#L623);
[`glm_quat_norm`, lines 233-235](https://github.com/recp/cglm/blob/1796cc5ce298235b615dc7a4750b8c3ba56a05dd/include/cglm/quat.h#L233-L235)
returns `glm_vec4_norm(q)`, the length.

## Suggested fix

Use the squared length, `glm_quat_norm2(q)` (as `glm_quat_inv` does), in all four
functions.  Outside a safe range such as 1e-30 to 1e30 for the squared length, normalize q
with scaling first and use `s = 2`.

## How hypatia does it

`matrix4_set_from_quaternion` uses `s = 2 / |q|^2` when |q|^2 is between 1e-30 and 1e30,
otherwise normalizes q with scaled normalization and uses `s = 2`; the zero quaternion
gives the identity.

```c
#define HYPATIA_SINGLE_PRECISION_FLOATS
#define HYPATIA_IMPLEMENTATION
#include <math.h>
#include <stdio.h>
#include "hypatia.h"

static void show(float k)
{
	float h = sqrtf(0.5f) * k;
	struct quaternion q;
	struct matrix4 m;
	struct vector3 x, r;

	quaternion_setf4(&q, 0, 0, h, h); /* 90 degrees about z, length k */
	matrix4_set_from_quaternion(&m, &q);
	matrix4_multiplyv3(&m, vector3_setf3(&x, 1, 0, 0), &r);
	printf("|q| = %g: matrix4_set_from_quaternion takes x to (%g, %g, %g); det %g\n",
	       quaternion_magnitude(&q), r.x, r.y, r.z, matrix4_determinant(&m));
}

int main(void)
{
	show(1.0f);
	show(2.0f);
	show(0.5f);
	return 0;
}
```

```text
|q| = 1: matrix4_set_from_quaternion takes x to (0, 1, 0); det 1
|q| = 2: matrix4_set_from_quaternion takes x to (0, 1, 0); det 1
|q| = 0.5: matrix4_set_from_quaternion takes x to (0, 1, 0); det 1
```

## Checking

`compare/check_reports.py docs/reports/cglm/15-quat-mat4-non-unit.md` builds both programs
above and compares their output with this report.
