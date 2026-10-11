# `HMM_InvGeneralM4` gives NaN or 0 for matrices with small or large entries

| | |
|---|---|
| Library | HandmadeMath master 661fef0 (2026-03-17) |
| Function | `HMM_InvGeneralM4`, `HMM_InvGeneralM3`, `HMM_InvGeneralM2` |
| Kind | wrong result |
| Precision | float |
| Status | still present in HandmadeMath v2.0.0 (the latest release, 2023-02-20) and at master 661fef0 (2026-03-17): the program prints the same |

## Summary

The inverse divides the adjugate by the determinant through its reciprocal, \`1 / det\`.
For a matrix with small entries the determinant underflows and the reciprocal is inf; for
large entries the determinant overflows and the reciprocal is 0.  The inverse itself is in
range in both cases.  s times the 4x4 identity, which has condition number 1, shows it:
its determinant is s^4, so in float the inverse of 1e-10 I is NaN (the inf reciprocal
times the zero entries of the adjugate) instead of 1e10 I, and the inverse of 1e10 I is 0
instead of 1e-10 I.

## Reproduction

```c
#include <HandmadeMath.h>
#include <stdio.h>

int main(void)
{
	float scales[] = {1e-10f, 1e10f};
	int i;

	for (i = 0; i < 2; i++)
		printf("HMM_InvGeneralM4(%g I): m00 = %g\n", scales[i], HMM_InvGeneralM4(HMM_M4D(scales[i])).Elements[0][0]);
	return 0;
}
```

Output (x86-64, gcc 13.3, \`-O2\`):

```text
HMM_InvGeneralM4(1e-10 I): m00 = -nan
HMM_InvGeneralM4(1e+10 I): m00 = 0
```

Expected: m00 = 1 / s: 1e10 and 1e-10.

## Cause

[`HandmadeMath.h` line 1709](https://github.com/HandmadeMath/HandmadeMath/blob/661fef0893bccfe30342049e848b8d54e7430234/HandmadeMath.h#L1709):
`float InvDeterminant = 1.0f / (HMM_DotV3(C01, B32) + HMM_DotV3(C23, B10));`; the same in
`HMM_InvGeneralM3` ([line 1478](https://github.com/HandmadeMath/HandmadeMath/blob/661fef0893bccfe30342049e848b8d54e7430234/HandmadeMath.h#L1478)) and `HMM_InvGeneralM2` ([line 1289](https://github.com/HandmadeMath/HandmadeMath/blob/661fef0893bccfe30342049e848b8d54e7430234/HandmadeMath.h#L1289)).

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

\`compare/check_reports.py docs/reports/handmademath/08-inverse-tiny-huge-scale.md\` builds the programs above and
compares their output with this report.
