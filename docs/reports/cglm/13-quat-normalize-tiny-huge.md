# `glm_quat_normalize` turns a tiny quaternion into the identity, a different rotation, and a huge one into zero

| | |
|---|---|
| Library | cglm 0.9.4 (1796cc5) |
| Function | `glm_quat_normalize`, `glm_quat_normalize_to` (`quat.h`) |
| Kind | wrong result |
| Precision | float |
| Status | still present in cglm 0.9.6 (the latest release) and at master 58d8c15 (2026-07-29): the program prints the same |

## Summary

`glm_quat_normalize` divides by `sqrtf(dot(q, q))` and returns the identity when the dot
product is not positive.  For components below about 1e-19 the dot product is subnormal
and loses precision; below about 2.6e-23 it is exactly 0 and the result is the identity:
(1e-25, 0, 0, 1e-25), a quarter turn, becomes no rotation.  Above 1e19 the dot product
overflows and the result is zero.  `glm_vec4_normalize` sets short vectors to zero
([04](04-normalize-zeroes-short-vectors.md)).

## Reproduction

```c
#include <cglm/cglm.h>
#include <stdio.h>

int main(void)
{
	versor tiny = {0, 0, 1e-25f, 1e-25f}, huge = {0, 0, 1e25f, 1e25f};

	glm_quat_normalize(tiny);
	glm_quat_normalize(huge);
	printf("normalize(z 1e-25, w 1e-25) = (%g, %g, %g, %g)\n", tiny[0], tiny[1], tiny[2], tiny[3]);
	printf("normalize(z 1e25, w 1e25)   = (%g, %g, %g, %g)\n", huge[0], huge[1], huge[2], huge[3]);
	return 0;
}
```

Output (x86-64, gcc 13.3, `-O2`):

```text
normalize(z 1e-25, w 1e-25) = (0, 0, 0, 1)
normalize(z 1e25, w 1e25)   = (0, 0, 0, 0)
```

Expected: (0, 0, 0.707107, 0.707107) in both cases.

## Cause

[`quat.h` lines 245-287](https://github.com/recp/cglm/blob/1796cc5ce298235b615dc7a4750b8c3ba56a05dd/include/cglm/quat.h#L245-L287): every branch computes `dot = q . q` in
float and returns the identity for `dot <= 0`.

## How hypatia does it

`quaternion_normalize` uses `hyp_normalize`: it divides by the length when the sum of the
squares is between 1e-30 and 1e30, and otherwise divides by the largest component first,
so tiny and huge quaternions normalize to the same unit quaternion as any other multiple.
Only the zero quaternion (or NaN) is left unchanged.

```c
#define HYPATIA_SINGLE_PRECISION_FLOATS
#define HYPATIA_IMPLEMENTATION
#include <math.h>
#include <stdio.h>
#include "hypatia.h"

int main(void)
{
	struct quaternion q;

	quaternion_normalize(quaternion_setf4(&q, 0, 0, 1e-25f, 1e-25f));
	printf("normalize(z 1e-25, w 1e-25) = (%g, %g, %g, %g)\n", q.x, q.y, q.z, q.w);
	quaternion_normalize(quaternion_setf4(&q, 0, 0, 1e25f, 1e25f));
	printf("normalize(z 1e25, w 1e25)   = (%g, %g, %g, %g)\n", q.x, q.y, q.z, q.w);
	return 0;
}
```

```text
normalize(z 1e-25, w 1e-25) = (0, 0, 0.707107, 0.707107)
normalize(z 1e25, w 1e25)   = (0, 0, 0.707107, 0.707107)
```

## Suggested fix

Return the identity only for an exactly zero quaternion; when `dot` is outside the normal
range, divide by the largest component first.

## Checking

`compare/check_reports.py docs/reports/cglm/13-quat-normalize-tiny-huge.md` builds both programs above and compares their output with this report.
