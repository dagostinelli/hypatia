# `glm_vec4_normalize` on SSE sets every vector shorter than 3.45e-4 to zero

| | |
|---|---|
| Library | cglm 0.9.4 (1796cc5) |
| Function | `glm_vec4_normalize`, `glm_vec4_normalize_to` (`vec4.h`) |
| Kind | wrong result |
| Precision | float |
| Status | still present in cglm 0.9.6 (the latest release) and at master 58d8c15 (2026-07-29): the program prints the same |

## Summary

On x86 (the SSE path, the default there), `glm_vec4_normalize_to` compares the squared
length `dot(v, v)` with `FLT_EPSILON` (1.19e-7) and returns the zero vector below it:
every 4-vector shorter than sqrt(1.19e-7) = 3.45e-4 is zeroed.  The WebAssembly path in
the source makes the same comparison; the program below shows the SSE path only.  The
scalar path compares the length itself with `FLT_EPSILON`, so the same call gives a unit
vector on one platform and zero on another for lengths between 1.19e-7 and 3.45e-4.

## Reproduction

```c
#include <cglm/cglm.h>
#include <stdio.h>

int main(void)
{
	vec4 v = {1e-4f, 2e-4f, 2e-4f, 0};

	glm_vec4_normalize(v);
	printf("normalize(1e-4, 2e-4, 2e-4, 0) = (%g, %g, %g, %g)\n", v[0], v[1], v[2], v[3]);
	return 0;
}
```

Output (x86-64, gcc 13.3, `-O2`):

```text
normalize(1e-4, 2e-4, 2e-4, 0) = (0, 0, 0, 0)
```

Expected: (0.333333, 0.666667, 0.666667, 0).

## Cause

[`vec4.h` lines 932-940](https://github.com/recp/cglm/blob/1796cc5ce298235b615dc7a4750b8c3ba56a05dd/include/cglm/vec4.h#L932-L940) (SSE):

```
xdot = glmm_vdot(x0, x0);
dot  = _mm_cvtss_f32(xdot);
if (CGLM_UNLIKELY(dot < FLT_EPSILON)) {
  glmm_store(dest, _mm_setzero_ps());
  return;
}
```

The threshold is on the square of the length; the WebAssembly branch at
[lines 912-925](https://github.com/recp/cglm/blob/1796cc5ce298235b615dc7a4750b8c3ba56a05dd/include/cglm/vec4.h#L912-L925)
does the same, and the scalar branch at
[lines 942-951](https://github.com/recp/cglm/blob/1796cc5ce298235b615dc7a4750b8c3ba56a05dd/include/cglm/vec4.h#L942-L951) applies it to the length.  See also
[04](04-normalize-zeroes-short-vectors.md) for the threshold itself.

## Suggested fix

Return zero only for `dot == 0` in every branch, and scale out-of-range vectors first
([04](04-normalize-zeroes-short-vectors.md)).

## How hypatia does it

`vector4_normalize` (through `hyp_normalize`) leaves unchanged only an exactly zero vector,
or one with a NaN component, and divides by the largest component first when the sum of
the squares is not between 1e-30 and 1e30; the result does not depend on the platform.

```c
#define HYPATIA_SINGLE_PRECISION_FLOATS
#define HYPATIA_IMPLEMENTATION
#include <stdio.h>
#include "hypatia.h"

int main(void)
{
	struct vector4 v;

	vector4_normalize(vector4_setf4(&v, 1e-4f, 2e-4f, 2e-4f, 0));
	printf("normalize(1e-4, 2e-4, 2e-4, 0) = (%g, %g, %g, %g)\n", v.x, v.y, v.z, v.w);
	return 0;
}
```

```text
normalize(1e-4, 2e-4, 2e-4, 0) = (0.333333, 0.666667, 0.666667, 0)
```

## Checking

`compare/check_reports.py docs/reports/cglm/14-vec4-normalize-sse-threshold.md` builds both
programs above and compares their output with this report.
