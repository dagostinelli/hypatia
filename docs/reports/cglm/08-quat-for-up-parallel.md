# `glm_quat_for` returns a quaternion of length 0.71 when up is parallel to the direction

| | |
|---|---|
| Library | cglm 0.9.4 (1796cc5) |
| Function | `glm_quat_for`, `glm_quat_forp` (`quat.h`) |
| Kind | wrong result on degenerate input |
| Precision | float |
| Status | still present in cglm 0.9.6 (the latest release) and at master 58d8c15 (2026-07-29): the program prints the same |

## Summary

When `up` is parallel to `dir`, `glm_vec3_crossn(up, -dir)` normalizes a zero cross
product to zero, the matrix passed to `glm_mat3_quat` has two zero columns, and the result
has length 0.71.  Its direction happens to turn -Z toward `dir`, but it is not a unit
quaternion: multiplied with other rotations it scales, and `glm_quat_rotatev`'s result
depends on how the caller normalizes.

## Reproduction

```c
#include <cglm/cglm.h>
#include <math.h>
#include <stdio.h>

int main(void)
{
	vec3 dir = {0, 1, 0}, up = {0, 1, 0};
	versor q;

	glm_quat_for(dir, up, q);
	printf("glm_quat_for((0, 1, 0), up (0, 1, 0)) = (%g, %g, %g, %g), length %g\n", q[0], q[1], q[2], q[3], glm_quat_norm(q));
	return 0;
}
```

Output (x86-64, gcc 13.3, `-O2`):

```text
glm_quat_for((0, 1, 0), up (0, 1, 0)) = (0.5, -0, 0, 0.5), length 0.707107
```

Expected: a unit quaternion (for example the shortest rotation from -Z to `dir`).

## Cause

[`quat.h` lines 772-783](https://github.com/recp/cglm/blob/1796cc5ce298235b615dc7a4750b8c3ba56a05dd/include/cglm/quat.h#L772-L783): `glm_vec3_crossn(up, m[2], m[0])` is zero
and so is `m[1] = cross(m[2], m[0])`; the matrix is not a rotation.

## How hypatia does it

`quaternion_set_look_rotation_rh` detects an exactly zero right axis (up zero or parallel
to the direction) and returns the shortest rotation from -Z to the direction
(`quaternion_get_rotation_tov3`), a unit quaternion.

```c
#define HYPATIA_SINGLE_PRECISION_FLOATS
#define HYPATIA_IMPLEMENTATION
#include <float.h>
#include <math.h>
#include <stdio.h>
#include "hypatia.h"

int main(void)
{
	struct vector3 dir;
	struct quaternion q;

	quaternion_set_look_rotation_rh(&q, vector3_setf3(&dir, 0, 1, 0), HYP_VECTOR3_UNIT_Y);
	printf("quaternion_set_look_rotation_rh((0, 1, 0), up (0, 1, 0)) = (%g, %g, %g, %g), length %g\n", q.x, q.y, q.z, q.w, quaternion_magnitude(&q));
	return 0;
}
```

```text
quaternion_set_look_rotation_rh((0, 1, 0), up (0, 1, 0)) = (0.707107, 0, 0, 0.707107), length 1
```

## Suggested fix

When the cross product is zero, fall back to `glm_quat_from_vecs((0, 0, -1), dir)` (once
that is accurate, [05](05-quat-from-vecs-thresholds.md)).

## Checking

`compare/check_reports.py docs/reports/cglm/08-quat-for-up-parallel.md` builds both programs above and compares their output with this report.
