# `mat4x4_look_at` gives NaN when the eye is at the target or looks along up

| | |
|---|---|
| Library | linmath.h master 3eef828 (2022-06-19) |
| Function | `mat4x4_look_at` |
| Kind | NaN on degenerate input |
| Precision | float |
| Status | present at master 3eef828 (2022-06-19, the latest commit; linmath.h has no releases) |

## Summary

`mat4x4_look_at` normalizes `center - eye` and `cross(f, up)` with `vec3_norm`, which
returns NaN for the zero vector ([02](02-norm-zero-tiny-huge.md)).  With the eye at the
target, 12 of the 16 entries of the view matrix are NaN; looking along up, 8.  Every vertex
transformed by such a matrix is NaN.

## Reproduction

```c
#include <linmath.h>
#include <stdio.h>

static void print(const char *what, mat4x4 m)
{
	int c, r, nans = 0;
	for (c = 0; c < 4; c++)
		for (r = 0; r < 4; r++)
			nans += isnan(m[c][r]) != 0;
	printf("%s: %d of 16 entries NaN\n", what, nans);
}

int main(void)
{
	vec3 eye = {0, 0, 5}, origin = {0, 0, 0}, above = {0, 5, 0}, up = {0, 1, 0};
	mat4x4 m;

	mat4x4_look_at(m, eye, eye, up);
	print("eye == target   ", m);
	mat4x4_look_at(m, origin, above, up);
	print("looking along up", m);
	return 0;
}
```

Output (x86-64, gcc 13.3, `-O2`):

```text
eye == target   : 12 of 16 entries NaN
looking along up: 8 of 16 entries NaN
```

Expected: a finite matrix (or an assertion): there is no view direction, or no right axis.

## Cause

[`linmath.h` lines 400-415](https://github.com/datenwolf/linmath.h/blob/3eef82841046507e16a0f6194a61cee2eadd34b3/linmath.h#L400-L415):
`vec3_sub(f, center, eye); vec3_norm(f, f); ... vec3_mul_cross(s, f, up); vec3_norm(s, s);`.

## Suggested fix

Normalize with a zero check ([02](02-norm-zero-tiny-huge.md)), or check the two lengths
and fall back (for example, to another up axis when `cross(f, up)` vanishes).

## How hypatia does it

`matrix4_view_lookat_rh` uses `vector3_normalize`, which leaves the zero vector unchanged,
so the degenerate axes are zero rows and the matrix stays finite.  Neither library can
produce a meaningful view without a direction; hypatia's result does not spread NaN.

```c
#define HYPATIA_SINGLE_PRECISION_FLOATS
#define HYPATIA_IMPLEMENTATION
#include <math.h>
#include <stdio.h>
#include "hypatia.h"

static void print(const char *what, const struct matrix4 *m)
{
	int i, nans = 0;
	for (i = 0; i < 16; i++)
		nans += isnan(m->m[i]) != 0;
	printf("%s: %d of 16 entries NaN\n", what, nans);
}

int main(void)
{
	struct matrix4 m;
	struct vector3 eye, target;

	vector3_setf3(&eye, 0, 0, 5);
	print("eye == target   ", matrix4_view_lookat_rh(&m, &eye, &eye, HYP_VECTOR3_UNIT_Y));
	vector3_setf3(&target, 0, 5, 0);
	print("looking along up", matrix4_view_lookat_rh(&m, HYP_VECTOR3_ZERO, &target, HYP_VECTOR3_UNIT_Y));
	return 0;
}
```

```text
eye == target   : 0 of 16 entries NaN
looking along up: 0 of 16 entries NaN
```

## Checking

`compare/check_reports.py docs/reports/linmath/05-look-at-degenerate-nan.md` builds both
programs above and compares their output with this report.
