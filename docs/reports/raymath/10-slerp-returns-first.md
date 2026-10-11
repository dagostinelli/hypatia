# `QuaternionSlerp` returns its first argument when the dot product rounds to 1

| | |
|---|---|
| Library | raymath (raylib 6.0, dbc56a8) |
| Function | `QuaternionSlerp` |
| Kind | wrong result (accuracy) |
| Precision | float |
| Status | still present in raylib 6.0 (the latest release) and at master 453cc35 (2026-10-10): the program prints the same |

## Summary

`QuaternionSlerp` returns `q1` unchanged, without interpolating, when
`fabsf(cosHalfTheta) >= 1.0f`.  For nearby float quaternions the rounding of the inputs and
of the dot product can make it exactly 1.  Over 20000 random pairs of unit quaternions
1e-3 rad apart, the program below gets `q1` back unchanged for 1620 pairs; 1e-4 rad apart,
for 15490.  For those pairs the result does not move at all: the angle error is the whole
of t x angle, as large as the angle itself.  hypatia's slerp, on the same inputs, returns
q1 only twice (for t so small that the step rounds away), and its largest error, 0.0022
of the angle at 1e-4 rad, is the float rounding of the inputs (6e-8 against 1e-4).

## Reproduction

```c
#define RAYMATH_STATIC_INLINE
#include <raymath.h>
#include <math.h>
#include <stdio.h>

static unsigned long long state = 88172645463325252ULL;
static double rnd(void) { state ^= state << 13; state ^= state >> 7; state ^= state << 17; return (state >> 11) * (1.0 / 9007199254740992.0); }

int main(void)
{
	double angles[] = {1e-3, 1e-4};
	int a, i, k;

	for (a = 0; a < 2; a++) {
		int stuck = 0;
		double largest = 0, h = angles[a] / 2;
		for (i = 0; i < 20000; i++) {
			double q[4], d[4], n = 0, m = 0, got;
			float t = (float)rnd();
			Quaternion qa, qb, r;
			for (k = 0; k < 4; k++) { q[k] = 2 * rnd() - 1; n += q[k] * q[k]; }
			for (k = 0; k < 3; k++) { d[k] = 2 * rnd() - 1; m += d[k] * d[k]; }
			for (k = 0; k < 4; k++) q[k] /= sqrt(n);
			for (k = 0; k < 3; k++) d[k] *= sin(h) / sqrt(m);
			d[3] = cos(h);
			qa = (Quaternion){(float)q[0], (float)q[1], (float)q[2], (float)q[3]};
			qb = QuaternionMultiply(qa, (Quaternion){(float)d[0], (float)d[1], (float)d[2], (float)d[3]});
			r = QuaternionSlerp(qa, qb, t);
			stuck += r.x == qa.x && r.y == qa.y && r.z == qa.z && r.w == qa.w && t > 0;
			/* the angle from qa to r, against t times the angle */
			r = QuaternionMultiply(QuaternionInvert(qa), r);
			got = 2 * atan2(sqrt(r.x * r.x + r.y * r.y + r.z * r.z), fabs(r.w));
			largest = fmax(largest, fabs(got - t * angles[a]) / angles[a]);
		}
		printf("%g rad apart: result equal to q1 for %d of 20000, largest error %.2g of the angle\n", angles[a], stuck, largest);
	}
	return 0;
}
```

Output (x86-64, gcc 13.3, `-O2`):

```text
0.001 rad apart: result equal to q1 for 1620 of 20000, largest error 1 of the angle
0.0001 rad apart: result equal to q1 for 15490 of 20000, largest error 1 of the angle
```

Expected: the result moves by t times the angle, with an error of the order of the float
rounding of the inputs.

## Cause

[`raymath.h` lines 2306-2349](https://github.com/raysan5/raylib/blob/dbc56a87da87d973a9c5baa4e7438a9d20121d28/src/raymath.h#L2306-L2349):
`if (fabsf(cosHalfTheta) >= 1.0f) result = q1;`.

## Suggested fix

Drop the early return.  Compute the angle as `2 atan2(|a - b|, |a + b|)`, which does not
round to 0 when the dot product rounds to 1, and use the slerp weights for every nonzero
angle.

## How hypatia does it

`quaternion_slerp` takes the angle from `2 atan2(|q - r|, |q + r|)` and has no early
return.

```c
#define HYPATIA_SINGLE_PRECISION_FLOATS
#define HYPATIA_IMPLEMENTATION
#include <math.h>
#include <stdio.h>
#include "hypatia.h"

static unsigned long long state = 88172645463325252ULL;
static double rnd(void) { state ^= state << 13; state ^= state >> 7; state ^= state << 17; return (state >> 11) * (1.0 / 9007199254740992.0); }

int main(void)
{
	double angles[] = {1e-3, 1e-4};
	int a, i, k;

	for (a = 0; a < 2; a++) {
		int stuck = 0;
		double largest = 0, h = angles[a] / 2;
		for (i = 0; i < 20000; i++) {
			double q[4], d[4], n = 0, m = 0, got;
			float t = (float)rnd();
			struct quaternion qa, qb, r, step;
			for (k = 0; k < 4; k++) { q[k] = 2 * rnd() - 1; n += q[k] * q[k]; }
			for (k = 0; k < 3; k++) { d[k] = 2 * rnd() - 1; m += d[k] * d[k]; }
			for (k = 0; k < 4; k++) q[k] /= sqrt(n);
			for (k = 0; k < 3; k++) d[k] *= sin(h) / sqrt(m);
			d[3] = cos(h);
			quaternion_setf4(&qa, (float)q[0], (float)q[1], (float)q[2], (float)q[3]);
			quaternion_setf4(&step, (float)d[0], (float)d[1], (float)d[2], (float)d[3]);
			quaternion_multiply(quaternion_set(&qb, &qa), &step);
			quaternion_slerp(&qa, &qb, t, &r);
			stuck += r.x == qa.x && r.y == qa.y && r.z == qa.z && r.w == qa.w && t > 0;
			/* the angle from qa to r, against t times the angle */
			quaternion_multiply(quaternion_inverse(quaternion_set(&step, &qa)), &r);
			got = 2 * atan2(sqrt(step.x * step.x + step.y * step.y + step.z * step.z), fabs(step.w));
			largest = fmax(largest, fabs(got - t * angles[a]) / angles[a]);
		}
		printf("%g rad apart: result equal to q1 for %d of 20000, largest error %.2g of the angle\n", angles[a], stuck, largest);
	}
	return 0;
}
```

```text
0.001 rad apart: result equal to q1 for 0 of 20000, largest error 0.00023 of the angle
0.0001 rad apart: result equal to q1 for 2 of 20000, largest error 0.0022 of the angle
```

## Checking

`compare/check_reports.py docs/reports/raymath/10-slerp-returns-first.md` builds both
programs above and compares their output with this report.
