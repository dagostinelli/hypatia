# `mat4x4_rotate` ignores the rotation when the axis is shorter than 1e-4, and `quat_rotate` is NaN for a zero axis

| | |
|---|---|
| Library | linmath.h master 3eef828 (2022-06-19) |
| Function | `mat4x4_rotate`, `quat_rotate` |
| Kind | wrong result; NaN on degenerate input |
| Precision | float |
| Status | present at master 3eef828 (2022-06-19, the latest commit; linmath.h has no releases) |

## Summary

`mat4x4_rotate` rotates only when `vec3_len(axis) > 1e-4`; otherwise it returns M
unchanged.  An axis such as (1e-5, 0, 0), for example a cross product of two short or
nearly parallel vectors, has a direction, but the rotation by 1 rad about it is dropped.
`quat_rotate` normalizes the axis with `vec3_norm`, so a zero axis gives a quaternion with
a NaN vector part ([02](02-norm-zero-tiny-huge.md)).

## Reproduction

```c
#include <linmath.h>
#include <stdio.h>

int main(void)
{
	mat4x4 id, m;
	vec3 zero = {0, 0, 0};
	quat q;

	mat4x4_identity(id);
	mat4x4_rotate(m, id, 1e-5f, 0, 0, 1);
	printf("mat4x4_rotate(axis (1e-5, 0, 0), 1 rad): M[1][1] = %g, M[1][2] = %g\n", m[1][1], m[1][2]);
	quat_rotate(q, 1, zero);
	printf("quat_rotate(1 rad, zero axis) = (%g, %g, %g, %g)\n", q[0], q[1], q[2], q[3]);
	return 0;
}
```

Output (x86-64, gcc 13.3, `-O2`):

```text
mat4x4_rotate(axis (1e-5, 0, 0), 1 rad): M[1][1] = 1, M[1][2] = 0
quat_rotate(1 rad, zero axis) = (-nan, -nan, -nan, 0.877583)
```

Expected: `M[1][1] = cos 1 = 0.540302` and `|M[1][2]| = sin 1 = 0.841471` for the rotation
about x; the identity (0, 0, 0, 1) for a rotation about no axis.

## Cause

[`linmath.h` lines 208-240](https://github.com/datenwolf/linmath.h/blob/3eef82841046507e16a0f6194a61cee2eadd34b3/linmath.h#L208-L240): `if(vec3_len(u) > 1e-4) { ... } else {
mat4x4_dup(R, M); }`; [lines 474-481](https://github.com/datenwolf/linmath.h/blob/3eef82841046507e16a0f6194a61cee2eadd34b3/linmath.h#L474-L481): `vec3_norm(axis_norm, axis);`.

## Suggested fix

Rotate whenever the axis is not exactly zero, normalizing with scaling for very short or
long axes; for the zero axis return M (`mat4x4_rotate`) or the identity (`quat_rotate`).

## How hypatia does it

`matrix4_set_from_axisv3_angle` and `quaternion_set_from_axis_anglev3` normalize the axis
with scaled normalization: any nonzero axis gives the rotation about its direction, and the
zero axis gives the identity.

```c
#define HYPATIA_SINGLE_PRECISION_FLOATS
#define HYPATIA_IMPLEMENTATION
#include <stdio.h>
#include "hypatia.h"

int main(void)
{
	struct matrix4 m;
	struct vector3 axis;
	struct quaternion q;

	matrix4_set_from_axisv3_angle(&m, vector3_setf3(&axis, 1e-5f, 0, 0), 1);
	printf("matrix4_set_from_axisv3_angle(axis (1e-5, 0, 0), 1 rad): r11 = %g, r21 = %g\n", m.r11, m.r21);
	quaternion_set_from_axis_anglev3(&q, vector3_setf3(&axis, 0, 0, 0), 1);
	printf("quaternion_set_from_axis_anglev3(zero axis, 1 rad) = (%g, %g, %g, %g)\n", q.x, q.y, q.z, q.w);
	return 0;
}
```

```text
matrix4_set_from_axisv3_angle(axis (1e-5, 0, 0), 1 rad): r11 = 0.540302, r21 = 0.841471
quaternion_set_from_axis_anglev3(zero axis, 1 rad) = (0, 0, 0, 1)
```

## Checking

`compare/check_reports.py docs/reports/linmath/04-rotate-short-axis-ignored.md` builds both
programs above and compares their output with this report.
