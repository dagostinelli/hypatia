# `HMM_SLerp` uses a normalized lerp up to 0.063 rad: angle errors 120 times the rounding

| | |
|---|---|
| Library | HandmadeMath master 661fef0 (2026-03-17) |
| Function | `HMM_SLerp` |
| Kind | precision |
| Precision | float |
| Status | still present in HandmadeMath v2.0.0 (the latest release, 2023-02-20) and at master 661fef0 (2026-03-17): the program prints the same |

## Summary

`HMM_SLerp` switches to `HMM_NLerp` when the dot product of the quaternions is above
0.9995, that is for rotations up to 2 acos(0.9995) = 0.063 rad apart.  A normalized lerp
does not move at constant angular speed, so the angle it produces at parameter t is not t
times the angle.  For rotations 0.06 rad apart the program below finds an angle error of
8.7e-7 rad, about 120 times the float rounding of the angle (7e-9 rad); hypatia's slerp,
on the same inputs, 5e-9 rad.

## Reproduction

```c
#include <HandmadeMath.h>
#include <math.h>
#include <stdio.h>

int main(void)
{
	double angle = 0.06, t, largest = 0;
	HMM_Quat a = HMM_Q(0, 0, 0, 1), b = HMM_Q(0, 0, (float)sin(angle / 2), (float)cos(angle / 2));

	for (t = 0.05; t < 1; t += 0.05) {
		HMM_Quat r = HMM_SLerp(a, (float)t, b);
		double e = fabs(2 * atan2(r.Z, r.W) - t * angle);
		largest = e > largest ? e : largest;
	}
	printf("HMM_SLerp, 0.06 rad apart: largest angle error %.2g rad\n", largest);
	return 0;
}
```

Output (x86-64, gcc 13.3, `-O2`):

```text
HMM_SLerp, 0.06 rad apart: largest angle error 8.7e-07 rad
```

Expected: an angle error of the order of the float rounding of the angle, 0.06 x 1.2e-7 =
7e-9 rad.

## Cause

[`HandmadeMath.h` lines 2375-2401](https://github.com/HandmadeMath/HandmadeMath/blob/661fef0893bccfe30342049e848b8d54e7430234/HandmadeMath.h#L2375-L2401):
`if (Cos_Theta > 0.9995f) { Result = HMM_NLerp(Left, Time, Right); }`.  The threshold
avoids dividing by a small `sin(theta)`, but the slerp formula does not need it there:
`sin((1 - t) theta) / sin(theta)` and `sin(t theta) / sin(theta)` are accurate for small
theta when theta itself is accurate.

## Suggested fix

Compute theta as `2 atan2(|a - b|, |a + b|)` (accurate for small angles, unlike `acos`),
use the slerp weights for every theta above zero, and fall back to the lerp only when
theta is 0.

## How hypatia does it

`quaternion_slerp` takes the angle from `2 atan2(|q - r|, |q + r|)` and uses the slerp
weights for every nonzero angle.

```c
#define HYPATIA_SINGLE_PRECISION_FLOATS
#define HYPATIA_IMPLEMENTATION
#include <math.h>
#include <stdio.h>
#include "hypatia.h"

int main(void)
{
	double angle = 0.06, t, largest = 0;
	struct quaternion a, b, r;

	quaternion_setf4(&a, 0, 0, 0, 1);
	quaternion_setf4(&b, 0, 0, (float)sin(angle / 2), (float)cos(angle / 2));
	for (t = 0.05; t < 1; t += 0.05) {
		double e;

		quaternion_slerp(&a, &b, (float)t, &r);
		e = fabs(2 * atan2(r.z, r.w) - t * angle);
		largest = e > largest ? e : largest;
	}
	printf("quaternion_slerp, 0.06 rad apart: largest angle error %.2g rad\n", largest);
	return 0;
}
```

```text
quaternion_slerp, 0.06 rad apart: largest angle error 5e-09 rad
```

## Checking

`compare/check_reports.py docs/reports/handmademath/05-slerp-nlerp-threshold.md` builds both
programs above and compares their output with this report.
