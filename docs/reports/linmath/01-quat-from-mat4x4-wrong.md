# `quat_from_mat4x4` returns the wrong rotation: the identity becomes a half turn about x

| | |
|---|---|
| Library | linmath.h master 3eef828 (2022-06-19) |
| Function | `quat_from_mat4x4` |
| Kind | wrong result |
| Precision | float |
| Status | present at master 3eef828 (2022-06-19, the latest commit; linmath.h has no releases) |

## Summary

`quat_from_mat4x4` gives a wrong quaternion for ordinary rotation matrices.  The identity
matrix gives (1, 0, 0, 0), a half turn about x; a rotation by 0.5 rad about z gives
(0.247, 0, 0, -0.969), a rotation about x; 0.5 rad about x gives (1, 0, 0, 0) again.
Converting the result back with `mat4x4_from_quat` does not give the input matrix.

## Reproduction

```c
#include <linmath.h>
#include <stdio.h>

static void show(const char *what, mat4x4 M)
{
	quat q;
	mat4x4 back;
	float d = 0;
	int i, j;

	quat_from_mat4x4(q, M);
	mat4x4_from_quat(back, q);
	for (i = 0; i < 3; i++)
		for (j = 0; j < 3; j++)
			d = fabsf(back[i][j] - M[i][j]) > d ? fabsf(back[i][j] - M[i][j]) : d;
	printf("%s: q = (%g, %g, %g, %g), back to a matrix: largest difference %g\n", what, q[0], q[1], q[2], q[3], d);
}

int main(void)
{
	mat4x4 id, m;

	mat4x4_identity(id);
	show("identity       ", id);
	mat4x4_rotate(m, id, 0, 0, 1, 0.5f);
	show("0.5 rad about z", m);
	mat4x4_rotate(m, id, 1, 0, 0, 0.5f);
	show("0.5 rad about x", m);
	return 0;
}
```

Output (x86-64, gcc 13.3, `-O2`):

```text
identity       : q = (1, 0, 0, 0), back to a matrix: largest difference 2
0.5 rad about z: q = (0.247404, 0, 0, -0.968912), back to a matrix: largest difference 0.479426
0.5 rad about x: q = (1, 0, 0, 0), back to a matrix: largest difference 1.87758
```

Expected: (0, 0, 0, 1) for the identity, (0, 0, 0.247, 0.969) and (0.247, 0, 0, 0.969) for
the rotations (linmath stores w last, as `quat_identity` shows), and a difference of
the order of 1e-7 back to the matrix.

## Cause

[`linmath.h` lines 546-575](https://github.com/datenwolf/linmath.h/blob/3eef82841046507e16a0f6194a61cee2eadd34b3/linmath.h#L546-L575):

```
for(i = 0; i<3; i++) {
	float m = M[i][i];
	if( m < r )
		continue;
	m = r;
	p = &perm[i];
}
```

assigns `m = r` where `r = m` is meant, so `r` stays 0 and the pivot is the last
non-negative diagonal entry, not the largest.  The results are then stored in
`q[0]` .. `q[3]` whatever the pivot, with `r / 2` in `q[0]` (x), and `q[1]`, `q[2]`
use differences of off-diagonal entries where Shepperd's method uses sums.

## Suggested fix

Use the standard method: pick the largest of the trace and the three diagonal entries,
compute that component from `sqrt(1 + ...)`, and the other three from the sums and
differences of the off-diagonal pairs, storing each in its own slot (w in `q[3]`).

## How hypatia does it

`quaternion_set_from_matrix4` uses the trace when it is positive and otherwise the largest
diagonal entry (Shepperd's method), and computes each component in its own slot;
converting back with `matrix4_set_from_quaternion` gives the input matrix.

```c
#define HYPATIA_SINGLE_PRECISION_FLOATS
#define HYPATIA_IMPLEMENTATION
#include <math.h>
#include <stdio.h>
#include "hypatia.h"

static void show(const char *what, const struct matrix4 *m)
{
	struct quaternion q;
	struct matrix4 back;
	float d = 0;
	int i;

	quaternion_set_from_matrix4(&q, m);
	matrix4_set_from_quaternion(&back, &q);
	for (i = 0; i < 16; i++)
		d = fabsf(back.m[i] - m->m[i]) > d ? fabsf(back.m[i] - m->m[i]) : d;
	printf("%s: q = (%g, %g, %g, %g), back to a matrix: largest difference %g\n", what, q.x, q.y, q.z, q.w, d);
}

int main(void)
{
	struct matrix4 m;
	struct vector3 axis;

	show("identity       ", matrix4_identity(&m));
	show("0.5 rad about z", matrix4_set_from_axisv3_angle(&m, vector3_setf3(&axis, 0, 0, 1), 0.5f));
	show("0.5 rad about x", matrix4_set_from_axisv3_angle(&m, vector3_setf3(&axis, 1, 0, 0), 0.5f));
	return 0;
}
```

```text
identity       : q = (0, 0, 0, 1), back to a matrix: largest difference 0
0.5 rad about z: q = (0, 0, 0.247404, 0.968912), back to a matrix: largest difference 0
0.5 rad about x: q = (0.247404, 0, 0, 0.968912), back to a matrix: largest difference 0
```

## Checking

`compare/check_reports.py docs/reports/linmath/01-quat-from-mat4x4-wrong.md` builds both
programs above and compares their output with this report.
