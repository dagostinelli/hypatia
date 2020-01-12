#ifndef _HYPATIA_H_
#define _HYPATIA_H_

#include <math.h> /* sin, cos, acos */
#include <stdlib.h> /* RAND_MAX, rand */
#include <float.h> /*FLT_EPSILON, DBL_EPSILON*/
#include <stdint.h>
#include <stdio.h> /* printf (in the _print* functions) */
#include <memory.h> /* memset */
/* SPDX-License-Identifier: MIT */

#ifndef _HYPATIA_CONFIG_H_
#define _HYPATIA_CONFIG_H_

/* start with HYPAPI being off */
#ifndef HYPAPI
#	define HYPAPI
#endif

#define HYPATIA_SINGLE_PRECISION_FLOATS

#define HYPATIA_VERSION "1.0.0.0"

#endif /* _HYPATIA_CONFIG_H_ */
/* SPDX-License-Identifier: MIT */

#ifndef _INC_HYPATIA
#define _INC_HYPATIA




#if defined(_WIN32) || defined(_WIN64) || defined(_MSC_VER) || defined(WIN32)
#	undef HYPAPI
#	define HYPAPI __declspec(dllexport)
	/* #ifndef HYP_EXPORTS_ON
	 * #else
	 *	#define HYPAPI __declspec(dllimport)
	 * #endif
	 */
#else
#	undef HYPAPI
#	define HYPAPI
#endif

#ifdef HYPATIA_SINGLE_PRECISION_FLOATS
#	define HYP_FLOAT float
#else
#	define HYP_FLOAT double
#endif

 /* sin, cos, acos */
 /* RAND_MAX, rand */
 /*FLT_EPSILON, DBL_EPSILON*/

 /* printf (in the _print* functions) */
 /* memset */

/**
 * @ingroup _constants
 * @{
 */

/** @brief PI to 100 digits (gets rounded off by the compiler) */
#define HYP_PI 3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679f
/** @brief Tau to 100 digits, which is 2 * PI */
#define HYP_TAU 6.2831853071795864769252867665590057683943387987502116419498891846156328125724179972560696506842341359f
/** @brief Half of PI */
#define HYP_PI_HALF 1.5707963267948966f
/** @brief PI * PI */
#define HYP_PI_SQUARED 9.8696044010893586f
/** @brief Log e is the Natural Logarithm in base 10 */
#define HYP_E 2.71828182845904523536028747135266249775724709369995f
/** @brief Radians per Degree = PI/180 */
#define HYP_RAD_PER_DEG 0.0174532925199432957692369076848861f
/** @brief Degrees per Radian = 180/PI */
#define HYP_DEG_PER_RAD 57.2957795130823208767981548141052f
/** @brief PI/180 */
#define HYP_PIOVER180  HYP_RAD_PER_DEG
/** @brief 180/PI */
#define HYP_PIUNDER180 HYP_DEG_PER_RAD
/** @brief Epsilon.  This is the value that is used to determine how much
 * rounding error is tolerated.
 */
#ifdef HYPATIA_SINGLE_PRECISION_FLOATS
#	define HYP_EPSILON 1E-5f
#else
#	define HYP_EPSILON 1E-7
#endif
/*@}*/

/** @brief A macro that returns the minimum of \a a and \a b */
#define HYP_MIN(a, b)  (((a) < (b)) ? (a) : (b))

/** @brief A macro that returns the maximum of \a a and \a b */
#define HYP_MAX(a, b)  (((a) > (b)) ? (b) : (a))

/** @brief A macro that swaps \a a and \a b */
#define HYP_SWAP(a, b) { HYP_FLOAT f = a; a = b; b = f; }

/** @brief A macro that returns a random float point number up to RAND_MAX */
#define HYP_RANDOM_FLOAT (((HYP_FLOAT)rand() - (HYP_FLOAT)rand()) / (HYP_FLOAT)RAND_MAX)

/** @brief A macro that converts an angle in degress to an angle in radians */
#define HYP_DEG_TO_RAD(angle)  ((angle) * HYP_RAD_PER_DEG)

/** @brief A macro that converts an angle in radians to an angle in degrees */
#define HYP_RAD_TO_DEG(radians) ((radians) * HYP_DEG_PER_RAD)

/** @brief A macro that squares a value squared */
#define HYP_SQUARE(number) (number * number)

/** @brief A macro that finds the square root of a value */
#define HYP_SQRT(number) ((HYP_FLOAT)sqrt(number))

/** @brief A macro that returns the absolute value */
#define HYP_ABS(value) (((value) < 0) ? -(value) : (value))

/** @brief A macro that wraps a value around and around in a range */
#define HYP_WRAP(value, start, limit) (value = fmod(start + (value - start), (limit - start)))

/** @brief A macro that constrains the value between two limits \a a and \a b */
#define HYP_CLAMP(value, start, limit) (value = ((value < start) ? start : (value > limit) ? limit : value))

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/* forward declarations */
struct vector2;
struct vector3;
struct vector4;
struct matrix3;
struct matrix4;
struct quaternion;

#define HYP_REF_VECTOR2_ZERO 0
#define HYP_REF_VECTOR2_UNIT_X 1
#define HYP_REF_VECTOR2_UNIT_Y 2
#define HYP_REF_VECTOR2_UNIT_X_NEGATIVE 3
#define HYP_REF_VECTOR2_UNIT_Y_NEGATIVE 4
#define HYP_REF_VECTOR2_ONE 5

HYPAPI const struct vector2 *vector2_get_reference_vector2(int id);

#define HYP_REF_VECTOR3_ZERO 0
#define HYP_REF_VECTOR3_UNIT_X 1
#define HYP_REF_VECTOR3_UNIT_Y 2
#define HYP_REF_VECTOR3_UNIT_Z 3
#define HYP_REF_VECTOR3_UNIT_X_NEGATIVE 4
#define HYP_REF_VECTOR3_UNIT_Y_NEGATIVE 5
#define HYP_REF_VECTOR3_UNIT_Z_NEGATIVE 6
#define HYP_REF_VECTOR3_ONE 7

HYPAPI const struct vector3 *vector3_get_reference_vector3(int id);

#define HYP_REF_VECTOR4_ZERO 0
#define HYP_REF_VECTOR4_UNIT_X 1
#define HYP_REF_VECTOR4_UNIT_Y 2
#define HYP_REF_VECTOR4_UNIT_Z 3
#define HYP_REF_VECTOR4_UNIT_X_NEGATIVE 4
#define HYP_REF_VECTOR4_UNIT_Y_NEGATIVE 5
#define HYP_REF_VECTOR4_UNIT_Z_NEGATIVE 6
#define HYP_REF_VECTOR4_ONE 7

HYPAPI const struct vector4 *vector4_get_reference_vector4(int id);

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

/** @ingroup reference_vectors */
/** @brief {0,0,0} */
#define HYP_VECTOR3_ZERO vector3_get_reference_vector3(HYP_REF_VECTOR3_ZERO)
/** @ingroup reference_vectors */
/** @brief {1,0,0} */
#define HYP_VECTOR3_UNIT_X vector3_get_reference_vector3(HYP_REF_VECTOR3_UNIT_X)
/** @ingroup reference_vectors */
/** @brief {0,1,0} */
#define HYP_VECTOR3_UNIT_Y vector3_get_reference_vector3(HYP_REF_VECTOR3_UNIT_Y)
/** @ingroup reference_vectors */
/** @brief {0,0,1} */
#define HYP_VECTOR3_UNIT_Z vector3_get_reference_vector3(HYP_REF_VECTOR3_UNIT_Z)
/** @ingroup reference_vectors */
/** @brief {-1,0,0} */
#define HYP_VECTOR3_UNIT_X_NEGATIVE vector3_get_reference_vector3(HYP_REF_VECTOR3_UNIT_X_NEGATIVE)
/** @ingroup reference_vectors */
/** @brief {0,-1,0} */
#define HYP_VECTOR3_UNIT_Y_NEGATIVE vector3_get_reference_vector3(HYP_REF_VECTOR3_UNIT_Y_NEGATIVE)
/** @ingroup reference_vectors */
/** @brief {0,0,-1} */
#define HYP_VECTOR3_UNIT_Z_NEGATIVE vector3_get_reference_vector3(HYP_REF_VECTOR3_UNIT_Z_NEGATIVE)
/** @ingroup reference_vectors */
/** @brief {1,1,1} */
#define HYP_VECTOR3_ONE vector3_get_reference_vector3(HYP_REF_VECTOR3_ONE)
/* @} */


/** @ingroup reference_vectors */
/** @brief {0,0} */
#define HYP_VECTOR2_ZERO vector2_get_reference_vector2(HYP_REF_VECTOR2_ZERO)
/** @ingroup reference_vectors */
/** @brief {1,0} */
#define HYP_VECTOR2_UNIT_X vector2_get_reference_vector2(HYP_REF_VECTOR2_UNIT_X)
/** @ingroup reference_vectors */
/** @brief {0,1} */
#define HYP_VECTOR2_UNIT_Y vector2_get_reference_vector2(HYP_REF_VECTOR2_UNIT_Y)
/** @ingroup reference_vectors */
/** @brief {-1,0} */
#define HYP_VECTOR2_UNIT_X_NEGATIVE vector2_get_reference_vector2(HYP_REF_VECTOR2_UNIT_X_NEGATIVE)
/** @ingroup reference_vectors */
/** @brief {0,-1} */
#define HYP_VECTOR2_UNIT_Y_NEGATIVE vector2_get_reference_vector2(HYP_REF_VECTOR2_UNIT_Y_NEGATIVE)
/** @ingroup reference_vectors */
/** @brief {1,1} */
#define HYP_VECTOR2_ONE vector2_get_reference_vector2(HYP_REF_VECTOR2_ONE)
/* @} */


HYPAPI short scalar_equalsf(const HYP_FLOAT f1, const HYP_FLOAT f2);
HYPAPI short scalar_equals_epsilonf(const HYP_FLOAT f1, const HYP_FLOAT f2, const HYP_FLOAT epsilon);

#define scalar_equals scalar_equalsf


/**
 * @ingroup trig
 * @{
 */

#define HYP_SIN(x) ((HYP_FLOAT)sin(x))
#define HYP_COS(x) ((HYP_FLOAT)cos(x))
#define HYP_TAN(x) ((HYP_FLOAT)tan(x))
#define HYP_ASIN(x) ((HYP_FLOAT)asin(x))
#define HYP_ACOS(x) ((HYP_FLOAT)acos(x))
#define HYP_ATAN2(y, x) ((HYP_FLOAT)atan2(y, x))
#define HYP_COT(a) (1.0f / HYP_TAN(a))

/* @} */









/**
 * @ingroup experimental
 * @{
 */

#define _HYP_SWAP(x, y) do { tmp = x; x = y; y = tmp; } while (0)

HYPAPI void _matrix3_print_with_columnrow_indexer(struct matrix3 *self);
HYPAPI void _matrix3_print_with_rowcolumn_indexer(struct matrix3 *self);
HYPAPI struct matrix3 *_matrix3_set_random(struct matrix3 *self);

HYPAPI void _matrix4_print_with_columnrow_indexer(struct matrix4 *self);
HYPAPI void _matrix4_print_with_rowcolumn_indexer(struct matrix4 *self);
HYPAPI struct matrix4 *_matrix4_set_random(struct matrix4 *self);

HYPAPI void _quaternion_print(const struct quaternion *self);
HYPAPI struct quaternion *_quaternion_set_random(struct quaternion *self);

HYPAPI void _vector3_print(const struct vector3 *self);
HYPAPI struct vector3 *_vector3_set_random(struct vector3 *self);

HYPAPI void _vector2_print(const struct vector2 *self);
HYPAPI struct vector2 *_vector2_set_random(struct vector2 *self);

HYPAPI void _vector4_print(const struct vector4 *self);
HYPAPI struct vector4 *_vector4_set_random(struct vector4 *self);

/* @} */

#endif /* _INC_HYPATIA */
/* SPDX-License-Identifier: MIT */

#ifndef _INC_VECTOR2
#define _INC_VECTOR2



struct vector2
{
	union
		{
		HYP_FLOAT v[2];
		struct
			{
			HYP_FLOAT x, y;
			};
		};
};


HYPAPI int vector2_equals(const struct vector2 *self, const struct vector2 *vT);

HYPAPI struct vector2 *vector2_zero(struct vector2 *self);
HYPAPI struct vector2 *vector2_set(struct vector2 *self, const struct vector2 *vT);
HYPAPI struct vector2 *vector2_setf2(struct vector2 *self, HYP_FLOAT xT, HYP_FLOAT yT);
HYPAPI struct vector2 *vector2_negate(struct vector2 *self);
HYPAPI struct vector2 *vector2_add(struct vector2 *self, const struct vector2 *vT);
HYPAPI struct vector2 *vector2_addf(struct vector2 *self, HYP_FLOAT fT);
HYPAPI struct vector2 *vector2_subtract(struct vector2 *self, const struct vector2 *vT);
HYPAPI struct vector2 *vector2_subtractf(struct vector2 *self, HYP_FLOAT fT);
HYPAPI struct vector2 *vector2_multiply(struct vector2 *self, const struct vector2 *vT);
HYPAPI struct vector2 *vector2_multiplyf(struct vector2 *self, HYP_FLOAT fT);
HYPAPI struct vector2 *vector2_multiplym3(struct vector2 *self, const struct matrix3 *mT);
HYPAPI struct vector2 *vector2_divide(struct vector2 *self, const struct vector2 *vT);
HYPAPI struct vector2 *vector2_dividef(struct vector2 *self, HYP_FLOAT fT);

HYPAPI struct vector2 *vector2_normalize(struct vector2 *self);
HYPAPI HYP_FLOAT vector2_magnitude(const struct vector2 *self);
HYPAPI HYP_FLOAT vector2_distance(const struct vector2 *v1, const struct vector2 *v2);

HYPAPI HYP_FLOAT vector2_dot_product(const struct vector2 *self, const struct vector2 *vT);
HYPAPI struct vector2 *vector2_cross_product(struct vector2 *vR, const struct vector2 *vT1, const struct vector2 *vT2);

HYPAPI HYP_FLOAT vector2_angle_between(const struct vector2 *self, const struct vector2 *vT);
HYPAPI struct vector2 *vector2_find_normal_axis_between(struct vector2 *vR, const struct vector2 *vT1, const struct vector2 *vT2);

/* the length is the same as "magnitude" */
#define vector2_length(v) vector2_magnitude(v)

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/* BETA aliases */
#define vec2 struct vector2

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

#endif /* _INC_VECTOR2 */
/* SPDX-License-Identifier: MIT */

#ifndef _INC_VECTOR3
#define _INC_VECTOR3



struct vector3
{
	union
		{
		HYP_FLOAT v[3];
		struct
			{
			HYP_FLOAT x, y, z;
			};
		struct
			{
			HYP_FLOAT yaw, pitch, roll;
			};
		};
};


HYPAPI int vector3_equals(const struct vector3 *self, const struct vector3 *vT);

HYPAPI struct vector3 *vector3_zero(struct vector3 *self);
HYPAPI struct vector3 *vector3_set(struct vector3 *self, const struct vector3 *vT);
HYPAPI struct vector3 *vector3_setf3(struct vector3 *self, HYP_FLOAT xT, HYP_FLOAT yT, HYP_FLOAT zT);
HYPAPI struct vector3 *vector3_negate(struct vector3 *self);
HYPAPI struct vector3 *vector3_add(struct vector3 *self, const struct vector3 *vT);
HYPAPI struct vector3 *vector3_addf(struct vector3 *self, HYP_FLOAT fT);
HYPAPI struct vector3 *vector3_subtract(struct vector3 *self, const struct vector3 *vT);
HYPAPI struct vector3 *vector3_subtractf(struct vector3 *self, HYP_FLOAT fT);
HYPAPI struct vector3 *vector3_multiply(struct vector3 *self, const struct vector3 *vT);
HYPAPI struct vector3 *vector3_multiplyf(struct vector3 *self, HYP_FLOAT fT);
HYPAPI struct vector3 *vector3_multiplym4(struct vector3 *self, const struct matrix4 *mT);
HYPAPI struct vector3 *vector3_divide(struct vector3 *self, const struct vector3 *vT);
HYPAPI struct vector3 *vector3_dividef(struct vector3 *self, HYP_FLOAT fT);

HYPAPI struct vector3 *vector3_normalize(struct vector3 *self);
HYPAPI HYP_FLOAT vector3_magnitude(const struct vector3 *self);
HYPAPI HYP_FLOAT vector3_distance(const struct vector3 *v1, const struct vector3 *v2);

HYPAPI HYP_FLOAT vector3_dot_product(const struct vector3 *self, const struct vector3 *vT);
HYPAPI struct vector3 *vector3_cross_product(struct vector3 *vR, const struct vector3 *vT1, const struct vector3 *vT2);

HYPAPI HYP_FLOAT vector3_angle_between(const struct vector3 *self, const struct vector3 *vT);
HYPAPI struct vector3 *vector3_find_normal_axis_between(struct vector3 *vR, const struct vector3 *vT1, const struct vector3 *vT2);

HYPAPI struct vector3 *vector3_rotate_by_quaternion(struct vector3 *self, const struct quaternion *qT);
HYPAPI struct vector3 *vector3_reflect_by_quaternion(struct vector3 *self, const struct quaternion *qT);

/*the length is the same as "magnitude" */
#define vector3_length(v) vector3_magnitude(v)

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/*BETA aliases */
#define vec3 struct vector3

#endif /*DOXYGEN_SHOULD_SKIP_THIS */

#endif /*_INC_VECTOR3 */
/* SPDX-License-Identifier: MIT */

#ifndef _INC_VECTOR4
#define _INC_VECTOR4



struct vector4
{
	union
	{
		HYP_FLOAT v[4];
		struct
		{
			HYP_FLOAT x, y, z, w;
		};
	};
};


HYPAPI int vector4_equals(const struct vector4 *self, const struct vector4 *vT);

HYPAPI struct vector4 *vector4_zero(struct vector4 *self);
HYPAPI struct vector4 *vector4_set(struct vector4 *self, const struct vector4 *vT);
HYPAPI struct vector4 *vector4_setf4(struct vector4 *self, HYP_FLOAT xT, HYP_FLOAT yT, HYP_FLOAT zT, HYP_FLOAT wT);
HYPAPI struct vector4 *vector4_negate(struct vector4 *self);
HYPAPI struct vector4 *vector4_add(struct vector4 *self, const struct vector4 *vT);
HYPAPI struct vector4 *vector4_addf(struct vector4 *self, HYP_FLOAT fT);
HYPAPI struct vector4 *vector4_subtract(struct vector4 *self, const struct vector4 *vT);
HYPAPI struct vector4 *vector4_subtractf(struct vector4 *self, HYP_FLOAT fT);
HYPAPI struct vector4 *vector4_multiply(struct vector4 *self, const struct vector4 *vT);
HYPAPI struct vector4 *vector4_multiplyf(struct vector4 *self, HYP_FLOAT fT);
HYPAPI struct vector4 *vector4_divide(struct vector4 *self, const struct vector4 *vT);
HYPAPI struct vector4 *vector4_dividef(struct vector4 *self, HYP_FLOAT fT);

HYPAPI struct vector4 *vector4_normalize(struct vector4 *self);
HYPAPI HYP_FLOAT vector4_magnitude(const struct vector4 *self);
HYPAPI HYP_FLOAT vector4_distance(const struct vector4 *v1, const struct vector4 *v2);

HYPAPI HYP_FLOAT vector4_dot_product(const struct vector4 *self, const struct vector4 *vT);
HYPAPI struct vector4 *vector4_cross_product(struct vector4 *vR, const struct vector4 *vT1, const struct vector4 *vT2);

/* the length is the same as "magnitude" */
#define vector4_length(v) vector4_magnitude(v)

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/* BETA aliases */
#define vec4 struct vector4

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

#endif /* _INC_VECTOR4 */
/* SPDX-License-Identifier: MIT */

#ifndef _INC_MATRIX3
#define _INC_MATRIX3



struct matrix3
{
	union
	{
		HYP_FLOAT m[9]; /* row-major numbering */
		struct
		{
			/* reference the matrix [row][column] */
			HYP_FLOAT m33[3][3];
		};
		struct
		{
			/* indexed (column-major numbering) */
			HYP_FLOAT i00, i03, i06;
			HYP_FLOAT i01, i04, i07;
			HYP_FLOAT i02, i05, i08;
		};
		struct
		{
			/* col-row */
			HYP_FLOAT c00, c10, c20;
			HYP_FLOAT c01, c11, c21;
			HYP_FLOAT c02, c12, c22;
		};
		struct
		{
			/* row-col */
			HYP_FLOAT r00, r01, r02;
			HYP_FLOAT r10, r11, r12;
			HYP_FLOAT r20, r21, r22;
		};
	};
};


HYPAPI int matrix3_equals(const struct matrix3 *self, const struct matrix3 *mT);

HYPAPI struct matrix3 *matrix3_zero(struct matrix3 *self);
HYPAPI struct matrix3 *matrix3_identity(struct matrix3 *self);
HYPAPI struct matrix3 *matrix3_set(struct matrix3 *self, const struct matrix3 *mT);
HYPAPI struct matrix3 *matrix3_add(struct matrix3 *self, const struct matrix3 *mT);
HYPAPI struct matrix3 *matrix3_subtract(struct matrix3 *self, const struct matrix3 *mT);
HYPAPI struct matrix3 *matrix3_multiply(struct matrix3 *self, const struct matrix3 *mT);
HYPAPI struct matrix3 *matrix3_multiplyf(struct matrix3 *self, HYP_FLOAT scalar);
HYPAPI struct matrix3 *matrix3_transpose(struct matrix3 *self);

HYPAPI struct matrix3 *matrix3_make_transformation_translationv2(struct matrix3 *self, const struct vector2 *translation);
HYPAPI struct matrix3 *matrix3_make_transformation_scalingv2(struct matrix3 *self, const struct vector2 *scale);
HYPAPI struct matrix3 *matrix3_make_transformation_rotationf_z(struct matrix3 *self, HYP_FLOAT angle);

HYPAPI struct matrix3 *matrix3_translatev2(struct matrix3 *self, const struct vector2 *translation);
HYPAPI struct matrix3 *matrix3_rotate(struct matrix3 *self, HYP_FLOAT angle);
HYPAPI struct matrix3 *matrix3_scalev2(struct matrix3 *self, const struct vector2 *scale);

HYPAPI struct matrix3 *_matrix3_transpose_rowcolumn(struct matrix3 *self);
HYPAPI struct matrix3 *_matrix3_transpose_columnrow(struct matrix3 *self);


#ifndef DOXYGEN_SHOULD_SKIP_THIS

/* BETA aliases */
#define mat3 struct matrix3

#define mat3_equals matrix3_equals
#define mat3_zero matrix3_zero
#define mat3_identity matrix3_identity
#define mat3_set matrix3_setm3
#define mat3_add matrix3_add
#define mat3_sub matrix3_subtract
#define mat3_mul matrix3_multiply
#define mat3_transpose matrix3_transpose

#define mat3_translatev2 matrix3_translatev2
#define mat3_rotate matrix3_rotate
#define mat3_scalev2 matrix3_scalev2


#define m3 struct matrix3

#define m3_equals matrix3_equals
#define m3_zero matrix3_zero
#define m3_identity matrix3_identity
#define m3_set matrix3_set
#define m3_add matrix3_add
#define m3_sub matrix3_subtract
#define m3_mul matrix3_multiply
#define m3_transpose matrix3_transpose

#define m3_translatev2 matrix3_translatev2
#define m3_rotate matrix3_rotate
#define m3_scalev2 matrix3_scalev2

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

#endif /* _INC_MATRIX3 */
/* SPDX-License-Identifier: MIT */

#ifndef _INC_MATRIX4
#define _INC_MATRIX4



struct matrix4
{
	union
	{
		HYP_FLOAT m[16]; /* row-major numbering */
		struct
		{
			/* reference the matrix [row][column] */
			HYP_FLOAT m44[4][4];
		};
		struct
		{
			/* indexed (column-major numbering) */
			HYP_FLOAT i00, i04, i08, i12;
			HYP_FLOAT i01, i05, i09, i13;
			HYP_FLOAT i02, i06, i10, i14;
			HYP_FLOAT i03, i07, i11, i15;
		};
		struct
		{
			/* col-row */
			HYP_FLOAT c00, c10, c20, c30;
			HYP_FLOAT c01, c11, c21, c31;
			HYP_FLOAT c02, c12, c22, c32;
			HYP_FLOAT c03, c13, c23, c33;
		};
		struct
		{
			/* row-col */
			HYP_FLOAT r00, r01, r02, r03;
			HYP_FLOAT r10, r11, r12, r13;
			HYP_FLOAT r20, r21, r22, r23;
			HYP_FLOAT r30, r31, r32, r33;
		};
	};
};

HYPAPI int matrix4_equals(const struct matrix4 *self, const struct matrix4 *mT);

HYPAPI struct matrix4 *matrix4_zero(struct matrix4 *self);
HYPAPI struct matrix4 *matrix4_identity(struct matrix4 *self);
HYPAPI struct matrix4 *matrix4_set(struct matrix4 *self, const struct matrix4 *mT);
HYPAPI struct matrix4 *matrix4_add(struct matrix4 *self, const struct matrix4 *mT);
HYPAPI struct matrix4 *matrix4_subtract(struct matrix4 *self, const struct matrix4 *mT);

HYPAPI struct matrix4 *matrix4_multiply(struct matrix4 *self, const struct matrix4 *mT);
HYPAPI struct matrix4 *matrix4_multiplyf(struct matrix4 *self, HYP_FLOAT scalar);
HYPAPI struct vector4 *matrix4_multiplyv4(const struct matrix4 *self, const struct vector4 *vT, struct vector4 *vR);
HYPAPI struct vector3 *matrix4_multiplyv3(const struct matrix4 *self, const struct vector3 *vT, struct vector3 *vR);
HYPAPI struct vector2 *matrix4_multiplyv2(const struct matrix4 *self, const struct vector2 *vT, struct vector2 *vR);

HYPAPI struct matrix4 *matrix4_transpose(struct matrix4 *self);
HYPAPI HYP_FLOAT matrix4_determinant(const struct matrix4 *self);
HYPAPI struct matrix4 *matrix4_invert(struct matrix4 *self);
HYPAPI struct matrix4 *matrix4_inverse(struct matrix4 *self, struct matrix4 *mR);

HYPAPI struct matrix4 *matrix4_make_transformation_translationv3(struct matrix4 *self, const struct vector3 *translation);
HYPAPI struct matrix4 *matrix4_make_transformation_scalingv3(struct matrix4 *self, const struct vector3 *scale);
HYPAPI struct matrix4 *matrix4_make_transformation_rotationq(struct matrix4 *self, const struct quaternion *qT);
HYPAPI struct matrix4 *matrix4_make_transformation_rotationf_x(struct matrix4 *self, HYP_FLOAT angle);
HYPAPI struct matrix4 *matrix4_make_transformation_rotationf_y(struct matrix4 *self, HYP_FLOAT angle);
HYPAPI struct matrix4 *matrix4_make_transformation_rotationf_z(struct matrix4 *self, HYP_FLOAT angle);

HYPAPI struct matrix4 *matrix4_translatev3(struct matrix4 *self, const struct vector3 *translation);
HYPAPI struct matrix4 *matrix4_rotatev3(struct matrix4 *self, const struct vector3 *axis, HYP_FLOAT angle);
HYPAPI struct matrix4 *matrix4_scalev3(struct matrix4 *self, const struct vector3 *scale);

HYPAPI struct matrix4 *_matrix4_transpose_rowcolumn(struct matrix4 *self);
HYPAPI struct matrix4 *_matrix4_transpose_columnrow(struct matrix4 *self);


#ifndef DOXYGEN_SHOULD_SKIP_THIS

/* BETA aliases */
#define mat4 struct matrix4

#define mat4_equals matrix4_equals
#define mat4_zero matrix4_zero
#define mat4_identity matrix4_identity
#define mat4_set matrix4_setm4
#define mat4_add matrix4_add
#define mat4_sub matrix4_subtract
#define mat4_mul matrix4_multiply
#define mat4_transpose matrix4_transpose

#define mat4_translatev3 matrix3_translatev3
#define mat4_rotatev3 matrix3_rotatev3
#define mat4_scalev3 matrix3_scalev3


#define m4 struct matrix4

#define m4_equals matrix4_equals
#define m4_zero matrix4_zero
#define m4_identity matrix4_identity
#define m4_set matrix4_set
#define m4_add matrix4_add
#define m4_sub matrix4_subtract
#define m4_mul matrix4_multiply
#define m4_transpose matrix4_transpose

#define m4_translatev3 matrix3_translatev3
#define m4_rotatev3 matrix3_rotatev3
#define m4_scalev3 matrix3_scalev3

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

#endif /* _INC_MATRIX4 */
/* SPDX-License-Identifier: MIT */

#ifndef _INC_QUATERNION
#define _INC_QUATERNION



struct quaternion
{
	union
		{
		HYP_FLOAT q[4];
		struct
			{
			HYP_FLOAT x, y, z, w;
			};
		struct
			{
			HYP_FLOAT i, j, k, a;
			};
		};
};


HYPAPI int quaternion_equals(const struct quaternion *self, const struct quaternion *vT);

HYPAPI struct quaternion *quaternion_identity(struct quaternion *self);
HYPAPI struct quaternion *quaternion_setf4(struct quaternion *self, HYP_FLOAT x, HYP_FLOAT y, HYP_FLOAT z, HYP_FLOAT w);
HYPAPI struct quaternion *quaternion_set(struct quaternion *self, const struct quaternion *qT);
HYPAPI struct quaternion *quaternion_add(struct quaternion *self, const struct quaternion *qT);
HYPAPI struct quaternion *quaternion_multiply(struct quaternion *self, const struct quaternion *qT);
HYPAPI struct quaternion *quaternion_multiplyv3(struct quaternion *self, const struct vector3 *vT);
HYPAPI struct quaternion *quaternion_multiplyf(struct quaternion *self, HYP_FLOAT f);
HYPAPI struct quaternion *quaternion_subtract(struct quaternion *self, const struct quaternion *qT);
HYPAPI struct quaternion *quaternion_negate(struct quaternion *self);
HYPAPI struct quaternion *quaternion_conjugate(struct quaternion *self);
HYPAPI struct quaternion *quaternion_inverse(struct quaternion *self);

HYPAPI short quaternion_is_unit(struct quaternion *self);
HYPAPI short quaternion_is_pure(struct quaternion *self);
HYPAPI HYP_FLOAT quaternion_norm(const struct quaternion *self);
HYPAPI HYP_FLOAT quaternion_magnitude(const struct quaternion *self);
HYPAPI struct quaternion *quaternion_normalize(struct quaternion *self);
HYPAPI HYP_FLOAT quaternion_dot_product(const struct quaternion *self, const struct quaternion *qT);

HYPAPI struct quaternion *quaternion_lerp(const struct quaternion *start, const struct quaternion *end, HYP_FLOAT percent, struct quaternion *qR);
HYPAPI struct quaternion *quaternion_nlerp(const struct quaternion *start, const struct quaternion *end, HYP_FLOAT percent, struct quaternion *qR);
HYPAPI struct quaternion *quaternion_slerp(const struct quaternion *start, const struct quaternion *end, HYP_FLOAT percent, struct quaternion *qR);

HYPAPI void quaternion_get_axis_anglev3(const struct quaternion *self, struct vector3 *vR, HYP_FLOAT *angle);

HYPAPI struct quaternion *quaternion_set_from_axis_anglev3(struct quaternion *self, const struct vector3 *axis, HYP_FLOAT angle);
HYPAPI struct quaternion *quaternion_set_from_axis_anglef3(struct quaternion *self, HYP_FLOAT x, HYP_FLOAT y, HYP_FLOAT z, HYP_FLOAT angle);

HYPAPI struct quaternion *quaternion_set_from_euler_anglesf3(struct quaternion *self, HYP_FLOAT ax, HYP_FLOAT ay, HYP_FLOAT az);
HYPAPI void quaternion_get_euler_anglesf3(const struct quaternion *self, HYP_FLOAT *ax, HYP_FLOAT *ay, HYP_FLOAT *az);

HYPAPI struct quaternion *quaternion_get_rotation_tov3(const struct vector3 *from, const struct vector3 *to, struct quaternion *qR);

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/* BETA aliases */
#define quat struct quaternion

#define quat_equals quaternion_equals
#define quat_identity quaternion_identity
#define quat_lerp quaternion_lerp
#define quat_nlerp quaternion_nlerp
#define quat_slerp quaternion_slerp

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

#endif /* _INC_QUATERNION */
/* SPDX-License-Identifier: MIT */

#ifndef _INC_EXPERIMENTAL
#define _INC_EXPERIMENTAL





HYPAPI struct quaternion *quaternion_rotate_by_quaternion_EXP(struct quaternion *self, const struct quaternion *qT);
HYPAPI struct quaternion *quaternion_rotate_by_axis_angle_EXP(struct quaternion *self, const struct vector3 *axis, HYP_FLOAT angle);
HYPAPI struct quaternion *quaternion_rotate_by_euler_angles_EXP(struct quaternion *self, HYP_FLOAT ax, HYP_FLOAT ay, HYP_FLOAT az);
HYPAPI HYP_FLOAT quaternion_difference_EXP(const struct quaternion *q1, const struct quaternion *q2);
HYPAPI HYP_FLOAT quaternion_angle_between_EXP(const struct quaternion *self, const struct quaternion *qT);
HYPAPI void quaternion_axis_between_EXP(const struct quaternion *self, const struct quaternion *qT, struct quaternion *qR);
HYPAPI struct matrix4 *matrix4_projection_perspective_fovy_rh_EXP(struct matrix4 *self, HYP_FLOAT fovy, HYP_FLOAT aspect, HYP_FLOAT zNear, HYP_FLOAT zFar);
HYPAPI struct matrix4 *matrix4_projection_ortho3d_rh_EXP(struct matrix4 *self, HYP_FLOAT xmin, HYP_FLOAT xmax, HYP_FLOAT ymin, HYP_FLOAT ymax, HYP_FLOAT zNear, HYP_FLOAT zFar);
HYPAPI struct matrix4 *matrix4_view_lookat_rh_EXP(struct matrix4 *self, const struct vector3 *eye, const struct vector3 *target, const struct vector3 *up);
HYPAPI struct vector3 *matrix4_multiplyv3_EXP(const struct matrix4 *m, const struct vector3 *vT, struct vector3 *vR);
HYPAPI struct quaternion quaternion_cross_product_EXP(const struct quaternion *self, const struct quaternion *vT);
HYPAPI struct matrix4 *matrix4_set_from_quaternion_EXP(struct matrix4 *self, const struct quaternion *qT);
HYPAPI struct matrix4 *matrix4_set_from_axisv3_angle_EXP(struct matrix4 *self, const struct vector3 *axis, HYP_FLOAT angle);
HYPAPI struct matrix4 *matrix4_set_from_axisf3_angle_EXP(struct matrix4 *self, HYP_FLOAT x, HYP_FLOAT y, HYP_FLOAT z, const HYP_FLOAT angle);
HYPAPI struct matrix4 *matrix4_set_from_euler_anglesf3_EXP(struct matrix4 *self, const HYP_FLOAT x, const HYP_FLOAT y, const HYP_FLOAT z);
HYPAPI struct vector3 *matrix4_get_translation_EXP(const struct matrix4 *self, struct vector3 *vT);
HYPAPI struct matrix4 *matrix4_make_transformation_rotationv3_EXP(struct matrix4 *self, const struct vector3 *vR);
HYPAPI struct matrix4 *matrix4_transformation_compose_EXP(struct matrix4 *self, const struct vector3 *scale, const struct quaternion *rotation, const struct vector3 *translation);
HYPAPI uint8_t matrix4_transformation_decompose_EXP(struct matrix4 *self, struct vector3 *scale, struct quaternion *rotation, struct vector3 *translation);
#endif /* _INC_EXPERIMENTAL */

#endif /* _HYPATIA_H_ */
