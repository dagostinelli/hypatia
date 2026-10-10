# `glm_quat_inv` of the zero quaternion is NaN; tiny and huge quaternions overflow

| | |
|---|---|
| Library | cglm 0.9.4 (1796cc5) |
| Function | `glm_quat_inv` (`quat.h`) |
| Kind | NaN on degenerate input |
| Precision | float |
| Status | still present in cglm 0.9.6 (the latest release) and at master 58d8c15 (2026-07-29): the program prints the same |

## Summary

`glm_quat_inv` scales the conjugate by `1.0f / glm_vec4_norm2(q)`.  For the zero quaternion
that is `0 * inf = NaN`; for components below about 1e-19 the squared norm underflows and
the result is inf or NaN, and above 1e19 it overflows to zero, although the inverse is
representable.

## Reproduction

```c
#include <cglm/cglm.h>
#include <math.h>
#include <stdio.h>

int main(void)
{
	versor zero = {0, 0, 0, 0}, tiny = {0, 0, 0, 1e-25f}, r;

	glm_quat_inv(zero, r);
	printf("inv(0)         = (%g, %g, %g, %g)\n", r[0], r[1], r[2], r[3]);
	glm_quat_inv(tiny, r);
	printf("inv(w = 1e-25) = (%g, %g, %g, %g)\n", r[0], r[1], r[2], r[3]);
	return 0;
}
```

Output (x86-64, gcc 13.3, `-O2`):

```text
inv(0)         = (-nan, -nan, -nan, -nan)
inv(w = 1e-25) = (-nan, -nan, -nan, inf)
```

Expected: a defined value for the zero quaternion (it has no inverse), and w = 1e25 for the second.

## Cause

[`quat.h` lines 333-337](https://github.com/recp/cglm/blob/1796cc5ce298235b615dc7a4750b8c3ba56a05dd/include/cglm/quat.h#L333-L337):
`glm_vec4_scale(conj, 1.0f / glm_vec4_norm2(q), dest);`.

## How hypatia does it

`quaternion_inverse` divides the conjugate by |q|^2 when it is between 1e-30 and 1e30, and
otherwise computes `(q / |q|) / |q|` with scaled normalization; the zero quaternion is
left unchanged.

```c
#define HYPATIA_SINGLE_PRECISION_FLOATS
#define HYPATIA_IMPLEMENTATION
#include <float.h>
#include <math.h>
#include <stdio.h>
#include "hypatia.h"

int main(void)
{
	struct quaternion q;

	quaternion_inverse(quaternion_setf4(&q, 0, 0, 0, 0));
	printf("inv(0)         = (%g, %g, %g, %g)\n", q.x, q.y, q.z, q.w);
	quaternion_inverse(quaternion_setf4(&q, 0, 0, 0, 1e-25f));
	printf("inv(w = 1e-25) = (%g, %g, %g, %g)\n", q.x, q.y, q.z, q.w);
	return 0;
}
```

```text
inv(0)         = (0, 0, 0, 0)
inv(w = 1e-25) = (-0, -0, -0, 1e+25)
```

## Suggested fix

Check the squared norm: zero, return the input; outside a safe range such as 1e-30 to
1e30, divide by the length twice.  Dividing instead of multiplying by the reciprocal also
removes a rounding ([12](12-precision-small-differences.md)).

## Checking

`compare/check_reports.py docs/reports/cglm/11-quat-inv-zero.md` builds both programs above and compares their output with this report.
