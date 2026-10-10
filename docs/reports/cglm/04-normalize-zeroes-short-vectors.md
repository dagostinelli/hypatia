# `glm_vec3_normalize` sets every vector shorter than 1.19e-7 to zero, and long vectors overflow

| | |
|---|---|
| Library | cglm 0.9.4 (1796cc5) |
| Function | `glm_vec3_normalize`, `glm_vec3_normalize_to`, `glm_vec4_normalize` (`vec3.h`, `vec4.h`) |
| Kind | wrong result |
| Precision | float |
| Status | still present in cglm 0.9.6 (the latest release) and at master 58d8c15 (2026-07-29): the program prints the same |

## Summary

`glm_vec3_normalize` returns the zero vector whenever the length is below `FLT_EPSILON`
(1.19e-7).  Vectors that short are ordinary in float (a velocity of 1e-8 per step, a
difference of nearby points); their direction is representable and lost.  Above about 1e19
the squared length overflows to inf and the result is zero as well; a NaN component makes
every component NaN.  Every cglm function that normalizes (rotation axes, `glm_quatv`,
`glm_rotate_make`, `glm_quat_imagn`) inherits this.  `glm_vec4_normalize` compares the
length with `FLT_EPSILON` the same way on its scalar path; its SSE and WebAssembly paths
compare the squared length, which zeroes every 4-vector shorter than 3.45e-4
([14](14-vec4-normalize-sse-threshold.md)).  For components spread from 1e-15 to 1e15 the
harness measures a mean error of 1.7e5 epsilons (largest 8.4e6), against 0.085 with
hypatia.

## Reproduction

```c
#include <cglm/cglm.h>
#include <math.h>
#include <stdio.h>

int main(void)
{
	vec3 a = {1e-8f, 2e-8f, 2e-8f};
	vec3 b = {1e30f, 1e30f, 0};
	vec3 c = {NAN, 1, 0};

	glm_vec3_normalize(a);
	glm_vec3_normalize(b);
	glm_vec3_normalize(c);
	printf("normalize(1e-8, 2e-8, 2e-8) = (%g, %g, %g)\n", a[0], a[1], a[2]);
	printf("normalize(1e30, 1e30, 0)    = (%g, %g, %g)\n", b[0], b[1], b[2]);
	printf("normalize(NaN, 1, 0)        = (%g, %g, %g)\n", c[0], c[1], c[2]);
	return 0;
}
```

Output (x86-64, gcc 13.3, `-O2`):

```text
normalize(1e-8, 2e-8, 2e-8) = (0, 0, 0)
normalize(1e30, 1e30, 0)    = (0, 0, 0)
normalize(NaN, 1, 0)        = (nan, nan, nan)
```

Expected: (0.333333, 0.666667, 0.666667) and (0.707107, 0.707107, 0); for the NaN input, NaN only in the component that was NaN (or the input unchanged).

## Cause

[`vec3.h` lines 649-660](https://github.com/recp/cglm/blob/1796cc5ce298235b615dc7a4750b8c3ba56a05dd/include/cglm/vec3.h#L649-L660):

```
norm = glm_vec3_norm(v);
if (CGLM_UNLIKELY(norm < FLT_EPSILON)) {
  v[0] = v[1] = v[2] = 0.0f;
  return;
}
glm_vec3_scale(v, 1.0f / norm, v);
```

`FLT_EPSILON` is a relative quantity (the spacing of floats near 1); as an absolute
threshold on a length it has no meaning.  `glm_vec3_norm` squares the components.

## How hypatia does it

`vector3_normalize` (all hypatia normalizations use `hyp_normalize`) leaves unchanged only
an exactly zero vector, or one with a NaN component.  When the sum of the squares is not
between 1e-30 and 1e30 it divides by the largest component first, so short and long
vectors normalize as accurately as others.

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

	vector3_normalize(vector3_setf3(&v, 1e-8f, 2e-8f, 2e-8f));
	printf("normalize(1e-8, 2e-8, 2e-8) = (%g, %g, %g)\n", v.x, v.y, v.z);
	vector3_normalize(vector3_setf3(&v, 1e30f, 1e30f, 0));
	printf("normalize(1e30, 1e30, 0)    = (%g, %g, %g)\n", v.x, v.y, v.z);
	vector3_normalize(vector3_setf3(&v, NAN, 1, 0));
	printf("normalize(NaN, 1, 0)        = (%g, %g, %g)\n", v.x, v.y, v.z);
	return 0;
}
```

```text
normalize(1e-8, 2e-8, 2e-8) = (0.333333, 0.666667, 0.666667)
normalize(1e30, 1e30, 0)    = (0.707107, 0.707107, 0)
normalize(NaN, 1, 0)        = (nan, 1, 0)
```

## Suggested fix

Return early only for a norm of exactly zero; when `dot(v, v)` is outside a safe range
such as 1e-30 to 1e30, divide by the largest component first.  Dividing by `norm` instead
of multiplying by `1.0f / norm` also removes a rounding
([12](12-precision-small-differences.md)).

## Checking

`compare/check_reports.py docs/reports/cglm/04-normalize-zeroes-short-vectors.md` builds both programs above and compares their output with this report. The harness: `results/single.md`, "Edge cases", and `results/precision/now.single.md`,
`vector3_normalize`.
