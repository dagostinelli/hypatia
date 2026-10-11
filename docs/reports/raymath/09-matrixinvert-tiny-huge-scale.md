# `MatrixInvert` gives inf or 0 for matrices with small or large entries

| | |
|---|---|
| Library | raymath (raylib 6.0, dbc56a8) |
| Function | `MatrixInvert` |
| Kind | wrong result |
| Precision | float |
| Status | still present in raylib 6.0 (the latest release) and at master 453cc35 (2026-10-10): the program prints the same |

## Summary

The inverse divides the adjugate by the determinant through its reciprocal, \`1 / det\`.
For a matrix with small entries the determinant underflows and the reciprocal is inf; for
large entries the determinant overflows and the reciprocal is 0.  The inverse itself is in
range in both cases.  s times the 4x4 identity, which has condition number 1, shows it:
its determinant is s^4, so in float the inverse of 1e-10 I is inf instead of 1e10 I, and
the inverse of 1e10 I is 0 instead of 1e-10 I.

## Reproduction

```c
#define RAYMATH_STATIC_INLINE
#include <raymath.h>
#include <stdio.h>

int main(void)
{
	float scales[] = {1e-10f, 1e10f};
	int i;

	for (i = 0; i < 2; i++) {
		Matrix m = MatrixIdentity();

		m.m0 = m.m5 = m.m10 = m.m15 = scales[i];
		printf("MatrixInvert(%g I): m0 = %g\n", scales[i], MatrixInvert(m).m0);
	}
	return 0;
}
```

Output (x86-64, gcc 13.3, \`-O2\`):

```text
MatrixInvert(1e-10 I): m0 = inf
MatrixInvert(1e+10 I): m0 = 0
```

Expected: m0 = 1 / s: 1e10 and 1e-10.

## Cause

[`raymath.h` line 1598](https://github.com/raysan5/raylib/blob/dbc56a87da87d973a9c5baa4e7438a9d20121d28/src/raymath.h#L1598):
`float invDet = 1.0f/(b00*b11 - b01*b10 + b02*b09 + b03*b08 - b04*b07 + b05*b06);`, and
every entry is multiplied by `invDet`.

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

\`compare/check_reports.py docs/reports/raymath/09-matrixinvert-tiny-huge-scale.md\` builds the programs above and
compares their output with this report.
