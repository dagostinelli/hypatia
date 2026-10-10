# `glm_quat_from_vecs` returns the identity for vectors up to 0.26 degrees apart, and a wrong half turn near opposite

| | |
|---|---|
| Library | cglm 0.9.4 (1796cc5) |
| Function | `glm_quat_from_vecs` (`quat.h`) |
| Kind | wrong result |
| Precision | float |
| Status | still present in cglm 0.9.6 (the latest release) and at master 58d8c15 (2026-07-29): the program prints the same |

## Summary

`glm_quat_from_vecs` compares the dot product with `1 - GLM_FLT_EPSILON` and
`-1 + GLM_FLT_EPSILON`, and `GLM_FLT_EPSILON` is 1e-5 by default.  So:
- for unit vectors less than sqrt(2e-5) = 4.5e-3 rad (0.26 degrees) apart it returns the
  identity;
- for vectors within 4.5e-3 rad of opposite it returns a half turn about `glm_vec3_ortho(a)`,
  an axis that ignores `b`: the rotation misses `b` by the whole remaining angle.

At 1e-3 rad from opposite every input misses by 1e-3 rad (8400 float epsilons); at 1e-6
rad apart every input misses by 1e-6 rad.

## Reproduction

```c
#include <cglm/cglm.h>
#include <math.h>
#include <stdio.h>

int main(void)
{
	vec3 a = {0.36f, 0.48f, 0.8f}, p = {0.8f, -0.6f, 0}, b, r;
	versor q;
	int k;

	/* 3e-3 rad apart: the identity */
	for (k = 0; k < 3; k++) b[k] = a[k] * cosf(3e-3f) + p[k] * sinf(3e-3f);
	glm_quat_from_vecs(a, b, q);
	glm_quat_rotatev(q, a, r);
	printf("3e-3 rad apart:        q = (%g, %g, %g, %g), misses b by %.3g rad\n", q[0], q[1], q[2], q[3], glm_vec3_distance(r, b));

	/* 1e-3 rad from opposite: a half turn about another axis */
	for (k = 0; k < 3; k++) b[k] = -a[k] * cosf(1e-3f) + p[k] * sinf(1e-3f);
	glm_quat_from_vecs(a, b, q);
	glm_quat_rotatev(q, a, r);
	printf("1e-3 rad from opposite: q = (%.3f, %.3f, %.3f, %g), misses b by %.3g rad\n", q[0], q[1], q[2], q[3], glm_vec3_distance(r, b));
	return 0;
}
```

Output (x86-64, gcc 13.3, `-O2`):

```text
3e-3 rad apart:        q = (0, 0, 0, 1), misses b by 0.003 rad
1e-3 rad from opposite: q = (-0.673, -0.460, 0.579, 0), misses b by 0.001 rad
```

Expected: a rotation that takes `a` onto `b` to within float precision (about 1e-7) in both cases.

## Cause

[`quat.h` lines 204-224](https://github.com/recp/cglm/blob/1796cc5ce298235b615dc7a4750b8c3ba56a05dd/include/cglm/quat.h#L204-L224), with
[`common.h` lines 58-64](https://github.com/recp/cglm/blob/1796cc5ce298235b615dc7a4750b8c3ba56a05dd/include/cglm/common.h#L58-L64) defining `GLM_FLT_EPSILON` as `1e-5f`
unless `CGLM_USE_DEFAULT_EPSILON` is set:

```
if (cos_theta >= 1.f - GLM_FLT_EPSILON) { glm_quat_identity(dest); return; }
if (cos_theta < -1.f + GLM_FLT_EPSILON) { glm_vec3_ortho(a, axis); cos_half_theta = 0.f; }
```

A threshold on the cosine is a threshold on the square of the angle.

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
#include <float.h>
#include <math.h>
#include <stdio.h>
#include "hypatia.h"

int main(void)
{
	struct vector3 a, p, b, r;
	struct quaternion q;
	float c, s;

	vector3_setf3(&a, 0.36f, 0.48f, 0.8f);
	vector3_setf3(&p, 0.8f, -0.6f, 0);

	c = cosf(3e-3f); s = sinf(3e-3f);
	vector3_setf3(&b, a.x * c + p.x * s, a.y * c + p.y * s, a.z * c + p.z * s);
	quaternion_get_rotation_tov3(&a, &b, &q);
	vector3_rotate_by_quaternion(vector3_set(&r, &a), &q);
	printf("3e-3 rad apart:         misses b by %.3g rad\n", vector3_distance(&r, &b));

	c = cosf(1e-3f); s = sinf(1e-3f);
	vector3_setf3(&b, -a.x * c + p.x * s, -a.y * c + p.y * s, -a.z * c + p.z * s);
	quaternion_get_rotation_tov3(&a, &b, &q);
	vector3_rotate_by_quaternion(vector3_set(&r, &a), &q);
	printf("1e-3 rad from opposite: misses b by %.3g rad\n", vector3_distance(&r, &b));
	return 0;
}
```

```text
3e-3 rad apart:         misses b by 6.66e-08 rad
1e-3 rad from opposite: misses b by 1.4e-07 rad
```

## Suggested fix

Remove both thresholds.  Take the half angle from `length(a + b)` and `length(a - b)` of
the unit vectors and the axis from `cross(a, a + b)`, and use `glm_vec3_ortho` only when
that cross product is exactly zero.

## Checking

`compare/check_reports.py docs/reports/cglm/05-quat-from-vecs-thresholds.md` builds both programs above and compares their output with this report. The harness: `results/precision/now.single.md`, `quaternion_get_rotation_tov3`, "1e-3
rad from opposite" (cglm 8390 epsilons in the mean, hypatia 0.35) and "1e-6 rad apart"
(cglm 8.39, hypatia 0.26).
