# `glm_vec3_refract` has a sign error: it never gives the refracted direction

| | |
|---|---|
| Library | cglm 0.9.4 (1796cc5) |
| Function | `glm_vec3_refract` (`vec3.h`) |
| Kind | wrong result |
| Precision | float |
| Status | fixed in cglm 0.9.5 by 48839a3 ("fix refract", 2024-07-15); not present in 0.9.6 or at master.  Kept as a record; not to be filed |

## Summary

`glm_vec3_refract` computes `k = 1 + eta^2 - (eta n.v)^2`; Snell's law gives
`k = 1 - eta^2 + (eta n.v)^2`, i.e. `1 - eta^2 (1 - (n.v)^2)`.  The result is wrong for
every input: with `eta = 1` (no change of medium) a ray at 45 degrees comes out at
(0.707, -1.225, 0), not even of unit length, instead of passing straight through; total
internal reflection is never detected for the cases where it happens.  In the comparison
harness cglm disagrees with GLM and hypatia on all 2000 random inputs.

## Reproduction

```c
#include <cglm/cglm.h>
#include <math.h>
#include <stdio.h>

int main(void)
{
	vec3 v = {0.70710678f, -0.70710678f, 0};  /* 45 degrees onto the floor */
	vec3 n = {0, 1, 0};
	vec3 r;

	glm_vec3_refract(v, n, 1.0f, r);
	printf("eta 1:     (%.6f, %.6f, %g), length %.6f\n", r[0], r[1], r[2], glm_vec3_norm(r));
	glm_vec3_refract(v, n, 1.0f / 1.5f, r);
	printf("eta 1/1.5: (%.6f, %.6f, %g), length %.6f\n", r[0], r[1], r[2], glm_vec3_norm(r));
	return 0;
}
```

Output (x86-64, gcc 13.3, `-O2`):

```text
eta 1:     (0.707107, -1.224745, 0), length 1.414214
eta 1/1.5: (0.471405, -1.105542, 0), length 1.201850
```

Expected: (0.707107, -0.707107, 0) for eta 1; (0.471405, -0.881917, 0), of unit length, for eta 1/1.5.

## Cause

[`vec3.h` lines 1261-1276](https://github.com/recp/cglm/blob/1796cc5ce298235b615dc7a4750b8c3ba56a05dd/include/cglm/vec3.h#L1261-L1276):

```
ndi = glm_vec3_dot(n, v);
eni = eta * ndi;
k   = 1.0f + eta * eta - eni * eni;
```

The signs of the two eta terms are swapped.

## How hypatia does it

`vector3_refract(self, normal, eta)` normalizes both vectors (they need not be unit
length), computes `k = 1 - eta^2 (1 - d^2)` with `d = n . v`, returns the zero vector when
`k < 0` (total internal reflection) and otherwise `eta v - (eta d + sqrt(k)) n`, a unit
direction.  A zero normal leaves the direction unchanged.

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
	struct vector3 n;

	vector3_setf3(&n, 0, 1, 0);
	vector3_refract(vector3_setf3(&v, 0.70710678f, -0.70710678f, 0), &n, 1.0f);
	printf("eta 1:     (%.6f, %.6f, %g), length %.6f\n", v.x, v.y, v.z, vector3_magnitude(&v));
	vector3_refract(vector3_setf3(&v, 0.70710678f, -0.70710678f, 0), &n, 1.0f / 1.5f);
	printf("eta 1/1.5: (%.6f, %.6f, %g), length %.6f\n", v.x, v.y, v.z, vector3_magnitude(&v));
	return 0;
}
```

```text
eta 1:     (0.707107, -0.707107, 0), length 1.000000
eta 1/1.5: (0.471405, -0.881917, 0), length 1.000000
```

## Suggested fix

`k = 1.0f - eta * eta + eni * eni;` (the rest of the function is right).

## Checking

`compare/check_reports.py docs/reports/cglm/01-refract-sign-error.md` builds both programs above and compares their output with this report. The harness: `results/single.md`, `vector3_refract` against `cglm glm_vec3_refract`
(2000 of 2000 inputs differ), and `results/precision/now.single.md` (cglm 9.2e6 epsilons in
the mean, hypatia 0.385).
