# `glm_quat_slerp` returns its first argument when the dot product rounds to 1

| | |
|---|---|
| Library | cglm 0.9.4 (1796cc5) |
| Function | `glm_quat_slerp` (`quat.h`) |
| Kind | precision |
| Precision | float |
| Status | still present in cglm 0.9.6 (the latest release) and at master 58d8c15 (2026-07-29): the program prints the same |

## Summary

`glm_quat_slerp` returns `from` unchanged, without interpolating, when
`fabsf(cosTheta) >= 1.0f`.  For nearby float quaternions the rounding of the inputs and of
the dot product can make `cosTheta` exactly 1.  For rotations 1e-3 rad apart (`cosTheta`
is 1 - 1.25e-7 before rounding, about two float ulps below 1) the program below finds this
for 99 of 20000 inputs, and those inputs carry the large errors: mean 2300, largest 5839
float epsilons.  The other 19901 inputs take the `sinTheta < 0.001` branch, an
unnormalized `glm_quat_lerp`, with a mean error of 0.34 and largest 1.2 epsilons; none
reaches the `acosf` branch.  Over all inputs the mean error is 11.7 epsilons (largest
5839), against 0.46 (largest 1.5) with hypatia; the comparison harness, over other inputs,
52.3 (largest 5890) against 0.474 (largest 1.67).  The same program with rotations 1e-6
rad apart (not shown) takes the early return for 17609 of 20000 inputs, mean error 2.32
epsilons (the `glm_quat_slerp` row of [12](12-precision-small-differences.md)).

## Reproduction

```c
#include <cglm/cglm.h>
#include <math.h>
#include <stdio.h>

static unsigned long long state = 88172645463325252ULL;
static double rnd(void) { state ^= state << 13; state ^= state >> 7; state ^= state << 17; return (state >> 11) * (1.0 / 9007199254740992.0); }

int main(void)
{
	long double sum = 0, largest = 0, bsum[3] = {0, 0, 0}, blargest[3] = {0, 0, 0};
	int i, k, branch, count[3] = {0, 0, 0};
	const char *name[3] = {"fabsf(cosTheta) >= 1, returns from", "sinTheta < 0.001, glm_quat_lerp", "acosf and sinf"};
	for (i = 0; i < 20000; i++) {
		long double q[4], n = 0, axis[3], m = 0, h, c, d[4], p[4], o[4], big = 0, e = 0, theta, ta = 0;
		float t = (float)rnd(), cosTheta;
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
		/* the branch glm_quat_slerp takes (its own conditions) */
		cosTheta = fabsf(glm_quat_dot(a, b));
		branch = cosTheta >= 1.0f ? 0 : sqrtf(1.0f - cosTheta * cosTheta) < 0.001f ? 1 : 2;
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
		count[branch]++;
		bsum[branch] += e;
		blargest[branch] = e > blargest[branch] ? e : blargest[branch];
	}
	printf("glm_quat_slerp, 1e-3 rad apart: largest %.0Lf, mean %.1Lf epsilons\n", largest, sum / 20000);
	for (k = 0; k < 3; k++)
		printf("  %-35s %5d inputs, largest %.1Lf, mean %.2Lf\n", name[k], count[k], blargest[k], count[k] ? bsum[k] / count[k] : 0);
	return 0;
}
```

Output (x86-64, gcc 13.3, `-O2`):

```text
glm_quat_slerp, 1e-3 rad apart: largest 5839, mean 11.7 epsilons
  fabsf(cosTheta) >= 1, returns from     99 inputs, largest 5839.4, mean 2300.23
  sinTheta < 0.001, glm_quat_lerp     19901 inputs, largest 1.2, mean 0.34
  acosf and sinf                          0 inputs, largest 0.0, mean 0.00
```

Expected: an error of about an epsilon.

## Cause

[`quat.h` lines 718-721](https://github.com/recp/cglm/blob/1796cc5ce298235b615dc7a4750b8c3ba56a05dd/include/cglm/quat.h#L718-L721):
`if (fabsf(cosTheta) >= 1.0f)` copies `from` to `dest` and returns, so a dot product that
rounds to 1 gives no interpolation at all; the error is then up to the full distance
between the two inputs.  Below `sinTheta = 0.001`
([lines 731-734](https://github.com/recp/cglm/blob/1796cc5ce298235b615dc7a4750b8c3ba56a05dd/include/cglm/quat.h#L731-L734))
the fallback is `glm_quat_lerp`, not normalized and not on the arc; at these angles its
error stays near an epsilon.  Above that threshold
([lines 728-742](https://github.com/recp/cglm/blob/1796cc5ce298235b615dc7a4750b8c3ba56a05dd/include/cglm/quat.h#L728-L742))
`sinTheta = sqrtf(1 - cosTheta^2)` and `angle = acosf(cosTheta)` both lose about half the
digits when `cosTheta` is near 1; the program does not reach that branch.

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
and both normalized), and drop the `fabsf(cosTheta) >= 1.0f` early return; then
`sin(angle)` is accurate for small angles and the lerp fallback is needed only for an
angle of exactly 0.  If a fallback is kept, normalize it and use `q1`.

## Checking

`compare/check_reports.py docs/reports/cglm/03-slerp-small-angles.md` builds both programs above and compares their output with this report. The harness: `results/precision/now.single.md`, `quaternion_slerp`, "1e-3 rad apart"
(cglm largest 5890, mean 52.3; hypatia 1.67 / 0.474) and "random" (cglm largest 16.6).
