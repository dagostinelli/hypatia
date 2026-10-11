# `mat4x4_invert` gives inf or 0 for matrices with small or large entries

| | |
|---|---|
| Library | linmath.h master 3eef828 (2022-06-19) |
| Function | `mat4x4_invert` |
| Kind | wrong result |
| Precision | float |
| Status | present at master 3eef828 (2022-06-19, the latest commit; linmath.h has no releases) |

## Summary

The inverse divides the adjugate by the determinant through its reciprocal, \`1 / det\`.
For a matrix with small entries the determinant underflows and the reciprocal is inf; for
large entries the determinant overflows and the reciprocal is 0.  The inverse itself is in
range in both cases.  s times the 4x4 identity, which has condition number 1, shows it:
its determinant is s^4, so in float the inverse of 1e-10 I is inf instead of 1e10 I, and
the inverse of 1e10 I is 0 instead of 1e-10 I.

## Reproduction

```c
#include <linmath.h>
#include <stdio.h>

int main(void)
{
	float scales[] = {1e-10f, 1e10f};
	int i;

	for (i = 0; i < 2; i++) {
		mat4x4 m, inverse;

		mat4x4_identity(m);
		m[0][0] = m[1][1] = m[2][2] = m[3][3] = scales[i];
		mat4x4_invert(inverse, m);
		printf("mat4x4_invert(%g I): m00 = %g\n", scales[i], inverse[0][0]);
	}
	return 0;
}
```

Output (x86-64, gcc 13.3, \`-O2\`):

```text
mat4x4_invert(1e-10 I): m00 = inf
mat4x4_invert(1e+10 I): m00 = 0
```

Expected: m00 = 1 / s: 1e10 and 1e-10.

## Cause

[`linmath.h` line 297](https://github.com/datenwolf/linmath.h/blob/3eef82841046507e16a0f6194a61cee2eadd34b3/linmath.h#L297):
`float idet = 1.0f/( s[0]*c[5]-s[1]*c[4]+s[2]*c[3]+s[3]*c[2]-s[4]*c[1]+s[5]*c[0] );`, and
every entry is multiplied by `idet`.

## Suggested fix

Scale the matrix by a power of two so that its largest entry is near 1, invert, and scale
the inverse by the same power of two: the powers of two are exact, and the determinant of
the scaled matrix stays in range whenever the inverse is.

## How hypatia does it

\`matrix4_inverse\` (and \`matrix2_inverse\`, \`matrix3_inverse\`) scale the matrix by a power
of two so that its largest entry is at least 1 and less than 2, invert it, dividing by the
determinant rather than multiplying by its reciprocal, and scale the result back.

```c
#define HYPATIA_SINGLE_PRECISION_FLOATS
#define HYPATIA_IMPLEMENTATION
#include <stdio.h>
#include "hypatia.h"

int main(void)
{
	float scales[] = {1e-10f, 1e10f};
	int i;

	for (i = 0; i < 2; i++) {
		struct matrix4 m, inverse;

		matrix4_multiplyf(matrix4_identity(&m), scales[i]);
		matrix4_inverse(&m, &inverse);
		printf("matrix4_inverse(%g I): m00 = %g\n", scales[i], inverse.m[0]);
	}
	return 0;
}
```

```text
matrix4_inverse(1e-10 I): m00 = 1e+10
matrix4_inverse(1e+10 I): m00 = 1e-10
```

## Checking

\`compare/check_reports.py docs/reports/linmath/06-invert-tiny-huge-scale.md\` builds the programs above and
compares their output with this report.
