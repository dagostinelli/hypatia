/* SPDX-License-Identifier: MIT */

/* The projections with HYP_DEPTH_MINUS_ONE_TO_ONE: depth maps to -1 at the
 * near plane and 1 at the far plane, in both handednesses.
 */

#define HYP_DEPTH_MINUS_ONE_TO_ONE
#define HYPATIA_IMPLEMENTATION
#include <stdio.h>
#include <hypatia.h>

/* the depth of the point (0, 0, z, 1) after m and the divide by w */
static HYP_FLOAT depth(const struct matrix4 *m, HYP_FLOAT z)
{
	struct vector4 point;
	struct vector4 clip;

	vector4_setf4(&point, HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), z, HYP_FLOAT_C(1.0));
	matrix4_multiplyv4(m, &point, &clip);

	return clip.z / clip.w;
}

static int near_and_far(const struct matrix4 *m, HYP_FLOAT zNear, HYP_FLOAT zFar)
{
	return scalar_equalsf(depth(m, zNear), -HYP_FLOAT_C(1.0)) && scalar_equalsf(depth(m, zFar), HYP_FLOAT_C(1.0));
}

int main(void)
{
	struct matrix4 m;
	HYP_FLOAT fovy = HYP_TAU / HYP_FLOAT_C(4.0);

	matrix4_projection_perspective_fovy_rh(&m, fovy, HYP_FLOAT_C(1.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(100.0));
	if (!near_and_far(&m, -HYP_FLOAT_C(1.0), -HYP_FLOAT_C(100.0))) {
		printf("perspective rh depth\n");
		return 1;
	}

	matrix4_projection_perspective_fovy_lh(&m, fovy, HYP_FLOAT_C(1.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(100.0));
	if (!near_and_far(&m, HYP_FLOAT_C(1.0), HYP_FLOAT_C(100.0))) {
		printf("perspective lh depth\n");
		return 1;
	}

	matrix4_projection_ortho3d_rh(&m, -HYP_FLOAT_C(1.0), HYP_FLOAT_C(1.0), -HYP_FLOAT_C(1.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(100.0));
	if (!near_and_far(&m, -HYP_FLOAT_C(1.0), -HYP_FLOAT_C(100.0))) {
		printf("ortho rh depth\n");
		return 1;
	}

	matrix4_projection_ortho3d_lh(&m, -HYP_FLOAT_C(1.0), HYP_FLOAT_C(1.0), -HYP_FLOAT_C(1.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(100.0));
	if (!near_and_far(&m, HYP_FLOAT_C(1.0), HYP_FLOAT_C(100.0))) {
		printf("ortho lh depth\n");
		return 1;
	}

	matrix4_projection_frustum_rh(&m, -HYP_FLOAT_C(1.0), HYP_FLOAT_C(3.0), -HYP_FLOAT_C(2.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(100.0));
	if (!near_and_far(&m, -HYP_FLOAT_C(1.0), -HYP_FLOAT_C(100.0))) {
		printf("frustum rh depth\n");
		return 1;
	}

	matrix4_projection_frustum_lh(&m, -HYP_FLOAT_C(1.0), HYP_FLOAT_C(3.0), -HYP_FLOAT_C(2.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(100.0));
	if (!near_and_far(&m, HYP_FLOAT_C(1.0), HYP_FLOAT_C(100.0))) {
		printf("frustum lh depth\n");
		return 1;
	}

	/* no far plane: the depth approaches 1 far away */
	matrix4_projection_perspective_fovy_infinite_rh(&m, fovy, HYP_FLOAT_C(1.0), HYP_FLOAT_C(1.0));
	if (!near_and_far(&m, -HYP_FLOAT_C(1.0), -HYP_FLOAT_C(1e6))) {
		printf("infinite perspective rh depth\n");
		return 1;
	}

	matrix4_projection_perspective_fovy_infinite_lh(&m, fovy, HYP_FLOAT_C(1.0), HYP_FLOAT_C(1.0));
	if (!near_and_far(&m, HYP_FLOAT_C(1.0), HYP_FLOAT_C(1e6))) {
		printf("infinite perspective lh depth\n");
		return 1;
	}

	printf("ALL TESTS PASSED\n");
	return 0;
}
