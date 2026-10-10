# `glm_vec3_proj` onto the zero vector is NaN

| | |
|---|---|
| Library | cglm 0.9.4 (1796cc5) |
| Function | `glm_vec3_proj` (`vec3.h`) |
| Kind | NaN on degenerate input |
| Precision | float |
| Status | still present in cglm 0.9.6 (the latest release) and at master 58d8c15 (2026-07-29): the program prints the same |

## Summary

`glm_vec3_proj(a, b)` scales `b` by `dot(a, b) / norm2(b)`: for `b = 0` that is 0/0 = NaN in
every component.  `norm2(b)` also underflows for components below 1e-19 and overflows
above 1e19.

## Reproduction

```c
#include <cglm/cglm.h>
#include <math.h>
#include <stdio.h>

int main(void)
{
	vec3 a = {1, 2, 3}, zero = {0, 0, 0}, tiny = {0, 1e-25f, 0}, r;

	glm_vec3_proj(a, zero, r);
	printf("proj((1, 2, 3), (0, 0, 0))     = (%g, %g, %g)\n", r[0], r[1], r[2]);
	glm_vec3_proj(a, tiny, r);
	printf("proj((1, 2, 3), (0, 1e-25, 0)) = (%g, %g, %g)\n", r[0], r[1], r[2]);
	return 0;
}
```

Output (x86-64, gcc 13.3, `-O2`):

```text
proj((1, 2, 3), (0, 0, 0))     = (-nan, -nan, -nan)
proj((1, 2, 3), (0, 1e-25, 0)) = (-nan, inf, -nan)
```

Expected: (0, 0, 0), and (0, 2, 0).

## Cause

[`vec3.h` lines 837-841](https://github.com/recp/cglm/blob/1796cc5ce298235b615dc7a4750b8c3ba56a05dd/include/cglm/vec3.h#L837-L841):
`glm_vec3_scale(b, glm_vec3_dot(a, b) / glm_vec3_norm2(b), dest);`.

## How hypatia does it

`vector3_project` divides by |b|^2 when it is in the normal range, projects onto the unit
vector (scaled normalization) otherwise, and gives the zero vector for `b = 0`.

```c
#define HYPATIA_SINGLE_PRECISION_FLOATS
#define HYPATIA_IMPLEMENTATION
#include <float.h>
#include <math.h>
#include <stdio.h>
#include "hypatia.h"

int main(void)
{
	struct vector3 v;
	struct vector3 onto;

	vector3_project(vector3_setf3(&v, 1, 2, 3), HYP_VECTOR3_ZERO);
	printf("project((1, 2, 3), (0, 0, 0))     = (%g, %g, %g)\n", v.x, v.y, v.z);
	vector3_project(vector3_setf3(&v, 1, 2, 3), vector3_setf3(&onto, 0, 1e-25f, 0));
	printf("project((1, 2, 3), (0, 1e-25, 0)) = (%g, %g, %g)\n", v.x, v.y, v.z);
	return 0;
}
```

```text
project((1, 2, 3), (0, 0, 0))     = (0, 0, 0)
project((1, 2, 3), (0, 1e-25, 0)) = (0, 2, 0)
```

## Suggested fix

Return the zero vector when `norm2(b)` is zero; outside the normal range, normalize `b`
with scaling and use `dot(a, b) b`.

## Checking

`compare/check_reports.py docs/reports/cglm/10-proj-onto-zero-vector.md` builds both programs above and compares their output with this report.
