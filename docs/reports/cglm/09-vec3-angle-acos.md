# `glm_vec3_angle` returns 0 for vectors up to 4.9e-4 rad apart, and NaN for a zero vector

| | |
|---|---|
| Library | cglm 0.9.4 (1796cc5) |
| Function | `glm_vec3_angle` (`vec3.h`) |
| Kind | precision; NaN on degenerate input |
| Precision | float |
| Status | still present in cglm 0.9.6 (the latest release) and at master 58d8c15 (2026-07-29): the program prints the same |

## Summary

`glm_vec3_angle` is `acosf` of the normalized dot product.  Near 0 the dot product rounds
to 1 for every angle below sqrt(2 FLT_EPSILON) = 4.9e-4 rad, and the result is 0: vectors
1e-4 rad apart have an angle of 0.  Above that threshold the relative error is still about
epsilon / angle^2.  With a zero vector, `1.0f / 0` gives inf, `0 * inf` gives NaN, and the
clamps let NaN through.

## Reproduction

```c
#include <cglm/cglm.h>
#include <math.h>
#include <stdio.h>

int main(void)
{
	vec3 x = {1, 0, 0}, near = {cosf(1e-4f), sinf(1e-4f), 0}, zero = {0, 0, 0};

	printf("angle between X and a vector 1e-4 rad from it: %g\n", glm_vec3_angle(x, near));
	printf("angle between X and the zero vector:           %g\n", glm_vec3_angle(x, zero));
	return 0;
}
```

Output (x86-64, gcc 13.3, `-O2`):

```text
angle between X and a vector 1e-4 rad from it: 0
angle between X and the zero vector:           -nan
```

Expected: 1e-4 (to within float precision), and a defined value (0) for the zero vector.

## Cause

[`vec3.h` lines 725-738](https://github.com/recp/cglm/blob/1796cc5ce298235b615dc7a4750b8c3ba56a05dd/include/cglm/vec3.h#L725-L738):

```
norm = 1.0f / (glm_vec3_norm(a) * glm_vec3_norm(b));
dot  = glm_vec3_dot(a, b) * norm;
if (dot > 1.0f) return 0.0f; else if (dot < -1.0f) return CGLM_PI;
return acosf(dot);
```

## How hypatia does it

`vector3_angle_between` computes `atan2(|a x b|, a . b)` of the unit vectors, accurate for
every angle; the angle with a zero vector is 0.

```c
#define HYPATIA_SINGLE_PRECISION_FLOATS
#define HYPATIA_IMPLEMENTATION
#include <float.h>
#include <math.h>
#include <stdio.h>
#include "hypatia.h"

int main(void)
{
	struct vector3 near;

	vector3_setf3(&near, cosf(1e-4f), sinf(1e-4f), 0);
	printf("angle between X and a vector 1e-4 rad from it: %g\n", vector3_angle_between(HYP_VECTOR3_UNIT_X, &near));
	printf("angle between X and the zero vector:           %g\n", vector3_angle_between(HYP_VECTOR3_UNIT_X, HYP_VECTOR3_ZERO));
	return 0;
}
```

```text
angle between X and a vector 1e-4 rad from it: 0.0001
angle between X and the zero vector:           0
```

## Suggested fix

`return atan2f(glm_vec3_norm(cross(a, b)), glm_vec3_dot(a, b));` with `a` and `b`
normalized (scale invariance makes the normalization optional for the angle itself), and
0 when either length is zero.

## Checking

`compare/check_reports.py docs/reports/cglm/09-vec3-angle-acos.md` builds both programs above and compares their output with this report.
