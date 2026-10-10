# `glm_quat_slerp` is inaccurate for small angles: an unnormalized lerp and acos

| | |
|---|---|
| Library | cglm 0.9.4 (1796cc5) |
| Function | `glm_quat_slerp` (`quat.h`) |
| Kind | precision |
| Precision | float |
| Status | still present in cglm 0.9.6 (the latest release) and at master 58d8c15 (2026-07-29): the program prints the same |

## Summary

For quaternions closer than `sinTheta < 0.001` (rotations less than 0.002 rad apart),
`glm_quat_slerp` returns `glm_quat_lerp` without normalizing, so the result is shorter than
unit length by up to 1.3e-7 and not on the arc; above that threshold the angle comes from
`acosf(cosTheta)`, which loses digits near 0.  For rotations 1e-3 rad apart the program
below measures a mean error of 11.7 float epsilons (largest 5839), against 0.46 (largest
1.5) with hypatia; the comparison harness, over other inputs, 52.3 (largest 5890) against
0.47 (largest 1.7).

## Reproduction

```c
#include <cglm/cglm.h>
#include <math.h>
#include <stdio.h>

static unsigned long long state = 88172645463325252ULL;
static double rnd(void) { state ^= state << 13; state ^= state >> 7; state ^= state << 17; return (state >> 11) * (1.0 / 9007199254740992.0); }

int main(void)
{
	long double sum = 0, largest = 0;
	int i, k;
	for (i = 0; i < 20000; i++) {
		long double q[4], n = 0, axis[3], m = 0, h, c, d[4], p[4], o[4], big = 0, e = 0, theta, ta = 0;
		float t = (float)rnd();
		versor a, b, r;
		for (k = 0; k < 4; k++) { q[k] = 2 * rnd() - 1; n += q[k] * q[k]; }
		for (k = 0; k < 3; k++) { axis[k] = 2 * rnd() - 1; m += axis[k] * axis[k]; }
		h = sinl(0.5e-3L) / sqrtl(m); c = cosl(0.5e-3L);                 /* 1e-3 rad apart */
		d[0] = axis[0] * h; d[1] = axis[1] * h; d[2] = axis[2] * h; d[3] = c;
		p[0] = q[3] * d[0] + q[0] * d[3] + q[1] * d[2] - q[2] * d[1];
		p[1] = q[3] * d[1] - q[0] * d[2] + q[1] * d[3] + q[2] * d[0];
		p[2] = q[3] * d[2] + q[0] * d[1] - q[1] * d[0] + q[2] * d[3];
		p[3] = q[3] * d[3] - q[0] * d[0] - q[1] * d[1] - q[2] * d[2];
		for (k = 0; k < 4; k++) { a[k] = (float)(q[k] / sqrtl(n)); b[k] = (float)(p[k] / sqrtl(n)); }
		glm_quat_slerp(a, b, t, r);
		/* the slerp of the rounded inputs, in long double */
		{
			long double na = 0, nb = 0, dot = 0, s2 = 0, c2 = 0, ua[4], ub[4];
			for (k = 0; k < 4; k++) { na += (long double)a[k] * a[k]; nb += (long double)b[k] * b[k]; }
			for (k = 0; k < 4; k++) { ua[k] = a[k] / sqrtl(na); ub[k] = b[k] / sqrtl(nb); dot += ua[k] * ub[k]; }
			for (k = 0; k < 4; k++) { if (dot < 0) ub[k] = -ub[k]; s2 += (ua[k] - ub[k]) * (ua[k] - ub[k]); c2 += (ua[k] + ub[k]) * (ua[k] + ub[k]); }
			theta = 2 * atan2l(sqrtl(s2), sqrtl(c2));
			for (k = 0; k < 4; k++) { o[k] = (sinl((1 - t) * theta) * ua[k] + sinl(t * theta) * ub[k]) / sinl(theta); big = fmaxl(big, fabsl(o[k])); }
			for (k = 0; k < 4; k++) { e = fmaxl(e, fabsl(r[k] - o[k])); ta = fmaxl(ta, fabsl(r[k] + o[k])); }
			e = fminl(e, ta) / big / FLT_EPSILON;   /* q or -q */
		}
		sum += e;
		largest = e > largest ? e : largest;
	}
	printf("glm_quat_slerp, 1e-3 rad apart: largest %.0Lf, mean %.1Lf epsilons\n", largest, sum / 20000);
	return 0;
}
```

Output (x86-64, gcc 13.3, `-O2`):

```text
glm_quat_slerp, 1e-3 rad apart: largest 5839, mean 11.7 epsilons
```

Expected: an error of about an epsilon.

## Cause

[`quat.h` lines 728-742](https://github.com/recp/cglm/blob/1796cc5ce298235b615dc7a4750b8c3ba56a05dd/include/cglm/quat.h#L728-L742): `sinTheta = sqrtf(1 - cosTheta^2)` and
`angle = acosf(cosTheta)` both lose about half the digits when `cosTheta` is near 1; below
`sinTheta = 0.001` the fallback `glm_quat_lerp` is neither normalized nor on the arc.

## How hypatia does it

`quaternion_slerp` takes the angle from `2 atan2(|s - t|, |s + t|)` of the unit
quaternions; both lengths are accurate for nearly equal quaternions, so the full slerp
formula is used down to an angle of exactly 0.

```c
#define HYPATIA_SINGLE_PRECISION_FLOATS
#define HYPATIA_IMPLEMENTATION
#include <float.h>
#include <math.h>
#include <stdio.h>
#include "hypatia.h"

static unsigned long long state = 88172645463325252ULL;
static double rnd(void) { state ^= state << 13; state ^= state >> 7; state ^= state << 17; return (state >> 11) * (1.0 / 9007199254740992.0); }

int main(void)
{
	long double sum = 0, largest = 0;
	int i, k;
	for (i = 0; i < 20000; i++) {
		long double q[4], n = 0, axis[3], m = 0, h, c, d[4], p[4], o[4], big = 0, e = 0, theta, ta = 0;
		float t = (float)rnd();
		float a[4], b[4], r[4];
		struct quaternion ha, hb, hr;
		for (k = 0; k < 4; k++) { q[k] = 2 * rnd() - 1; n += q[k] * q[k]; }
		for (k = 0; k < 3; k++) { axis[k] = 2 * rnd() - 1; m += axis[k] * axis[k]; }
		h = sinl(0.5e-3L) / sqrtl(m); c = cosl(0.5e-3L);
		d[0] = axis[0] * h; d[1] = axis[1] * h; d[2] = axis[2] * h; d[3] = c;
		p[0] = q[3] * d[0] + q[0] * d[3] + q[1] * d[2] - q[2] * d[1];
		p[1] = q[3] * d[1] - q[0] * d[2] + q[1] * d[3] + q[2] * d[0];
		p[2] = q[3] * d[2] + q[0] * d[1] - q[1] * d[0] + q[2] * d[3];
		p[3] = q[3] * d[3] - q[0] * d[0] - q[1] * d[1] - q[2] * d[2];
		for (k = 0; k < 4; k++) { a[k] = (float)(q[k] / sqrtl(n)); b[k] = (float)(p[k] / sqrtl(n)); }
		quaternion_setf4(&ha, a[0], a[1], a[2], a[3]);
		quaternion_setf4(&hb, b[0], b[1], b[2], b[3]);
		quaternion_slerp(&ha, &hb, t, &hr);
		r[0] = hr.x; r[1] = hr.y; r[2] = hr.z; r[3] = hr.w;
		{
			long double na = 0, nb = 0, dot = 0, s2 = 0, c2 = 0, ua[4], ub[4];
			for (k = 0; k < 4; k++) { na += (long double)a[k] * a[k]; nb += (long double)b[k] * b[k]; }
			for (k = 0; k < 4; k++) { ua[k] = a[k] / sqrtl(na); ub[k] = b[k] / sqrtl(nb); dot += ua[k] * ub[k]; }
			for (k = 0; k < 4; k++) { if (dot < 0) ub[k] = -ub[k]; s2 += (ua[k] - ub[k]) * (ua[k] - ub[k]); c2 += (ua[k] + ub[k]) * (ua[k] + ub[k]); }
			theta = 2 * atan2l(sqrtl(s2), sqrtl(c2));
			for (k = 0; k < 4; k++) { o[k] = (sinl((1 - t) * theta) * ua[k] + sinl(t * theta) * ub[k]) / sinl(theta); big = fmaxl(big, fabsl(o[k])); }
			for (k = 0; k < 4; k++) { e = fmaxl(e, fabsl(r[k] - o[k])); ta = fmaxl(ta, fabsl(r[k] + o[k])); }
			e = fminl(e, ta) / big / FLT_EPSILON;
		}
		sum += e;
		largest = e > largest ? e : largest;
	}
	printf("quaternion_slerp, 1e-3 rad apart: largest %.1Lf, mean %.2Lf epsilons\n", largest, sum / 20000);
	return 0;
}
```

```text
quaternion_slerp, 1e-3 rad apart: largest 1.5, mean 0.46 epsilons
```

## Suggested fix

Take the angle from `2 atan2f(length(q1 - to), length(q1 + to))` (with `q1` sign-corrected
and both normalized); then `sin(angle)` is accurate for small angles and the lerp
fallback is needed only for an angle of exactly 0.  If a fallback is kept, normalize it and
use `q1`.

## Checking

`compare/check_reports.py docs/reports/cglm/03-slerp-small-angles.md` builds both programs above and compares their output with this report. The harness: `results/precision/now.single.md`, `quaternion_slerp`, "1e-3 rad apart"
(cglm largest 5890, mean 52.3; hypatia 1.67 / 0.474) and "random" (cglm largest 16.6).
