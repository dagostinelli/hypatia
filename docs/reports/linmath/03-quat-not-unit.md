# `quat_mul_vec3` and `mat4x4_from_quat` disagree, and neither rotates, when q is not unit length

| | |
|---|---|
| Library | linmath.h master 3eef828 (2022-06-19) |
| Function | `quat_mul_vec3`, `mat4x4_from_quat`, `mat4x4o_mul_quat` |
| Kind | wrong result for non-unit q |
| Precision | float |
| Status | present at master 3eef828 (2022-06-19, the latest commit; linmath.h has no releases) |

## Summary

`quat_mul_vec3` uses `v + w t + q.xyz x t` with `t = 2 q.xyz x v`, the expansion of
`q v q*` for |q| = 1; `mat4x4_from_quat` uses `a2 + b2 - c2 - d2` on the diagonal, which
is |q|^2 times the rotation.  For twice the quarter turn about Z the first takes x to
(-3, 4, 0), the second to (0, 4, 0).  Quaternions that are composed repeatedly with
`quat_mul` drift from unit length; at a drift of 1e-3 the vector rotated by
`quat_mul_vec3` is 2e-3 longer than the input.

## Reproduction

```c
#include <linmath.h>
#include <stdio.h>

int main(void)
{
	float h = sqrtf(0.5f);
	quat twice = {0, 0, 2 * h, 2 * h}, drifted = {0, 0, 1.001f * h, 1.001f * h};
	vec3 x = {1, 0, 0}, r;
	vec4 x4 = {1, 0, 0, 0}, m4;
	mat4x4 m;

	quat_mul_vec3(r, twice, x);
	printf("quat_mul_vec3(2q, x)        = (%g, %g, %g)\n", r[0], r[1], r[2]);
	mat4x4_from_quat(m, twice);
	mat4x4_mul_vec4(m4, m, x4);
	printf("mat4x4_from_quat(2q) * x    = (%g, %g, %g)\n", m4[0], m4[1], m4[2]);
	quat_mul_vec3(r, drifted, x);
	printf("quat_mul_vec3(1.001 q, x): length %.7g\n", vec3_len(r));
	return 0;
}
```

Output (x86-64, gcc 13.3, `-O2`):

```text
quat_mul_vec3(2q, x)        = (-3, 4, 0)
mat4x4_from_quat(2q) * x    = (0, 4, 0)
quat_mul_vec3(1.001 q, x): length 1.002003
```

Expected: (0, 1, 0) from both, and length 1 for the drifted q.

## Cause

[`linmath.h` lines 482-500](https://github.com/datenwolf/linmath.h/blob/3eef82841046507e16a0f6194a61cee2eadd34b3/linmath.h#L482-L500) (`quat_mul_vec3`) and
[lines 502-530](https://github.com/datenwolf/linmath.h/blob/3eef82841046507e16a0f6194a61cee2eadd34b3/linmath.h#L502-L530) (`mat4x4_from_quat`, `M[0][0] = a2 + b2 - c2 - d2;`).

## Suggested fix

Divide both by `|q|^2`: in `quat_mul_vec3` divide the result, in `mat4x4_from_quat` divide
every entry of the 3x3 part.  If unit length is a requirement, document it on both.

## How hypatia does it

`vector3_rotate_by_quaternion` evaluates `(2 (u . v) u + (w^2 - u . u) v + 2 w (u x v)) /
|q|^2` and `matrix4_set_from_quaternion` uses `2 / |q|^2`: both are the rotation by
`q / |q|` for any length of q, and they agree.

```c
#define HYPATIA_SINGLE_PRECISION_FLOATS
#define HYPATIA_IMPLEMENTATION
#include <math.h>
#include <stdio.h>
#include "hypatia.h"

int main(void)
{
	float h = sqrtf(0.5f);
	struct quaternion twice, drifted;
	struct vector3 r, x;
	struct matrix4 m;

	quaternion_setf4(&twice, 0, 0, 2 * h, 2 * h);
	quaternion_setf4(&drifted, 0, 0, 1.001f * h, 1.001f * h);
	vector3_rotate_by_quaternion(vector3_setf3(&r, 1, 0, 0), &twice);
	printf("vector3_rotate_by_quaternion(x, 2q) = (%g, %g, %g)\n", r.x, r.y, r.z);
	matrix4_multiplyv3(matrix4_set_from_quaternion(&m, &twice), vector3_setf3(&x, 1, 0, 0), &r);
	printf("matrix4_set_from_quaternion(2q) * x = (%g, %g, %g)\n", r.x, r.y, r.z);
	vector3_rotate_by_quaternion(vector3_setf3(&r, 1, 0, 0), &drifted);
	printf("vector3_rotate_by_quaternion(x, 1.001 q): length %.7g\n", vector3_magnitude(&r));
	return 0;
}
```

```text
vector3_rotate_by_quaternion(x, 2q) = (0, 1, 0)
matrix4_set_from_quaternion(2q) * x = (0, 1, 0)
vector3_rotate_by_quaternion(x, 1.001 q): length 1
```

## Checking

`compare/check_reports.py docs/reports/linmath/03-quat-not-unit.md` builds both programs
above and compares their output with this report.
