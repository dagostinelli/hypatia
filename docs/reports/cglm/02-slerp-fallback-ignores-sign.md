# `glm_quat_slerp` returns a near-zero quaternion for nearly equal rotations of opposite sign

| | |
|---|---|
| Library | cglm 0.9.4 (1796cc5) |
| Function | `glm_quat_slerp` (`quat.h`) |
| Kind | wrong result |
| Precision | float |
| Status | still present in cglm 0.9.6 (the latest release) and at master 58d8c15 (2026-07-29): the program prints the same |

## Summary

`glm_quat_slerp` negates `from` when the dot product is negative (to take the shorter arc),
but its fallback for small angles (`sinTheta < 0.001`) interpolates between the original
`from` and `to`, ignoring the negation.  For two quaternions that describe nearly the same
rotation with opposite signs (q and -q', which happens whenever quaternions come from
different computations), the halfway point is close to the zero quaternion: length 0.00025
in the example.  Normalized, it is an arbitrary rotation.

## Reproduction

```c
#include <cglm/cglm.h>
#include <math.h>
#include <stdio.h>

int main(void)
{
	versor a, b, r;

	glm_quat(a, 1.0f, 0, 0, 1);      /* 1 rad about Z */
	glm_quat(b, 1.001f, 0, 0, 1);    /* 1.001 rad about Z */
	glm_vec4_negate(b);              /* -b: the same rotation */
	glm_quat_slerp(a, b, 0.5f, r);
	printf("slerp(a, -b, 0.5) = (%g, %g, %g, %g), length %g\n", r[0], r[1], r[2], r[3], glm_quat_norm(r));
	return 0;
}
```

Output (x86-64, gcc 13.3, `-O2`):

```text
slerp(a, -b, 0.5) = (0, 0, -0.000219375, 0.000119925), length 0.000250014
```

Expected: the rotation by 1.0005 rad about Z: (0, 0, 0.47965, 0.87745) or its negative, of unit length.

## Cause

[`quat.h` lines 715-734](https://github.com/recp/cglm/blob/1796cc5ce298235b615dc7a4750b8c3ba56a05dd/include/cglm/quat.h#L715-L734):

```
if (cosTheta < 0.0f) {
  glm_vec4_negate(q1);          /* q1 is the copy of from */
  cosTheta = -cosTheta;
}
sinTheta = sqrtf(1.0f - cosTheta * cosTheta);
/* LERP to avoid zero division */
if (fabsf(sinTheta) < 0.001f) {
  glm_quat_lerp(from, to, t, dest);   /* from, not q1 */
  return;
}
```

## How hypatia does it

`quaternion_slerp` negates the target when the dot product is negative and uses the
negated quaternion in every branch.  It measures the angle as `2 atan2(|s - t|, |s + t|)`
of the unit quaternions, which is accurate down to zero, so it needs no small-angle
fallback: the linear form is used only when the angle is exactly 0.

```c
#define HYPATIA_SINGLE_PRECISION_FLOATS
#define HYPATIA_IMPLEMENTATION
#include <float.h>
#include <math.h>
#include <stdio.h>
#include "hypatia.h"

int main(void)
{
	struct quaternion a;
	struct quaternion b;
	struct quaternion r;

	quaternion_set_from_axis_anglev3(&a, HYP_VECTOR3_UNIT_Z, 1.0f);
	quaternion_set_from_axis_anglev3(&b, HYP_VECTOR3_UNIT_Z, 1.001f);
	quaternion_negate(&b);
	quaternion_slerp(&a, &b, 0.5f, &r);
	printf("slerp(a, -b, 0.5) = (%g, %g, %g, %g), length %g\n", r.x, r.y, r.z, r.w, quaternion_magnitude(&r));
	return 0;
}
```

```text
slerp(a, -b, 0.5) = (0, 0, 0.479645, 0.877463), length 1
```

## Suggested fix

`glm_quat_lerp(q1, to, t, dest);` in the fallback, and normalize its result (see
[03](03-slerp-small-angles.md)).

## Checking

`compare/check_reports.py docs/reports/cglm/02-slerp-fallback-ignores-sign.md` builds both programs above and compares their output with this report.
