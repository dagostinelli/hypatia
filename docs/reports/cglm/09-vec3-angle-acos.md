# `glm_vec3_angle` returns 0 for vectors up to about 6e-4 rad apart, and NaN for a zero vector

| | |
|---|---|
| Library | cglm 0.9.4 (1796cc5) |
| Function | `glm_vec3_angle` (`vec3.h`) |
| Kind | precision; NaN on degenerate input |
| Precision | float |
| Status | still present in cglm 0.9.6 (the latest release) and at master 58d8c15 (2026-07-29): the program prints the same |

## Summary

`glm_vec3_angle` is `acosf` of the dot product times the reciprocal of the product of the
norms, and returns 0 when that is above 1.  Near 0 the result is 0 whenever the scaled
dot product rounds to 1 or above.  For X and `(cosf(a), sinf(a), 0)`, a scan of every
float angle a from 1e-5 rad (not shown) finds 0 for every a below 2.99e-4 rad and for 21%
of the angles between 2.99e-4 and 4.57e-4 rad; the program below shows 1e-4 and 4.5e-4.
Over random pairs of unit vectors 6e-4 rad apart, the program finds 2918 of 100000 give 0.
The nonzero results are coarse: the float just below 1 is 1 - 2^-24, whose acos is
3.45e-4, so 3e-4 and 4e-4 both give 3.45e-4.  With a zero vector, `1.0f / 0` gives inf,
`0 * inf` gives NaN, and the comparisons let NaN through.

## Reproduction

```c
#include <cglm/cglm.h>
#include <math.h>
#include <stdio.h>

static unsigned long long state = 88172645463325252ULL;
static double rnd(void) { state ^= state << 13; state ^= state >> 7; state ^= state << 17; return (state >> 11) * (1.0 / 9007199254740992.0); }

int main(void)
{
	float angles[4] = {1e-4f, 3e-4f, 4e-4f, 4.5e-4f};
	vec3 x = {1, 0, 0}, zero = {0, 0, 0};
	int i, k, zeros = 0;

	for (i = 0; i < 4; i++) {
		vec3 y = {cosf(angles[i]), sinf(angles[i]), 0};
		printf("angle between X and a vector %g rad from it: %g\n", angles[i], glm_vec3_angle(x, y));
	}
	/* random unit vectors u and vectors 6e-4 rad from them */
	for (i = 0; i < 100000; i++) {
		double u[3], w[3], n = 0, d = 0, m = 0;
		vec3 p, q;
		for (k = 0; k < 3; k++) { u[k] = 2 * rnd() - 1; n += u[k] * u[k]; }
		for (k = 0; k < 3; k++) { u[k] /= sqrt(n); w[k] = 2 * rnd() - 1; d += w[k] * u[k]; }
		for (k = 0; k < 3; k++) { w[k] -= d * u[k]; m += w[k] * w[k]; }
		for (k = 0; k < 3; k++) { p[k] = (float)u[k]; q[k] = (float)(u[k] * cos(6e-4) + w[k] / sqrt(m) * sin(6e-4)); }
		zeros += glm_vec3_angle(p, q) == 0;
	}
	printf("random pairs 6e-4 rad apart: %d of 100000 give 0\n", zeros);
	printf("angle between X and the zero vector: %g\n", glm_vec3_angle(x, zero));
	return 0;
}
```

Output (x86-64, gcc 13.3, `-O2`):

```text
angle between X and a vector 0.0001 rad from it: 0
angle between X and a vector 0.0003 rad from it: 0.000345267
angle between X and a vector 0.0004 rad from it: 0.000345267
angle between X and a vector 0.00045 rad from it: 0
random pairs 6e-4 rad apart: 2918 of 100000 give 0
angle between X and the zero vector: -nan
```

Expected: each angle to within float precision, no zeros among the random pairs, and a
defined value (0) for the zero vector.

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
#include <math.h>
#include <stdio.h>
#include "hypatia.h"

static unsigned long long state = 88172645463325252ULL;
static double rnd(void) { state ^= state << 13; state ^= state >> 7; state ^= state << 17; return (state >> 11) * (1.0 / 9007199254740992.0); }

int main(void)
{
	float angles[4] = {1e-4f, 3e-4f, 4e-4f, 4.5e-4f};
	struct vector3 y, p, q;
	int i, k, zeros = 0;

	for (i = 0; i < 4; i++) {
		vector3_setf3(&y, cosf(angles[i]), sinf(angles[i]), 0);
		printf("angle between X and a vector %g rad from it: %g\n", angles[i], vector3_angle_between(HYP_VECTOR3_UNIT_X, &y));
	}
	for (i = 0; i < 100000; i++) {
		double u[3], w[3], n = 0, d = 0, m = 0;
		for (k = 0; k < 3; k++) { u[k] = 2 * rnd() - 1; n += u[k] * u[k]; }
		for (k = 0; k < 3; k++) { u[k] /= sqrt(n); w[k] = 2 * rnd() - 1; d += w[k] * u[k]; }
		for (k = 0; k < 3; k++) { w[k] -= d * u[k]; m += w[k] * w[k]; }
		for (k = 0; k < 3; k++) { p.v[k] = (float)u[k]; q.v[k] = (float)(u[k] * cos(6e-4) + w[k] / sqrt(m) * sin(6e-4)); }
		zeros += vector3_angle_between(&p, &q) == 0;
	}
	printf("random pairs 6e-4 rad apart: %d of 100000 give 0\n", zeros);
	printf("angle between X and the zero vector: %g\n", vector3_angle_between(HYP_VECTOR3_UNIT_X, HYP_VECTOR3_ZERO));
	return 0;
}
```

```text
angle between X and a vector 0.0001 rad from it: 0.0001
angle between X and a vector 0.0003 rad from it: 0.0003
angle between X and a vector 0.0004 rad from it: 0.0004
angle between X and a vector 0.00045 rad from it: 0.00045
random pairs 6e-4 rad apart: 0 of 100000 give 0
angle between X and the zero vector: 0
```

## Suggested fix

`return atan2f(glm_vec3_norm(cross(a, b)), glm_vec3_dot(a, b));` with `a` and `b`
normalized (scale invariance makes the normalization optional for the angle itself), and
0 when either length is zero.

## Checking

`compare/check_reports.py docs/reports/cglm/09-vec3-angle-acos.md` builds both programs above and compares their output with this report.
