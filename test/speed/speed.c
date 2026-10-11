/* SPDX-License-Identifier: MIT */

/* The work each function does, for the speed gate: one bench_ function per
 * hypatia function, each calling it on the same M inputs.  speed.sh runs this
 * under valgrind (callgrind) and divides the instructions counted in each
 * bench_ function by M.  The inputs are prepared outside the bench_ functions,
 * so their preparation is not counted.
 *
 * Needs GCC (noinline).
 */
#define HYPATIA_IMPLEMENTATION
#include "hypatia.h"

#define M 1000

#define BENCH static __attribute__((noinline)) void

static struct vector2 v2a[M], v2b[M];
static struct vector3 v3a[M], v3b[M], v3c[M];
static struct vector4 v4a[M];
static struct quaternion qa[M], qb[M], qu[M], qv[M];
static struct matrix2 m2a[M];
static struct matrix3 m3a[M];
static struct matrix4 m4a[M], m4b[M], m4r[M];
static HYP_FLOAT fa[M], fb[M], fc[M], fd[M];

static volatile HYP_FLOAT sink;

static unsigned long state = 88172645UL;

/* a number in [a, b) */
static HYP_FLOAT uniform(HYP_FLOAT a, HYP_FLOAT b)
{
	state = (state * 1103515245UL + 12345UL) & 0x7fffffffUL;
	return a + (b - a) * ((HYP_FLOAT)state / HYP_FLOAT_C(2147483648.0));
}

static void prepare(void)
{
	int i, k;

	for (i = 0; i < M; i++) {
		vector2_setf2(&v2a[i], uniform(-10, 10), uniform(-10, 10));
		vector2_setf2(&v2b[i], uniform(-10, 10), uniform(-10, 10));
		vector3_setf3(&v3a[i], uniform(-10, 10), uniform(-10, 10), uniform(-10, 10));
		vector3_setf3(&v3b[i], uniform(-10, 10), uniform(-10, 10), uniform(-10, 10));
		vector3_setf3(&v3c[i], uniform(-10, 10), uniform(-10, 10), uniform(-10, 10));
		vector4_setf4(&v4a[i], uniform(-10, 10), uniform(-10, 10), uniform(-10, 10), uniform(-10, 10));
		quaternion_setf4(&qa[i], uniform(-1, 1), uniform(-1, 1), uniform(-1, 1), uniform(-1, 1));
		quaternion_setf4(&qb[i], uniform(-1, 1), uniform(-1, 1), uniform(-1, 1), uniform(-1, 1));
		quaternion_normalize(quaternion_set(&qu[i], &qa[i]));
		quaternion_normalize(quaternion_set(&qv[i], &qb[i]));
		for (k = 0; k < 4; k++) {
			m2a[i].m[k] = uniform(-1, 1);
		}
		for (k = 0; k < 9; k++) {
			m3a[i].m[k] = uniform(-1, 1);
		}
		for (k = 0; k < 16; k++) {
			m4a[i].m[k] = uniform(-1, 1);
			m4b[i].m[k] = uniform(-1, 1);
		}
		/* rotation, scale and translation */
		matrix4_set_from_quaternion(&m4r[i], &qu[i]);
		m4r[i].r03 = uniform(-10, 10);
		m4r[i].r13 = uniform(-10, 10);
		m4r[i].r23 = uniform(-10, 10);
		fa[i] = uniform(0, 1);
		fb[i] = uniform(HYP_FLOAT_C(0.5), 2);
		fc[i] = uniform(HYP_FLOAT_C(0.01), 1);
		fd[i] = uniform(10, 1000);
	}
}

/* ------------------------------------------------------------ vectors */

BENCH bench_vector2_normalize(void)
{
	int i;
	for (i = 0; i < M; i++) {
		struct vector2 v = v2a[i];
		sink = vector2_normalize(&v)->x;
	}
}

BENCH bench_vector3_normalize(void)
{
	int i;
	for (i = 0; i < M; i++) {
		struct vector3 v = v3a[i];
		sink = vector3_normalize(&v)->x;
	}
}

BENCH bench_vector4_normalize(void)
{
	int i;
	for (i = 0; i < M; i++) {
		struct vector4 v = v4a[i];
		sink = vector4_normalize(&v)->x;
	}
}

BENCH bench_vector3_cross_product(void)
{
	int i;
	for (i = 0; i < M; i++) {
		struct vector3 r;
		sink = vector3_cross_product(&r, &v3a[i], &v3b[i])->x;
	}
}

BENCH bench_vector3_project(void)
{
	int i;
	for (i = 0; i < M; i++) {
		struct vector3 v = v3a[i];
		sink = vector3_project(&v, &v3b[i])->x;
	}
}

BENCH bench_vector3_reflect(void)
{
	int i;
	for (i = 0; i < M; i++) {
		struct vector3 v = v3a[i];
		sink = vector3_reflect(&v, &v3b[i])->x;
	}
}

BENCH bench_vector3_refract(void)
{
	int i;
	for (i = 0; i < M; i++) {
		struct vector3 v = v3a[i];
		sink = vector3_refract(&v, &v3b[i], fb[i])->x;
	}
}

BENCH bench_vector2_angle_between(void)
{
	int i;
	for (i = 0; i < M; i++) {
		sink = vector2_angle_between(&v2a[i], &v2b[i]);
	}
}

BENCH bench_vector3_angle_between(void)
{
	int i;
	for (i = 0; i < M; i++) {
		sink = vector3_angle_between(&v3a[i], &v3b[i]);
	}
}

BENCH bench_vector3_rotate_by_quaternion(void)
{
	int i;
	for (i = 0; i < M; i++) {
		struct vector3 v = v3a[i];
		sink = vector3_rotate_by_quaternion(&v, &qu[i])->x;
	}
}

/* ------------------------------------------------------------ quaternions */

BENCH bench_quaternion_normalize(void)
{
	int i;
	for (i = 0; i < M; i++) {
		struct quaternion q = qa[i];
		sink = quaternion_normalize(&q)->x;
	}
}

BENCH bench_quaternion_inverse(void)
{
	int i;
	for (i = 0; i < M; i++) {
		struct quaternion q = qa[i];
		sink = quaternion_inverse(&q)->x;
	}
}

BENCH bench_quaternion_multiply(void)
{
	int i;
	for (i = 0; i < M; i++) {
		struct quaternion q = qa[i];
		sink = quaternion_multiply(&q, &qb[i])->x;
	}
}

BENCH bench_quaternion_angle_between(void)
{
	int i;
	for (i = 0; i < M; i++) {
		sink = quaternion_angle_between(&qu[i], &qv[i]);
	}
}

BENCH bench_quaternion_slerp(void)
{
	int i;
	for (i = 0; i < M; i++) {
		struct quaternion q;
		sink = quaternion_slerp(&qu[i], &qv[i], fa[i], &q)->x;
	}
}

BENCH bench_quaternion_set_from_axis_anglev3(void)
{
	int i;
	for (i = 0; i < M; i++) {
		struct quaternion q;
		sink = quaternion_set_from_axis_anglev3(&q, &v3a[i], fb[i])->x;
	}
}

BENCH bench_quaternion_get_axis_anglev3(void)
{
	int i;
	for (i = 0; i < M; i++) {
		struct vector3 axis;
		HYP_FLOAT angle;
		quaternion_get_axis_anglev3(&qu[i], &axis, &angle);
		sink = angle;
	}
}

BENCH bench_quaternion_set_from_matrix4(void)
{
	int i;
	for (i = 0; i < M; i++) {
		struct quaternion q;
		sink = quaternion_set_from_matrix4(&q, &m4r[i])->x;
	}
}

BENCH bench_quaternion_get_rotation_tov3(void)
{
	int i;
	for (i = 0; i < M; i++) {
		struct quaternion q;
		sink = quaternion_get_rotation_tov3(&v3a[i], &v3b[i], &q)->x;
	}
}

BENCH bench_quaternion_set_look_rotation_rh(void)
{
	int i;
	for (i = 0; i < M; i++) {
		struct quaternion q;
		sink = quaternion_set_look_rotation_rh(&q, &v3a[i], &v3b[i])->x;
	}
}

/* ------------------------------------------------------------ matrices */

BENCH bench_matrix2_inverse(void)
{
	int i;
	for (i = 0; i < M; i++) {
		struct matrix2 r;
		sink = matrix2_inverse(&m2a[i], &r)->m[0];
	}
}

BENCH bench_matrix3_inverse(void)
{
	int i;
	for (i = 0; i < M; i++) {
		struct matrix3 r;
		sink = matrix3_inverse(&m3a[i], &r)->m[0];
	}
}

BENCH bench_matrix4_inverse(void)
{
	int i;
	for (i = 0; i < M; i++) {
		struct matrix4 r;
		sink = matrix4_inverse(&m4a[i], &r)->m[0];
	}
}

BENCH bench_matrix4_determinant(void)
{
	int i;
	for (i = 0; i < M; i++) {
		sink = matrix4_determinant(&m4a[i]);
	}
}

BENCH bench_matrix4_multiply(void)
{
	int i;
	for (i = 0; i < M; i++) {
		struct matrix4 m = m4a[i];
		sink = matrix4_multiply(&m, &m4b[i])->m[0];
	}
}

BENCH bench_matrix4_multiplyv4(void)
{
	int i;
	for (i = 0; i < M; i++) {
		struct vector4 r;
		sink = matrix4_multiplyv4(&m4a[i], &v4a[i], &r)->x;
	}
}

BENCH bench_matrix4_set_from_quaternion(void)
{
	int i;
	for (i = 0; i < M; i++) {
		struct matrix4 m;
		sink = matrix4_set_from_quaternion(&m, &qu[i])->m[0];
	}
}

BENCH bench_matrix4_set_from_axisv3_angle(void)
{
	int i;
	for (i = 0; i < M; i++) {
		struct matrix4 m;
		sink = matrix4_set_from_axisv3_angle(&m, &v3a[i], fb[i])->m[0];
	}
}

BENCH bench_matrix4_view_lookat_rh(void)
{
	int i;
	for (i = 0; i < M; i++) {
		struct matrix4 m;
		sink = matrix4_view_lookat_rh(&m, &v3a[i], &v3b[i], &v3c[i])->m[0];
	}
}

BENCH bench_matrix4_projection_perspective_fovy_rh(void)
{
	int i;
	for (i = 0; i < M; i++) {
		struct matrix4 m;
		sink = matrix4_projection_perspective_fovy_rh(&m, fb[i], fb[i], fc[i], fd[i])->m[0];
	}
}

int main(void)
{
	prepare();

	bench_vector2_normalize();
	bench_vector3_normalize();
	bench_vector4_normalize();
	bench_vector3_cross_product();
	bench_vector3_project();
	bench_vector3_reflect();
	bench_vector3_refract();
	bench_vector2_angle_between();
	bench_vector3_angle_between();
	bench_vector3_rotate_by_quaternion();

	bench_quaternion_normalize();
	bench_quaternion_inverse();
	bench_quaternion_multiply();
	bench_quaternion_angle_between();
	bench_quaternion_slerp();
	bench_quaternion_set_from_axis_anglev3();
	bench_quaternion_get_axis_anglev3();
	bench_quaternion_set_from_matrix4();
	bench_quaternion_get_rotation_tov3();
	bench_quaternion_set_look_rotation_rh();

	bench_matrix2_inverse();
	bench_matrix3_inverse();
	bench_matrix4_inverse();
	bench_matrix4_determinant();
	bench_matrix4_multiply();
	bench_matrix4_multiplyv4();
	bench_matrix4_set_from_quaternion();
	bench_matrix4_set_from_axisv3_angle();
	bench_matrix4_view_lookat_rh();
	bench_matrix4_projection_perspective_fovy_rh();

	return 0;
}
