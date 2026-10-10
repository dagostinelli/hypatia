# `glm::rotation` loses most of its digits for nearly opposite vectors

| | |
|---|---|
| Library | GLM 1.0.1 (0af55cc) |
| Function | `glm::rotation(orig, dest)` (`gtx/quaternion.hpp`) |
| Kind | precision |
| Precision | double and float |
| Status | still present in GLM 1.0.3 (the latest release) and at master 6f14f47 (2026-04-07): the program prints the same |

## Summary

For unit vectors 1e-3 rad from opposite, the rotation from `glm::rotation` puts `orig` up to
2100 double epsilons away from `dest` in the program below (mean 330); over the 20000
inputs of the comparison harness the largest is 4090 (mean 840).  The cause is the
cancellation in `1 + cosTheta` and in the cross product of nearly opposite vectors.  The
same inputs give 1.8 epsilons at most with hypatia (2.2 in the harness).  On random inputs
GLM's largest error in the harness is 116 epsilons, hypatia's 2.2.

## Reproduction

```cpp
// glm::rotation for unit vectors 1e-3 rad from opposite: how far it lands from dest
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <cfloat>
#include <cmath>
#include <cstdio>

static unsigned long long state = 88172645463325252ULL;
static double rnd() { state ^= state << 13; state ^= state >> 7; state ^= state << 17; return (state >> 11) * (1.0 / 9007199254740992.0); }

/* how far the rotation by q (normalized) puts unit f from unit t, in long double */
static long double landing(const double f[3], const double t[3], const double q[4] /* x y z w */)
{
	long double n = sqrtl((long double)q[0] * q[0] + (long double)q[1] * q[1] + (long double)q[2] * q[2] + (long double)q[3] * q[3]);
	long double u[3] = {q[0] / n, q[1] / n, q[2] / n}, w = q[3] / n, nf = 0, nt = 0, e = 0;
	for (int i = 0; i < 3; i++) { nf += (long double)f[i] * f[i]; nt += (long double)t[i] * t[i]; }
	long double v[3] = {f[0] / sqrtl(nf), f[1] / sqrtl(nf), f[2] / sqrtl(nf)};
	long double c[3] = {u[1] * v[2] - u[2] * v[1], u[2] * v[0] - u[0] * v[2], u[0] * v[1] - u[1] * v[0]};
	long double cc[3] = {u[1] * c[2] - u[2] * c[1], u[2] * c[0] - u[0] * c[2], u[0] * c[1] - u[1] * c[0]};
	for (int i = 0; i < 3; i++) { long double r = v[i] + 2 * (w * c[i] + cc[i]) - t[i] / sqrtl(nt); e += r * r; }
	return sqrtl(e);
}

/* a unit vector, and one at angle a from its opposite, both rounded to double */
static void near_opposite(double f[3], double t[3], long double a)
{
	long double v[3], p[3], n = 0, m = 0;
	for (int i = 0; i < 3; i++) { v[i] = 2 * rnd() - 1; n += v[i] * v[i]; }
	for (int i = 0; i < 3; i++) v[i] /= sqrtl(n);
	p[0] = v[1]; p[1] = -v[0]; p[2] = 0;                  /* perpendicular to v */
	for (int i = 0; i < 3; i++) m += p[i] * p[i];
	for (int i = 0; i < 3; i++) { f[i] = (double)v[i]; t[i] = (double)(-v[i] * cosl(a) + p[i] / sqrtl(m) * sinl(a)); }
}

int main()
{
	long double largest = 0, sum = 0;
	for (int i = 0; i < 2000; i++) {
		double f[3], t[3];
		near_opposite(f, t, 1e-3L);
		glm::dquat g = glm::rotation(glm::dvec3(f[0], f[1], f[2]), glm::dvec3(t[0], t[1], t[2]));
		double q[4] = {g.x, g.y, g.z, g.w};
		long double e = landing(f, t, q) / DBL_EPSILON;
		largest = e > largest ? e : largest;
		sum += e;
	}
	std::printf("glm::rotation, 1e-3 rad from opposite: largest %.0Lf, mean %.0Lf epsilons\n", largest, sum / 2000);
}
```

Output (x86-64, gcc 13.3, `-O2`):

```text
glm::rotation, 1e-3 rad from opposite: largest 2097, mean 328 epsilons
```

Expected: an error of a few epsilons, as for other inputs.

## Cause

[`gtx/quaternion.inl` lines 148-157](https://github.com/g-truc/glm/blob/0af55ccecd98d4e5a8d1fad7de25ba429d60e863/glm/gtx/quaternion.inl#L148-L157):

```
rotationAxis = cross(orig, dest);

T s = sqrt((T(1) + cosTheta) * static_cast<T>(2));
T invs = static_cast<T>(1) / s;

return qua<T, Q>::wxyz(
	s * static_cast<T>(0.5f),
	rotationAxis.x * invs,
	rotationAxis.y * invs,
	rotationAxis.z * invs);
```

For nearly opposite unit vectors `cosTheta` is close to -1 and `1 + cosTheta` keeps only
the digits of `cosTheta` that are not cancelled; `cross(orig, dest)` is small and carries
the rounding of both inputs.  Both errors are divided by the small `s`.

## How hypatia does it

`quaternion_get_rotation_tov3` normalizes both vectors and takes the half angle from two
lengths that stay accurate for any angle: for unit vectors at angle a,
|f + t| = 2 cos(a/2) and |f - t| = 2 sin(a/2).  The axis is f x (f + t), which has the
direction of f x t but stays accurate as the vectors become opposite.  There is no
threshold: only an exactly zero axis (vectors exactly parallel or opposite after rounding)
is a special case.  The result is normalized, so it is always a unit quaternion.

```c
#define HYPATIA_IMPLEMENTATION
#include <float.h>
#include <math.h>
#include <stdio.h>
#include "hypatia.h"

static unsigned long long state = 88172645463325252ULL;
static double rnd(void) { state ^= state << 13; state ^= state >> 7; state ^= state << 17; return (state >> 11) * (1.0 / 9007199254740992.0); }

/* how far the rotation by q (normalized) puts unit f from unit t, in long double */
static long double landing(const double f[3], const double t[3], const double q[4] /* x y z w */)
{
	long double n = sqrtl((long double)q[0] * q[0] + (long double)q[1] * q[1] + (long double)q[2] * q[2] + (long double)q[3] * q[3]);
	long double u[3] = {q[0] / n, q[1] / n, q[2] / n}, w = q[3] / n, nf = 0, nt = 0, e = 0;
	for (int i = 0; i < 3; i++) { nf += (long double)f[i] * f[i]; nt += (long double)t[i] * t[i]; }
	long double v[3] = {f[0] / sqrtl(nf), f[1] / sqrtl(nf), f[2] / sqrtl(nf)};
	long double c[3] = {u[1] * v[2] - u[2] * v[1], u[2] * v[0] - u[0] * v[2], u[0] * v[1] - u[1] * v[0]};
	long double cc[3] = {u[1] * c[2] - u[2] * c[1], u[2] * c[0] - u[0] * c[2], u[0] * c[1] - u[1] * c[0]};
	for (int i = 0; i < 3; i++) { long double r = v[i] + 2 * (w * c[i] + cc[i]) - t[i] / sqrtl(nt); e += r * r; }
	return sqrtl(e);
}

/* a unit vector, and one at angle a from its opposite, both rounded to double */
static void near_opposite(double f[3], double t[3], long double a)
{
	long double v[3], p[3], n = 0, m = 0;
	for (int i = 0; i < 3; i++) { v[i] = 2 * rnd() - 1; n += v[i] * v[i]; }
	for (int i = 0; i < 3; i++) v[i] /= sqrtl(n);
	p[0] = v[1]; p[1] = -v[0]; p[2] = 0;                  /* perpendicular to v */
	for (int i = 0; i < 3; i++) m += p[i] * p[i];
	for (int i = 0; i < 3; i++) { f[i] = (double)v[i]; t[i] = (double)(-v[i] * cosl(a) + p[i] / sqrtl(m) * sinl(a)); }
}

int main(void)
{
	long double largest = 0, sum = 0;
	int i;
	for (i = 0; i < 2000; i++) {
		double f[3], t[3];
		struct vector3 from, to;
		struct quaternion h;
		near_opposite(f, t, 1e-3L);
		vector3_setf3(&from, f[0], f[1], f[2]);
		vector3_setf3(&to, t[0], t[1], t[2]);
		quaternion_get_rotation_tov3(&from, &to, &h);
		{
			double q[4] = {h.x, h.y, h.z, h.w};
			long double e = landing(f, t, q) / DBL_EPSILON;
			largest = e > largest ? e : largest;
			sum += e;
		}
	}
	printf("quaternion_get_rotation_tov3, 1e-3 rad from opposite: largest %.1Lf, mean %.2Lf epsilons\n", largest, sum / 2000);
	return 0;
}
```

```text
quaternion_get_rotation_tov3, 1e-3 rad from opposite: largest 1.8, mean 0.32 epsilons
```

## Suggested fix

Take the half angle from `length(f + t)` and `length(f - t)` of the unit vectors and the
axis from `cross(f, f + t)`, as described in
[01-rotation-opposite-vectors.md](01-rotation-opposite-vectors.md).  Neither quantity
cancels as the vectors approach opposite.

## Checking

`compare/check_reports.py docs/reports/glm/03-rotation-near-opposite-precision.md` builds both programs above and compares their output with this report. The harness measures the same over 20000 inputs (`results/precision/now.double.md`,
`quaternion_get_rotation_tov3`, "1e-3 rad from opposite" and "random").
