/* SPDX-License-Identifier: MIT */

#ifndef _INC_HYPATIA
#define _INC_HYPATIA

#define HYPATIA_VERSION "2.0.0"

#ifndef HYPAPI
#	ifdef HYP_STATIC
#		define HYPAPI static
#	else
#		define HYPAPI extern
#	endif
#endif

#ifndef HYP_DEF
#	define HYPDEF HYPAPI
#endif

#ifndef HYP_INLINE
#	ifdef _MSC_VER
#		define HYP_INLINE __inline
#	else
#		define HYP_INLINE __inline__
#	endif
#endif

#ifndef HYP_FLOAT
#	ifdef HYPATIA_SINGLE_PRECISION_FLOATS
#		define HYP_FLOAT float
#	else
#		define HYP_FLOAT double
#	endif
#endif

#ifndef HYP_NO_C_MATH
#	include <math.h> /* sin, cos, acos, fmod */
#endif

#ifndef HYP_NO_STDIO
#	include <stdio.h> /* printf */
#endif

/**
 * @ingroup _constants
 * @{
 */

/** @brief PI to 100 digits (gets rounded off by the compiler) */
#ifndef HYP_PI
#	define HYP_PI 3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679f
#endif
/** @brief Tau to 100 digits, which is 2 * PI */
#ifndef HYP_TAU
#	define HYP_TAU 6.2831853071795864769252867665590057683943387987502116419498891846156328125724179972560696506842341359f
#endif
/** @brief Half of PI */
#ifndef HYP_PI_HALF
#	define HYP_PI_HALF 1.5707963267948966f
#endif
/** @brief PI * PI */
#ifndef HYP_PI_SQUARED
#	define HYP_PI_SQUARED 9.8696044010893586f
#endif
/** @brief Log e is the Natural Logarithm in base 10 */
#ifndef HYP_E
#	define HYP_E 2.71828182845904523536028747135266249775724709369995f
#endif
/** @brief Radians per Degree = PI/180 */
#ifndef HYP_RAD_PER_DEG
#	define HYP_RAD_PER_DEG 0.0174532925199432957692369076848861f
#endif
/** @brief Degrees per Radian = 180/PI */
#ifndef HYP_DEG_PER_RAD
#	define HYP_DEG_PER_RAD 57.2957795130823208767981548141052f
#endif
/** @brief PI/180 */
#ifndef HYP_PIOVER180
#	define HYP_PIOVER180  HYP_RAD_PER_DEG
#endif
/** @brief 180/PI */
#ifndef HYP_PIUNDER180
#	define HYP_PIUNDER180 HYP_DEG_PER_RAD
#endif
/** @brief Epsilon.  This is the value that is used to determine how much
 * rounding error is tolerated.
 */
#ifndef HYP_EPSILON
#	ifdef HYPATIA_SINGLE_PRECISION_FLOATS
#		define HYP_EPSILON 1E-5f
#	else
#		define HYP_EPSILON 1E-5
#	endif
#endif
/*@}*/

/** @brief A macro that enabled you to override memset */
#ifndef HYP_MEMSET
#	include <memory.h> /* memset */
#	define HYP_MEMSET(a, b, c)  memset(a, b, c)
#endif

/** @brief A function that returns the minimum of \a a and \a b */
static HYP_INLINE HYP_FLOAT HYP_MIN(HYP_FLOAT a, HYP_FLOAT b)
{
	return (a < b) ? a : b;
}

/** @brief A macro that returns the maximum of \a a and \a b */
static HYP_INLINE HYP_FLOAT HYP_MAX(HYP_FLOAT a, HYP_FLOAT b)
{
	return (a > b) ? b : a;
}

/** @brief A macro that swaps \a a and \a b */
static HYP_INLINE void HYP_SWAP(HYP_FLOAT *a, HYP_FLOAT *b)
{
	HYP_FLOAT f = *a; *a = *b; *b = f;
}

/** @brief A macro that returns a random float point number up to RAND_MAX */
#ifndef HYP_RANDOM_FLOAT
#	include <stdlib.h> /* RAND_MAX, rand */
#	define HYP_RANDOM_FLOAT (((HYP_FLOAT)rand() - (HYP_FLOAT)rand()) / (HYP_FLOAT)RAND_MAX)
#endif

/** @brief A macro that converts an angle in degress to an angle in radians */
#ifndef HYP_DEG_TO_RAD
#	define HYP_DEG_TO_RAD(angle)  ((angle) * HYP_RAD_PER_DEG)
#endif

/** @brief A macro that converts an angle in radians to an angle in degrees */
#ifndef HYP_RAD_TO_DEG
#	define HYP_RAD_TO_DEG(radians) ((radians) * HYP_DEG_PER_RAD)
#endif

/** @brief A macro that squares a value squared */
static HYP_INLINE HYP_FLOAT HYP_SQUARE(HYP_FLOAT number)
{
	return number * number;
}

/** @brief A macro that finds the square root of a value */
#ifndef HYP_SQRT
#	define HYP_SQRT(number) ((HYP_FLOAT)sqrt(number))
#endif

/** @brief A macro that returns the absolute value */
static HYP_INLINE HYP_FLOAT HYP_ABS(HYP_FLOAT value)
{
	return (value < 0.0f) ? -value : value;
}

/** @brief A macro that wraps a value around and around in a range */
static HYP_INLINE HYP_FLOAT HYP_WRAP(HYP_FLOAT value, HYP_FLOAT start, HYP_FLOAT limit)
{
	return (HYP_FLOAT)fmod(start + (value - start), (limit - start));
}

/** @brief A macro that constrains the value between two limits \a a and \a b */
static HYP_INLINE HYP_FLOAT HYP_CLAMP(HYP_FLOAT value, HYP_FLOAT start, HYP_FLOAT limit)
{
	return ((value < start) ? start : (value > limit) ? limit : value);
}

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

struct vector2 {
	union {
		HYP_FLOAT v[2];
		struct {
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


struct vector3 {
	union {
		HYP_FLOAT v[3];
		struct {
			HYP_FLOAT x, y, z;
		};
		struct {
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


struct vector4 {
	union {
		HYP_FLOAT v[4];
		struct {
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


struct matrix3 {
	union {
		HYP_FLOAT m[9]; /* row-major numbering */
		struct {
			/* reference the matrix [row][column] */
			HYP_FLOAT m33[3][3];
		};
		struct {
			/* indexed (column-major numbering) */
			HYP_FLOAT i00, i03, i06;
			HYP_FLOAT i01, i04, i07;
			HYP_FLOAT i02, i05, i08;
		};
		struct {
			/* col-row */
			HYP_FLOAT c00, c10, c20;
			HYP_FLOAT c01, c11, c21;
			HYP_FLOAT c02, c12, c22;
		};
		struct {
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
HYPAPI struct vector2 *matrix3_multiplyv2(const struct matrix3 *self, const struct vector2 *vT, struct vector2 *vR);

HYPAPI struct matrix3 *matrix3_transpose(struct matrix3 *self);
HYPAPI HYP_FLOAT matrix3_determinant(const struct matrix3 *self);
HYPAPI struct matrix3 *matrix3_invert(struct matrix3 *self);
HYPAPI struct matrix3 *matrix3_inverse(const struct matrix3 *self, struct matrix3 *mR);

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


struct matrix4 {
	union {
		HYP_FLOAT m[16]; /* row-major numbering */
		struct {
			/* reference the matrix [row][column] */
			HYP_FLOAT m44[4][4];
		};
		struct {
			/* indexed (column-major numbering) */
			HYP_FLOAT i00, i04, i08, i12;
			HYP_FLOAT i01, i05, i09, i13;
			HYP_FLOAT i02, i06, i10, i14;
			HYP_FLOAT i03, i07, i11, i15;
		};
		struct {
			/* col-row */
			HYP_FLOAT c00, c10, c20, c30;
			HYP_FLOAT c01, c11, c21, c31;
			HYP_FLOAT c02, c12, c22, c32;
			HYP_FLOAT c03, c13, c23, c33;
		};
		struct {
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
HYPAPI struct matrix4 *matrix4_inverse(const struct matrix4 *self, struct matrix4 *mR);

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


struct quaternion {
	union {
		HYP_FLOAT q[4];
		struct {
			HYP_FLOAT x, y, z, w;
		};
		struct {
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


#include <stdint.h>

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
#endif /* _INC_HYPATIA */


#ifdef HYPATIA_IMPLEMENTATION


/**
 * @brief This checks for mathematical equality within HYP_EPSILON.
 *
 */
HYPAPI short scalar_equalsf(const HYP_FLOAT f1, const HYP_FLOAT f2)
{
	return scalar_equals_epsilonf(f1, f2, HYP_EPSILON);
}

/**
 * @brief This checks for mathematical equality within a custom epsilon.
 *
 */
HYPAPI short scalar_equals_epsilonf(const HYP_FLOAT f1, const HYP_FLOAT f2, const HYP_FLOAT epsilon)
{
	if ((HYP_ABS(f1 - f2) < epsilon) == 0) {
		return 0;
	}

	return 1;
}


static struct vector2 _vector2_zero = { { {0.0f, 0.0f} } };
static struct vector2 _vector2_one = { { {1.0f, 1.0f} } };
static struct vector2 _vector2_unit_x = { { {1.0f, 0.0f} } };
static struct vector2 _vector2_unit_y = { { {0.0f, 1.0f} } };
static struct vector2 _vector2_unit_x_negative = { { {-1.0f, 0.0f} } };
static struct vector2 _vector2_unit_y_negative = { { {0.0f, -1.0f} } };


HYPAPI const struct vector2 *vector2_get_reference_vector2(int id)
{
	switch (id) {
	case HYP_REF_VECTOR2_ZERO:
		return &_vector2_zero;
	case HYP_REF_VECTOR2_ONE:
		return &_vector2_one;
	case HYP_REF_VECTOR2_UNIT_X:
		return &_vector2_unit_x;
	case HYP_REF_VECTOR2_UNIT_Y:
		return &_vector2_unit_y;
	case HYP_REF_VECTOR2_UNIT_X_NEGATIVE:
		return &_vector2_unit_x_negative;
	case HYP_REF_VECTOR2_UNIT_Y_NEGATIVE:
		return &_vector2_unit_y_negative;
	default:
		/* undefined case */
		return &_vector2_zero;
	}
}


HYPAPI struct vector2 *vector2_set(struct vector2 *self, const struct vector2 *vT)
{
	self->x = vT->x;
	self->y = vT->y;
	return self;
}


HYPAPI struct vector2 *vector2_setf2(struct vector2 *self, HYP_FLOAT xT, HYP_FLOAT yT)
{
	self->x = xT;
	self->y = yT;
	return self;
}


HYPAPI struct vector2 *vector2_zero(struct vector2 *self)
{
	return vector2_setf2(self, 0.0f, 0.0f);
}


HYPAPI int vector2_equals(const struct vector2 *self, const struct vector2 *vT)
{
	return scalar_equals(self->x, vT->x) && scalar_equals(self->y, vT->y);
}


HYPAPI struct vector2 *vector2_negate(struct vector2 *self)
{
	self->v[0] = -self->v[0];
	self->v[1] = -self->v[1];
	return self;
}


HYPAPI struct vector2 *vector2_add(struct vector2 *self, const struct vector2 *vT)
{
	self->v[0] += vT->v[0];
	self->v[1] += vT->v[1];
	return self;
}


HYPAPI struct vector2 *vector2_addf(struct vector2 *self, HYP_FLOAT fT)
{
	self->v[0] += fT;
	self->v[1] += fT;
	return self;
}


HYPAPI struct vector2 *vector2_subtract(struct vector2 *self, const struct vector2 *vT)
{
	self->v[0] -= vT->v[0];
	self->v[1] -= vT->v[1];
	return self;
}


HYPAPI struct vector2 *vector2_subtractf(struct vector2 *self, HYP_FLOAT fT)
{
	self->v[0] -= fT;
	self->v[1] -= fT;
	return self;
}


HYPAPI struct vector2 *vector2_multiply(struct vector2 *self, const struct vector2 *vT)
{
	self->v[0] *= vT->v[0];
	self->v[1] *= vT->v[1];
	return self;
}


HYPAPI struct vector2 *vector2_multiplyf(struct vector2 *self, HYP_FLOAT fT)
{
	self->v[0] *= fT;
	self->v[1] *= fT;
	return self;
}


HYPAPI struct vector2 *vector2_divide(struct vector2 *self, const struct vector2 *vT)
{
	self->v[0] /= vT->v[0];
	self->v[1] /= vT->v[1];
	return self;
}


HYPAPI struct vector2 *vector2_dividef(struct vector2 *self, HYP_FLOAT fT)
{
	self->v[0] /= fT;
	self->v[1] /= fT;
	return self;
}


HYPAPI HYP_FLOAT vector2_magnitude(const struct vector2 *self)
{
	return HYP_SQRT((self->x * self->x) + (self->y * self->y));
}


HYPAPI struct vector2 *vector2_normalize(struct vector2 *self)
{
	HYP_FLOAT mag = vector2_magnitude(self);

	self->x = self->x / mag;
	self->y = self->y / mag;
	return self;
}


HYPAPI HYP_FLOAT vector2_dot_product(const struct vector2 *self, const struct vector2 *vT)
{
	return (self->x * vT->x) + (self->y * vT->y);
}


HYPAPI struct vector2 *vector2_cross_product(struct vector2 *vR, const struct vector2 *vT1, const struct vector2 *vT2)
{
	vR->x = (vT1->x * vT2->y) - (vT1->y * vT2->x);
	vR->y = (vT1->y * vT2->x) - (vT1->x * vT2->y);
	return vR;
}


HYPAPI HYP_FLOAT vector2_angle_between(const struct vector2 *self, const struct vector2 *vT)
{
	return vector2_dot_product(self, vT) / (vector2_magnitude(self) * vector2_magnitude(vT));
}


HYPAPI struct vector2 *vector2_find_normal_axis_between(struct vector2 *vR, const struct vector2 *vT1, const struct vector2 *vT2)
{
	return vector2_normalize(vector2_cross_product(vR, vT1, vT2));
}


/**
 * @brief Calculates the distance between two points
 *
 * \f$\sqrt{(x_2-x_1)^2+(y_2-y_1)^2}\f$
 *
 * https://en.wikipedia.org/wiki/Distance
 */
HYPAPI HYP_FLOAT vector2_distance(const struct vector2 *v1, const struct vector2 *v2)
{
	return HYP_SQRT((v2->x - v1->x) * (v2->x - v1->x) + (v2->y - v1->y) * (v2->y - v1->y));
}


/**
 * @brief Multiply a vector by a matrix, returns a vector
 *
 * @param self The vector being multiplied
 * @param mT The matrix used to do the multiplication
 */
HYPAPI struct vector2 *vector2_multiplym3(struct vector2 *self, const struct matrix3 *mT)
{
	struct vector2 vR;

	vector2_zero(&vR);

	matrix3_multiplyv2(mT, self, &vR);

	vector2_set(self, &vR);

	return self;
}


#ifndef HYP_NO_STDIO
HYPAPI void _vector2_print(const struct vector2 *self)
{
	printf("x:%10f, y:%10f\r\n", self->x, self->y);
}
#endif


/**
 * @ingroup vector2
 * @brief Randomly fills the vector with values. Good for testing.
 *
 */
HYPAPI struct vector2 *_vector2_set_random(struct vector2 *self)
{
	self->x = HYP_RANDOM_FLOAT;
	self->y = HYP_RANDOM_FLOAT;
	return self;
}


static struct vector3 _vector3_zero = { { {0.0f, 0.0f, 0.0f} } };
static struct vector3 _vector3_one = { { {1.0f, 1.0f, 1.0f} } };
static struct vector3 _vector3_unit_x = { { {1.0f, 0.0f, 0.0f} } };
static struct vector3 _vector3_unit_y = { { {0.0f, 1.0f, 0.0f} } };
static struct vector3 _vector3_unit_z = { { {0.0f, 0.0f, 1.0f} } };
static struct vector3 _vector3_unit_x_negative = { { {-1.0f, 0.0f, 0.0f} } };
static struct vector3 _vector3_unit_y_negative = { { {0.0f, -1.0f,  0.0f} } };
static struct vector3 _vector3_unit_z_negative = { { {0.0f,  0.0f, -1.0f} } };


HYPAPI const struct vector3 *vector3_get_reference_vector3(int id)
{
	switch (id) {
	case HYP_REF_VECTOR3_ZERO:
		return &_vector3_zero;
	case HYP_REF_VECTOR3_ONE:
		return &_vector3_one;
	case HYP_REF_VECTOR3_UNIT_X:
		return &_vector3_unit_x;
	case HYP_REF_VECTOR3_UNIT_Y:
		return &_vector3_unit_y;
	case HYP_REF_VECTOR3_UNIT_Z:
		return &_vector3_unit_z;
	case HYP_REF_VECTOR3_UNIT_X_NEGATIVE:
		return &_vector3_unit_x_negative;
	case HYP_REF_VECTOR3_UNIT_Y_NEGATIVE:
		return &_vector3_unit_y_negative;
	case HYP_REF_VECTOR3_UNIT_Z_NEGATIVE:
		return &_vector3_unit_z_negative;
	default:
		/* undefined case */
		return &_vector3_zero;
	}
}


/**
 * @ingroup vector3
 * @brief initializes the vertex with specific values
 */
HYPAPI struct vector3 *vector3_setf3(struct vector3 *self, HYP_FLOAT xT, HYP_FLOAT yT, HYP_FLOAT zT)
{
	self->x = xT;
	self->y = yT;
	self->z = zT;
	return self;
}


/**
 * @ingroup vector3
 * @brief initializes the vertex with values from another vector
 */
HYPAPI struct vector3 *vector3_set(struct vector3 *self, const struct vector3 *vT)
{
	self->x = vT->x;
	self->y = vT->y;
	self->z = vT->z;
	return self;
}


/**
 * @ingroup vector3
 * @brief initializes the vertex with zeros
 */
HYPAPI struct vector3 *vector3_zero(struct vector3 *self)
{
	return vector3_setf3(self, 0.0f, 0.0f, 0.0f);
}


/**
 * @ingroup vector3
 * @brief compares two vectors.  Uses epsilon to deal with rounding errors
 */
HYPAPI int vector3_equals(const struct vector3 *self, const struct vector3 *vT)
{
	return  HYP_ABS(self->x - vT->x) < HYP_EPSILON &&
		HYP_ABS(self->y - vT->y) < HYP_EPSILON &&
		HYP_ABS(self->z - vT->z) < HYP_EPSILON;
}


/**
 * @ingroup vector3
 * @brief switches the sign on each component of the vector
 */
HYPAPI struct vector3 *vector3_negate(struct vector3 *self)
{
	self->v[0] = -self->v[0];
	self->v[1] = -self->v[1];
	self->v[2] = -self->v[2];
	return self;
}


/**
 * @ingroup vector3
 * @brief adds vectors using component-wise addition
 */
HYPAPI struct vector3 *vector3_add(struct vector3 *self, const struct vector3 *vT)
{
	self->v[0] += vT->v[0];
	self->v[1] += vT->v[1];
	self->v[2] += vT->v[2];
	return self;
}


/**
 * @ingroup vector3
 * @brief add to each component of the vector using a scalar
 */
HYPAPI struct vector3 *vector3_addf(struct vector3 *self, HYP_FLOAT f)
{
	self->v[0] += f;
	self->v[1] += f;
	self->v[2] += f;
	return self;
}


/**
 * @ingroup vector3
 * @brief subtract two vectors using component-wise subtraction
 */
HYPAPI struct vector3 *vector3_subtract(struct vector3 *self, const struct vector3 *vT)
{
	self->v[0] -= vT->v[0];
	self->v[1] -= vT->v[1];
	self->v[2] -= vT->v[2];
	return self;
}


/**
 * @ingroup vector3
 * @brief subtract each vector's component by a scalar
 */
HYPAPI struct vector3 *vector3_subtractf(struct vector3 *self, HYP_FLOAT f)
{
	self->v[0] -= f;
	self->v[1] -= f;
	self->v[2] -= f;
	return self;
}


/**
 * @ingroup vector3
 * @brief multiplies two vectors using component-wise multiplication
 */
HYPAPI struct vector3 *vector3_multiply(struct vector3 *self, const struct vector3 *vT)
{
	self->v[0] *= vT->v[0];
	self->v[1] *= vT->v[1];
	self->v[2] *= vT->v[2];
	return self;
}


/**
 * @ingroup vector3
 * @brief multiplies each component of the vector by a scalar
 */
HYPAPI struct vector3 *vector3_multiplyf(struct vector3 *self, HYP_FLOAT f)
{
	self->v[0] *= f;
	self->v[1] *= f;
	self->v[2] *= f;
	return self;
}


/**
 * @ingroup vector3
 * @brief divides one vector into another using component-wise division
 *
 */
HYPAPI struct vector3 *vector3_divide(struct vector3 *self, const struct vector3 *vT)
{
	self->v[0] /= vT->v[0];
	self->v[1] /= vT->v[1];
	self->v[2] /= vT->v[2];
	return self;
}


/**
 * @ingroup vector3
 * @brief calculates the magnitude of the vector
 */
HYPAPI HYP_FLOAT vector3_magnitude(const struct vector3 *self)
{
	return HYP_SQRT((self->x * self->x) + (self->y * self->y) + (self->z * self->z));
}


/**
 * @ingroup vector3
 * @brief normalizes the vector by dividing each component by the magnitude
 */
HYPAPI struct vector3 *vector3_normalize(struct vector3 *self)
{
	HYP_FLOAT mag;

	mag = vector3_magnitude(self);

	if (scalar_equalsf(mag, 0.0f)) {
		/* can't normalize a zero
		 * avoid divide by zero
		 */
		return self;
	}

	self->x = self->x / mag;
	self->y = self->y / mag;
	self->z = self->z / mag;

	return self;
}


/**
 * @ingroup vector3
 * @brief computes the dot product of two vectors
 */
HYPAPI HYP_FLOAT vector3_dot_product(const struct vector3 *self, const struct vector3 *vT)
{
	return (self->x * vT->x) + (self->y * vT->y) + (self->z * vT->z);
}


/**
 * @ingroup vector3
 * @brief computes the cross-product between two vectors
 */
HYPAPI struct vector3 *vector3_cross_product(struct vector3 *vR, const struct vector3 *vT1, const struct vector3 *vT2)
{
	vR->x = (vT1->y * vT2->z) - (vT1->z * vT2->y);
	vR->y = (vT1->z * vT2->x) - (vT1->x * vT2->z);
	vR->z = (vT1->x * vT2->y) - (vT1->y * vT2->x);
	return vR;
}

/**
 * @ingroup vector3
 * @brief finds the angle between two vectors. make sure to do this on a
 * normalized vector only
 *
 */
HYPAPI HYP_FLOAT vector3_angle_between(const struct vector3 *vT1, const struct vector3 *vT2)
{
	HYP_FLOAT c; /* cosine */

	c = vector3_dot_product(vT1, vT2) / (vector3_magnitude(vT1) * vector3_magnitude(vT2));

	return 2.0f * HYP_ACOS(c);
}


/**
 * @ingroup vector3
 * @brief finds the vector describing the normal between two vectors
 *
 */
HYPAPI struct vector3 *vector3_find_normal_axis_between(struct vector3 *vR, const struct vector3 *vT1, const struct vector3 *vT2)
{
	vector3_cross_product(vR, vT1, vT2);
	vector3_normalize(vR);
	return vR;
}


/**
 * @brief Calculates the distance between two points
 *
 * \f$\sqrt{(x_2-x_1)^2+(y_2-y_1)^2+(z_2-z_1)^2}\f$
 *
 * https://en.wikipedia.org/wiki/Distance
 */
HYPAPI HYP_FLOAT vector3_distance(const struct vector3 *v1, const struct vector3 *v2)
{
	return HYP_SQRT((v2->x - v1->x) * (v2->x - v1->x) + (v2->y - v1->y) * (v2->y - v1->y) + (v2->z - v1->z) * (v2->z - v1->z));
}


/**
 * @brief Multiply a vector by a matrix, mutates the vector and returns it
 *
 * @param self The vector being multiplied
 * @param mT The matrix used to do the multiplication
 */
HYPAPI struct vector3 *vector3_multiplym4(struct vector3 *self, const struct matrix4 *mT)
{
	struct vector3 vR;

	vector3_zero(&vR);

	matrix4_multiplyv3(mT, self, &vR);

	vector3_set(self, &vR);

	return self;
}


#ifndef HYP_NO_STDIO
HYPAPI void _vector3_print(const struct vector3 *self)
{
	printf("x:%10f, y:%10f, z:%10f\r\n", self->x, self->y, self->z);
}
#endif


/**
 * @ingroup vector3
 * @brief Rotate a point by the quaternion.  Returns the rotated point.
 *
 * \f$self= qT * self * conjugate(qT)\f$
 *
 * @param self the starting point
 * @param qT the quaternion
 *
 */
HYPAPI struct vector3 *vector3_rotate_by_quaternion(struct vector3 *self, const struct quaternion *qT)
{
	struct quaternion qconj;
	struct quaternion q;

	/* make the conjugate */
	quaternion_set(&qconj, qT);
	quaternion_conjugate(&qconj);

	quaternion_set(&q, qT);
	quaternion_multiplyv3(&q, self);
	quaternion_multiply(&q, &qconj);

	self->x = q.x;
	self->y = q.y;
	self->z = q.z;

	return self;
}


/**
 * @ingroup vector3
 * @brief Reflect a point by the quaternion.  Returns the reflected point.
 * (through the origin)
 *
 * \f$self= qT * self * qT\f$
 *
 * @param qT the quaternion
 * @param self the starting point that is rotated by qT
 *
 */
HYPAPI struct vector3 *vector3_reflect_by_quaternion(struct vector3 *self, const struct quaternion *qT)
{
	struct quaternion q;

	quaternion_set(&q, qT);
	quaternion_multiplyv3(&q, self);
	quaternion_multiply(&q, qT);

	/* this seems to be necessary */
	quaternion_normalize(&q);

	self->x = q.x;
	self->y = q.y;
	self->z = q.z;

	return self;
}


/**
 * @ingroup vector3
 * @brief Randomly fills the vector with values. Good for testing.
 *
 */
HYPAPI struct vector3 *_vector3_set_random(struct vector3 *self)
{
	self->x = HYP_RANDOM_FLOAT;
	self->y = HYP_RANDOM_FLOAT;
	self->z = HYP_RANDOM_FLOAT;
	return self;
}


static struct vector4 _vector4_zero = { { {0.0f, 0.0f, 0.0f, 0.0f} } };
static struct vector4 _vector4_one = { { {1.0f, 1.0f, 1.0f, 1.0f} } };
static struct vector4 _vector4_unit_x = { { {1.0f, 0.0f, 0.0f, 0.0f} } };
static struct vector4 _vector4_unit_y = { { {0.0f, 1.0f, 0.0f, 0.0f} } };
static struct vector4 _vector4_unit_z = { { {0.0f, 0.0f, 1.0f, 0.0f} } };
static struct vector4 _vector4_unit_x_negative = { { {-1.0f, 0.0f, 0.0f, 0.0f} } };
static struct vector4 _vector4_unit_y_negative = { { {0.0f, -1.0f,  0.0f, 0.0f} } };
static struct vector4 _vector4_unit_z_negative = { { {0.0f,  0.0f, -1.0f, 0.0f} } };


HYPAPI const struct vector4 *vector4_get_reference_vector4(int id)
{
	switch (id) {
	case HYP_REF_VECTOR4_ZERO:
		return &_vector4_zero;
	case HYP_REF_VECTOR4_ONE:
		return &_vector4_one;
	case HYP_REF_VECTOR4_UNIT_X:
		return &_vector4_unit_x;
	case HYP_REF_VECTOR4_UNIT_Y:
		return &_vector4_unit_y;
	case HYP_REF_VECTOR4_UNIT_Z:
		return &_vector4_unit_z;
	case HYP_REF_VECTOR4_UNIT_X_NEGATIVE:
		return &_vector4_unit_x_negative;
	case HYP_REF_VECTOR4_UNIT_Y_NEGATIVE:
		return &_vector4_unit_y_negative;
	case HYP_REF_VECTOR4_UNIT_Z_NEGATIVE:
		return &_vector4_unit_z_negative;
	default:
		/* undefined case */
		return &_vector4_zero;
	}
}


/**
 * @ingroup vector4
 * @brief initializes the vertex with specific values
 */
HYPAPI struct vector4 *vector4_setf4(struct vector4 *self, HYP_FLOAT xT, HYP_FLOAT yT, HYP_FLOAT zT, HYP_FLOAT wT)
{
	self->x = xT;
	self->y = yT;
	self->z = zT;
	self->w = wT;
	return self;
}


/**
 * @ingroup vector4
 * @brief initializes the vertex with values from another vector
 */
HYPAPI struct vector4 *vector4_set(struct vector4 *self, const struct vector4 *vT)
{
	self->x = vT->x;
	self->y = vT->y;
	self->z = vT->z;
	return self;
}


/**
 * @ingroup vector4
 * @brief initializes the vertex with zeros
 */
HYPAPI struct vector4 *vector4_zero(struct vector4 *self)
{
	return vector4_setf4(self, 0.0f, 0.0f, 0.0f, 0.0f);
}


/**
 * @ingroup vector4
 * @brief compares two vectors.  Uses epsilon to deal with rounding errors
 */
HYPAPI int vector4_equals(const struct vector4 *self, const struct vector4 *vT)
{
	return  HYP_ABS(self->x - vT->x) < HYP_EPSILON &&
		HYP_ABS(self->y - vT->y) < HYP_EPSILON &&
		HYP_ABS(self->z - vT->z) < HYP_EPSILON &&
		HYP_ABS(self->w - vT->w) < HYP_EPSILON;
}


/**
 * @ingroup vector4
 * @brief switches the sign on each component of the vector
 */
HYPAPI struct vector4 *vector4_negate(struct vector4 *self)
{
	self->v[0] = -self->v[0];
	self->v[1] = -self->v[1];
	self->v[2] = -self->v[2];
	self->v[3] = -self->v[3];
	return self;
}


/**
 * @ingroup vector4
 * @brief adds vectors using component-wise addition
 */
HYPAPI struct vector4 *vector4_add(struct vector4 *self, const struct vector4 *vT)
{
	self->v[0] += vT->v[0];
	self->v[1] += vT->v[1];
	self->v[2] += vT->v[2];
	self->v[3] += vT->v[3];
	return self;
}


/**
 * @ingroup vector4
 * @brief add to each component of the vector using a scalar
 */
HYPAPI struct vector4 *vector4_addf(struct vector4 *self, HYP_FLOAT f)
{
	self->v[0] += f;
	self->v[1] += f;
	self->v[2] += f;
	self->v[3] += f;
	return self;
}


/**
 * @ingroup vector4
 * @brief subtract two vectors using component-wise subtraction
 */
HYPAPI struct vector4 *vector4_subtract(struct vector4 *self, const struct vector4 *vT)
{
	self->v[0] -= vT->v[0];
	self->v[1] -= vT->v[1];
	self->v[2] -= vT->v[2];
	self->v[3] -= vT->v[3];
	return self;
}


/**
 * @ingroup vector4
 * @brief subtract each vector's component by a scalar
 */
HYPAPI struct vector4 *vector4_subtractf(struct vector4 *self, HYP_FLOAT f)
{
	self->x -= f;
	self->y -= f;
	self->z -= f;
	self->w -= f;
	return self;
}


/**
 * @ingroup vector4
 * @brief multiplies two vectors using component-wise multiplication
 */
HYPAPI struct vector4 *vector4_multiply(struct vector4 *self, const struct vector4 *vT)
{
	self->v[0] *= vT->v[0];
	self->v[1] *= vT->v[1];
	self->v[2] *= vT->v[2];
	self->v[3] *= vT->v[3];
	return self;
}


/**
 * @ingroup vector4
 * @brief multiplies each component of the vector by a scalar
 */
HYPAPI struct vector4 *vector4_multiplyf(struct vector4 *self, HYP_FLOAT f)
{
	self->v[0] *= f;
	self->v[1] *= f;
	self->v[2] *= f;
	self->v[3] *= f;
	return self;
}


/**
 * @ingroup vector4
 * @brief divides one vector into another using component-wise division
 *
 */
HYPAPI struct vector4 *vector4_divide(struct vector4 *self, const struct vector4 *vT)
{
	self->v[0] /= vT->v[0];
	self->v[1] /= vT->v[1];
	self->v[2] /= vT->v[2];
	self->v[3] /= vT->v[3];
	return self;
}


/**
 * @ingroup vector4
 * @brief calculates the magnitude of the vector
 */
HYPAPI HYP_FLOAT vector4_magnitude(const struct vector4 *self)
{
	return HYP_SQRT((self->x * self->x) + (self->y * self->y) + (self->z * self->z) + (self->w * self->w));
}


/**
 * @ingroup vector4
 * @brief normalizes the vector by dividing each component by the magnitude
 */
HYPAPI struct vector4 *vector4_normalize(struct vector4 *self)
{
	HYP_FLOAT mag;

	mag = vector4_magnitude(self);

	if (scalar_equalsf(mag, 0.0)) {
		/* can't normalize a zero
		 * avoid divide by zero
		 */
		return self;
	}

	self->x = self->x / mag;
	self->y = self->y / mag;
	self->z = self->z / mag;
	self->w = self->w / mag;

	return self;
}


/**
 * @ingroup vector4
 * @brief computes the dot product of two vectors
 */
HYPAPI HYP_FLOAT vector4_dot_product(const struct vector4 *self, const struct vector4 *vT)
{
	return (self->x * vT->x) + (self->y * vT->y) + (self->z * vT->z) + (self->w * vT->w);
}


/**
 * @ingroup vector4
 * @brief computes the cross-product between two vectors
 */
HYPAPI struct vector4 *vector4_cross_product(struct vector4 *vR, const struct vector4 *vT1, const struct vector4 *vT2)
{
	vR->x = (vT1->y * vT2->z) - (vT1->z * vT2->y);
	vR->y = (vT1->z * vT2->x) - (vT1->x * vT2->z);
	vR->z = (vT1->x * vT2->y) - (vT1->y * vT2->x);
	vR->w = (vT1->w * vT2->w) - (vT1->w * vT2->w);
	return vR;
}


/**
 * @brief Calculates the distance between two points
 *
 * \f$\sqrt{(x_2-x_1)^2+(y_2-y_1)^2+(z_2-z_1)^2}\f$
 *
 * https://en.wikipedia.org/wiki/Distance
 */
HYPAPI HYP_FLOAT vector4_distance(const struct vector4 *v1, const struct vector4 *v2)
{
	return HYP_SQRT((v2->x - v1->x) * (v2->x - v1->x)
		    + (v2->y - v1->y) * (v2->y - v1->y)
		    + (v2->z - v1->z) * (v2->z - v1->z)
		    + (v2->w - v1->w) * (v2->w - v1->w));
}


#ifndef HYP_NO_STDIO
HYPAPI void _vector4_print(const struct vector4 *self)
{
	printf("x:%10f, y:%10f, z:%10f, w:%10f\r\n", self->x, self->y, self->z, self->w);
}
#endif


/**
 * @ingroup vector4
 * @brief Randomly fills the vector with values. Good for testing.
 *
 */
HYPAPI struct vector4 *_vector4_set_random(struct vector4 *self)
{
	self->x = HYP_RANDOM_FLOAT;
	self->y = HYP_RANDOM_FLOAT;
	self->z = HYP_RANDOM_FLOAT;
	self->w = HYP_RANDOM_FLOAT;
	return self;
}


/**
 * @ingroup matrix3
 * @brief Initializes the matrix with 0.0 in every element.
 */
HYPAPI struct matrix3 *matrix3_zero(struct matrix3 *self)
{
	HYP_MEMSET(self, 0, sizeof(struct matrix3));
	return self;
}


/**
 * @ingroup matrix3
 * @brief Initializes the matrix as an identity matrix.
 */
HYPAPI struct matrix3 *matrix3_identity(struct matrix3 *m)
{
	m->c00 = 1.0f, m->c10 = 0.0f, m->c20 = 0.0f;
	m->c01 = 0.0f, m->c11 = 1.0f, m->c21 = 0.0f;
	m->c02 = 0.0f, m->c12 = 0.0f, m->c22 = 1.0f;

	return m;
}


/**
 * @ingroup matrix3
 * @brief Initializes the matrix by copying mT
 *
 * @param self The matrix to initialize
 * @param mT The matrix to copy
 */
HYPAPI struct matrix3 *matrix3_set(struct matrix3 *self, const struct matrix3 *mT)
{
	uint8_t i;

	for (i = 0; i < 9; i++) {
		self->m[i] = mT->m[i];
	}

	return self;
}


/**
 * @ingroup matrix3
 * @brief Compares every element of the matrix.  Uses HYP_EPSILON for precision.
 * returns 1 if equal, 0 if different
 */
HYPAPI int matrix3_equals(const struct matrix3 *self, const struct matrix3 *mT)
{
	uint8_t i;

	for (i = 0; i < 9; i++) {
		if (scalar_equalsf(self->m[i], mT->m[i]) == 0) {
			return 0;
		}
	}

	return 1;
}


/**
 * @ingroup matrix3
 * @brief "add row and column to row and column"
 *
 * @param self The matrix being changed
 * @param mT The matrix to add
 */
HYPAPI struct matrix3 *matrix3_add(struct matrix3 *self, const struct matrix3 *mT)
{
	/* "add row and column to row and column" */
	uint8_t i;

	for (i = 0; i < 9; i++) {
		self->m[i] += mT->m[i];
	}

	return self;
}


/**
 * @ingroup matrix3
 * @brief "subtract row and column from row and column"
 *
 * @param self The matrix being changed
 * @param mT The matrix to subtract from self (self = self - mT)
 */
HYPAPI struct matrix3 *matrix3_subtract(struct matrix3 *self, const struct matrix3 *mT)
{
	/* "subtract row and column from row and column" */
	uint8_t i;

	for (i = 0; i < 9; i++) {
		self->m[i] -= mT->m[i];
	}

	return self;
}


/**
 * @ingroup matrix3
 * @brief Multiply a matrix by a scalar, returns the matrix changed
 *
 * @param self The matrix being changed
 * @param scalar The scalar factor being multiplied in
 */
HYPAPI struct matrix3 *matrix3_multiplyf(struct matrix3 *self, HYP_FLOAT scalar)
{
	uint8_t i;

	for (i = 0; i < 9; i++) {
		self->m[i] *= scalar;
	}

	return self;
}


/**
 * @ingroup matrix3
 * @brief Multiply a matrix by a matrix, returns the matrix changed
 *
 * @param self the matrix being changed
 * @param mT The matrix being multiplied into self
 *
 * self = self * mT
 */
HYPAPI struct matrix3 *matrix3_multiply(struct matrix3 *self, const struct matrix3 *mT)
{
	/* mT is the multiplicand */

	struct matrix3 r;

	matrix3_identity(&r);

	/* first row */
	r.r00 = self->c00 * mT->c00 + self->c01 * mT->c10 + self->c02 * mT->c20;
	r.r01 = self->c10 * mT->c00 + self->c11 * mT->c10 + self->c12 * mT->c20;
	r.r02 = self->c20 * mT->c00 + self->c21 * mT->c10 + self->c22 * mT->c20;

	/* second row */
	r.r10 = self->c00 * mT->c01 + self->c01 * mT->c11 + self->c02 * mT->c21;
	r.r11 = self->c10 * mT->c01 + self->c11 * mT->c11 + self->c12 * mT->c21;
	r.r12 = self->c20 * mT->c01 + self->c21 * mT->c11 + self->c22 * mT->c21;

	/* third row */
	r.r20 = self->c00 * mT->c02 + self->c01 * mT->c12 + self->c02 * mT->c22;
	r.r21 = self->c10 * mT->c02 + self->c11 * mT->c12 + self->c12 * mT->c22;
	r.r22 = self->c20 * mT->c02 + self->c21 * mT->c12 + self->c22 * mT->c22;

	matrix3_set(self, &r); /* overwrite/save it */

	return self;
}


/**
 * @ingroup matrix3
 * @brief Multiply a vector by a matrix
 *
 * @param self The matrix used to do the multiplication
 * @param vT The vector being multiplied
 * @param vR The vector returned
 */
HYPAPI struct vector2 *matrix3_multiplyv2(const struct matrix3 *self, const struct vector2 *vT, struct vector2 *vR)
{
	vR->x = vT->x * self->c00 + vT->y * self->c01 + self->c20;
	vR->y = vT->x * self->c10 + vT->y * self->c11 + self->c21;

	return vR;
}


/**
 * @ingroup matrix3
 * @brief Transpose the matrix
 *
 * @param self The matrix being changed
 */
HYPAPI struct matrix3 *matrix3_transpose(struct matrix3 *self)
{
	return _matrix3_transpose_columnrow(self);
}


/**
 * @ingroup matrix3
 * @brief Swaps the row and column
 *
 */
HYPAPI struct matrix3 *_matrix3_transpose_rowcolumn(struct matrix3 *self)
{
	HYP_SWAP(&self->r01, &self->r10);
	HYP_SWAP(&self->r02, &self->r20);
	HYP_SWAP(&self->r12, &self->r21);

	return self;
}


/**
 * @ingroup matrix3
 * @brief Swaps the columns and row
 *
 */
HYPAPI struct matrix3 *_matrix3_transpose_columnrow(struct matrix3 *self)
{
	HYP_SWAP(&self->c01, &self->c10);
	HYP_SWAP(&self->c02, &self->c20);
	HYP_SWAP(&self->c12, &self->c21);

	return self;
}


#ifndef HYP_NO_STDIO
/**
 * @ingroup matrix3
 * @brief Prints out the matrix using column and row notation
 *
 */
HYPAPI void _matrix3_print_with_columnrow_indexer(struct matrix3 *self)
{
	printf("%10f, %10f, %10f\r\n", self->c00, self->c10, self->c20);
	printf("%10f, %10f, %10f\r\n", self->c01, self->c11, self->c21);
	printf("%10f, %10f, %10f\r\n", self->c02, self->c12, self->c22);
}
#endif


#ifndef HYP_NO_STDIO
/**
 * @ingroup matrix3
 * @brief Prints out the matrix using row and column notation
 *
 */
HYPAPI void _matrix3_print_with_rowcolumn_indexer(struct matrix3 *self)
{
	printf("%10f, %10f, %10f\r\n", self->r00, self->r01, self->r02);
	printf("%10f, %10f, %10f\r\n", self->r10, self->r11, self->r12);
	printf("%10f, %10f, %10f\r\n", self->r20, self->r21, self->r22);
}
#endif

/**
 * @ingroup matrix3
 * @brief Randomly fills the matrix with values. Good for testing.
 *
 */
HYPAPI struct matrix3 *_matrix3_set_random(struct matrix3 *self)
{
	uint8_t i;

	for (i = 0; i < 9; i++) {
		self->m[i] = HYP_RANDOM_FLOAT;
	}

	return self;
}


/**
 * @ingroup matrix3
 * @brief creates a translation matrix.  It's opinionated about what that means.
 *
 */
HYPAPI struct matrix3 *matrix3_make_transformation_translationv2(struct matrix3 *self, const struct vector2 *translation)
{
	matrix3_identity(self);

	self->r02 = translation->x;
	self->r12 = translation->y;

	return self;
}


/**
 * @ingroup matrix3
 * @brief creates a scaling matrix.  It's opinionated about what that means.
 *
 */
HYPAPI struct matrix3 *matrix3_make_transformation_scalingv2(struct matrix3 *self, const struct vector2 *scale)
{
	matrix3_identity(self);

	self->r00 = scale->x;
	self->r11 = scale->y;

	return self;
}


/**
 * @ingroup matrix3
 * @brief creates a rotation matrix about the z.  It's opinionated about what
 * that means.
 *
 * multiply this matrix by another matrix to rotate the other matrix
 */
HYPAPI struct matrix3 *matrix3_make_transformation_rotationf_z(struct matrix3 *m, HYP_FLOAT angle)
{
	HYP_FLOAT c = HYP_COS(angle);
	HYP_FLOAT s = HYP_SIN(angle);

	matrix3_identity(m);

	m->r00 = c;
	m->r01 = s;
	m->r10 = -s;
	m->r12 = c;

	return m;
}


/**
 * @ingroup matrix3
 * @brief Creates a temporary translation matrix and then multiplies self by
 * that.  Opinionated function about what translation means.
 *
 * @param self The transformation matrix being translated
 * @param translation the translation vector
 *
 */
HYPAPI struct matrix3 *matrix3_translatev2(struct matrix3 *self, const struct vector2 *translation)
{
	struct matrix3 translationMatrix;

	return matrix3_multiply(self,
		matrix3_make_transformation_translationv2(&translationMatrix, translation));
}


/**
 * @ingroup matrix3
 * @brief Creates a temporary rotation matrix and then multiplies self by that.
 * Opinionated function about what rotation means.  It always rotates about
 * the z which it assumes is coming out of the screen.
 *
 * @param self The transformation matrix being rotated
 * @param angle the angle of rotation in radians
 *
 */
HYPAPI struct matrix3 *matrix3_rotate(struct matrix3 *self, HYP_FLOAT angle)
{
	struct matrix3 rotationMatrix;

	return matrix3_multiply(self,
		matrix3_make_transformation_rotationf_z(&rotationMatrix, angle));
}


/**
 * @ingroup matrix3
 * @brief Creates a temporary scaling matrix and then multiplies self by that.
 * Opinionated function about what scaling means.
 *
 * @param self The transformation matrix being scaled
 * @param scale the scaling vector
 *
 */
HYPAPI struct matrix3 *matrix3_scalev2(struct matrix3 *self, const struct vector2 *scale)
{
	struct matrix3 scalingMatrix;

	return matrix3_multiply(self,
		matrix3_make_transformation_scalingv2(&scalingMatrix, scale));
}


#define CAT(a, ...) PRIMITIVE_CAT(a, __VA_ARGS__)
#define PRIMITIVE_CAT(a, ...) a ## __VA_ARGS__
#define DEC(x) PRIMITIVE_CAT(DEC_, x)
#define DEC_11 00
#define DEC_12 01
#define DEC_13 02
#define DEC_14 03
#define DEC_21 10
#define DEC_22 11
#define DEC_23 12
#define DEC_24 13
#define DEC_31 20
#define DEC_32 21
#define DEC_33 22
#define DEC_34 23
#define DEC_41 30
#define DEC_42 31
#define DEC_43 32
#define DEC_44 33
#define A(x) CAT(self->r,  DEC(x))
#define B(x) CAT(inverse.r, DEC(x))
#define A4(x1, x2, x3, x4) (A(x1) * A(x2) * A(x3) * A(x4))
#define A3(x1, x2, x3) (A(x1) * A(x2) * A(x3))
#define A2(x1, x2) (A(x1) * A(x2))


/**
 * @ingroup matrix3
 * @brief Finds the determinant of a matrix
 *
 * @param self The transformation matrix being questioned
 *
 */
HYPAPI HYP_FLOAT matrix3_determinant(const struct matrix3 *self)
{
	HYP_FLOAT determinant;

	determinant =
	  (A(11) * (A2(22, 33) - A2(32, 23)))
	- (A(12) * (A2(21, 33) - A2(31, 23)))
	+ (A(13) * (A2(21, 32) - A2(31, 22)))
	;

	return determinant;
}


/**
 * @ingroup matrix3
 * @brief Invert a matrix
 *
 * @param self The transformation matrix being inverted
 *
 */
HYPAPI struct matrix3 *matrix3_invert(struct matrix3 *self)
{
	struct matrix3 inverse;
	uint8_t i;

	if (matrix3_inverse(self, &inverse)) {
		for (i = 0; i < 9; i++) {
			self->m[i] = inverse.m[i];
		}
	}

	return self;
}


/**
 * @ingroup matrix3
 * @brief Find the inverse of the matrix
 *
 * @param self The transformation matrix being examined
 * @param mR the inverse of the matrix is returned here
 *
 */
HYPAPI struct matrix3 *matrix3_inverse(const struct matrix3 *self, struct matrix3 *mR)
{
	struct matrix3 inverse;
	HYP_FLOAT determinant;
	uint8_t i;

	determinant = matrix3_determinant(self);

	/* calculated early for a quick exit if no determinant exists */
	if (scalar_equalsf(determinant, 0.0f)) {
		return NULL;
	}

	determinant = 1.0f / determinant;

	matrix3_identity(&inverse);

	/* find the adjugate of self */
	B(11) = A2(22, 33) - A2(32, 23);
	B(12) = A2(32, 13) - A2(12, 33);
	B(13) = A2(12, 23) - A2(22, 13);

	B(21) = A2(23, 31) - A2(33, 21);
	B(22) = A2(33, 11) - A2(13, 31);
	B(23) = A2(13, 21) - A2(23, 11);

	B(31) = A2(21, 32) - A2(31, 22);
	B(32) = A2(31, 12) - A2(11, 32);
	B(33) = A2(11, 22) - A2(21, 12);

	/* divide the determinant */
	for (i = 0; i < 9; i++) {
		mR->m[i] = inverse.m[i] * determinant;
	}

	return mR;
}


/**
 * @ingroup matrix4
 * @brief Initializes the matrix with 0.0 in every element.
 */
HYPAPI struct matrix4 *matrix4_zero(struct matrix4 *self)
{
	HYP_MEMSET(self, 0, sizeof(struct matrix4));
	return self;
}


/**
 * @ingroup matrix4
 * @brief Initializes the matrix as an identity matrix.
 */
HYPAPI struct matrix4 *matrix4_identity(struct matrix4 *m)
{
	m->c00 = 1.0f, m->c10 = 0.0f, m->c20 = 0.0f, m->c30 = 0.0f;
	m->c01 = 0.0f, m->c11 = 1.0f, m->c21 = 0.0f, m->c31 = 0.0f;
	m->c02 = 0.0f, m->c12 = 0.0f, m->c22 = 1.0f, m->c32 = 0.0f;
	m->c03 = 0.0f, m->c13 = 0.0f, m->c23 = 0.0f, m->c33 = 1.0f;

	return m;
}


/**
 * @ingroup matrix4
 * @brief Initializes the matrix by copying mT
 *
 * @param self The matrix to initialize
 * @param mT The matrix to copy
 */
HYPAPI struct matrix4 *matrix4_set(struct matrix4 *self, const struct matrix4 *mT)
{
	uint8_t i;

	for (i = 0; i < 16; i++) {
		self->m[i] = mT->m[i];
	}

	return self;
}


/**
 * @ingroup matrix4
 * @brief Compares every element of the matrix.  Uses HYP_EPSILON for precision.
 * returns 1 if equal, 0 if different
 */
HYPAPI int matrix4_equals(const struct matrix4 *self, const struct matrix4 *mT)
{
	uint8_t i;

	for (i = 0; i < 16; i++) {
		if (scalar_equalsf(self->m[i], mT->m[i]) == 0) {
			return 0;
		}
	}

	return 1;
}


/**
 * @ingroup matrix4
 * @brief "add row and column to row and column"
 *
 * @param self The matrix being changed
 * @param mT The matrix to add
 */
HYPAPI struct matrix4 *matrix4_add(struct matrix4 *self, const struct matrix4 *mT)
{
	/* "add row and column to row and column" */
	uint8_t i;

	for (i = 0; i < 16; i++) {
		self->m[i] += mT->m[i];
	}

	return self;
}


/**
 * @ingroup matrix4
 * @brief "subtract row and column from row and column"
 *
 * @param self The matrix being changed
 * @param mT The matrix to subtract from self (self = self - mT)
 */
HYPAPI struct matrix4 *matrix4_subtract(struct matrix4 *self, const struct matrix4 *mT)
{
	/* "subtract row and column from row and column" */
	uint8_t i;

	for (i = 0; i < 16; i++) {
		self->m[i] -= mT->m[i];
	}

	return self;
}


/**
 * @ingroup matrix4
 * @brief Multiply a matrix by a scalar, returns the matrix changed
 *
 * @param self The matrix being changed
 * @param scalar The scalar factor being multiplied in
 */
HYPAPI struct matrix4 *matrix4_multiplyf(struct matrix4 *self, HYP_FLOAT scalar)
{
	uint8_t i;

	for (i = 0; i < 16; i++) {
		self->m[i] *= scalar;
	}

	return self;
}


/**
 * @ingroup matrix4
 * @brief Multiply a matrix by a matrix, returns the matrix changed
 *
 * @param self the matrix being changed
 * @param mT The matrix being multiplied into self
 *
 * self = self * mT
 */
HYPAPI struct matrix4 *matrix4_multiply(struct matrix4 *self, const struct matrix4 *mT)
{
	/* mT is the multiplicand */

	struct matrix4 r;

	matrix4_identity(&r);

	/* first row */
	r.r00 = self->c00 * mT->c00 + self->c01 * mT->c10 + self->c02 * mT->c20 + self->c03 * mT->c30;
	r.r01 = self->c10 * mT->c00 + self->c11 * mT->c10 + self->c12 * mT->c20 + self->c13 * mT->c30;
	r.r02 = self->c20 * mT->c00 + self->c21 * mT->c10 + self->c22 * mT->c20 + self->c23 * mT->c30;
	r.r03 = self->c30 * mT->c00 + self->c31 * mT->c10 + self->c32 * mT->c20 + self->c33 * mT->c30;

	/* second row */
	r.r10 = self->c00 * mT->c01 + self->c01 * mT->c11 + self->c02 * mT->c21 + self->c03 * mT->c31;
	r.r11 = self->c10 * mT->c01 + self->c11 * mT->c11 + self->c12 * mT->c21 + self->c13 * mT->c31;
	r.r12 = self->c20 * mT->c01 + self->c21 * mT->c11 + self->c22 * mT->c21 + self->c23 * mT->c31;
	r.r13 = self->c30 * mT->c01 + self->c31 * mT->c11 + self->c32 * mT->c21 + self->c33 * mT->c31;

	/* third row */
	r.r20 = self->c00 * mT->c02 + self->c01 * mT->c12 + self->c02 * mT->c22 + self->c03 * mT->c32;
	r.r21 = self->c10 * mT->c02 + self->c11 * mT->c12 + self->c12 * mT->c22 + self->c13 * mT->c32;
	r.r22 = self->c20 * mT->c02 + self->c21 * mT->c12 + self->c22 * mT->c22 + self->c23 * mT->c32;
	r.r23 = self->c30 * mT->c02 + self->c31 * mT->c12 + self->c32 * mT->c22 + self->c33 * mT->c32;

	/* fourth row */
	r.r30 = self->c00 * mT->c03 + self->c01 * mT->c13 + self->c02 * mT->c23 + self->c03 * mT->c33;
	r.r31 = self->c10 * mT->c03 + self->c11 * mT->c13 + self->c12 * mT->c23 + self->c13 * mT->c33;
	r.r32 = self->c20 * mT->c03 + self->c21 * mT->c13 + self->c22 * mT->c23 + self->c23 * mT->c33;
	r.c33 = self->c30 * mT->c03 + self->c31 * mT->c13 + self->c32 * mT->c23 + self->c33 * mT->c33;

	matrix4_set(self, &r); /* overwrite/save it */

	return self;
}


/**
 * @ingroup matrix4
 * @brief Multiply a matrix by a vector4, returns a vector4.
 *
 * @param self the matrix being multiplied. Remains unchanged.
 * @param vT The vector being multiplied. Remains unchanged.
 * @param vR The resultant vector. It is set to the answer
 *
 * vR = vT * self
 */
HYPAPI struct vector4 *matrix4_multiplyv4(const struct matrix4 *self, const struct vector4 *vT, struct vector4 *vR)
{
	vR->x = vT->x * self->r00 + vT->y * self->r01 + vT->z * self->r02 + vT->w * self->r03;
	vR->y = vT->x * self->r10 + vT->y * self->r11 + vT->z * self->r12 + vT->w * self->r13;
	vR->z = vT->x * self->r20 + vT->y * self->r21 + vT->z * self->r22 + vT->w * self->r23;
	vR->w = vT->x * self->r30 + vT->y * self->r31 + vT->z * self->r32 + vT->w * self->r33;

	return vR;
}


/**
 * @ingroup matrix4
 * @brief Multiply a vector by a matrix
 *
 * @param self The matrix used to do the multiplication
 * @param vT The vector being multiplied
 * @param vR The vector returned
 */
HYPAPI struct vector3 *matrix4_multiplyv3(const struct matrix4 *self, const struct vector3 *vT, struct vector3 *vR)
{
	vR->x = vT->x * self->r00 + vT->y * self->r01 + vT->z * self->r02 + self->r03;
	vR->y = vT->x * self->r10 + vT->y * self->r11 + vT->z * self->r12 + self->r13;
	vR->z = vT->x * self->r20 + vT->y * self->r21 + vT->z * self->r22 + self->r23;

	return vR;
}


/**
 * @ingroup matrix4
 * @brief Multiply a vector by a matrix
 *
 * @param self The matrix used to do the multiplication
 * @param vT The vector being multiplied
 * @param vR The vector returned
 */
HYPAPI struct vector2 *matrix4_multiplyv2(const struct matrix4 *self, const struct vector2 *vT, struct vector2 *vR)
{
	vR->x = vT->x * self->r00 + vT->y * self->r01 + self->r02 + self->r03;
	vR->y = vT->x * self->r10 + vT->y * self->r11 + self->r12 + self->r13;

	return vR;
}


/**
 * @ingroup matrix4
 * @brief Transpose the matrix
 *
 * @param self The matrix being changed
 */
HYPAPI struct matrix4 *matrix4_transpose(struct matrix4 *self)
{
	return _matrix4_transpose_columnrow(self);
}


/**
 * @ingroup matrix4
 * @brief Swaps the row and column
 *
 */
HYPAPI struct matrix4 *_matrix4_transpose_rowcolumn(struct matrix4 *self)
{
	HYP_SWAP(&self->r01, &self->r10);
	HYP_SWAP(&self->r02, &self->r20);
	HYP_SWAP(&self->r03, &self->r30);
	HYP_SWAP(&self->r12, &self->r21);
	HYP_SWAP(&self->r13, &self->r31);
	HYP_SWAP(&self->r23, &self->r32);

	return self;
}


/**
 * @ingroup matrix4
 * @brief Swaps the columns and row
 *
 */
HYPAPI struct matrix4 *_matrix4_transpose_columnrow(struct matrix4 *self)
{
	HYP_SWAP(&self->c01, &self->c10);
	HYP_SWAP(&self->c02, &self->c20);
	HYP_SWAP(&self->c03, &self->c30);
	HYP_SWAP(&self->c12, &self->c21);
	HYP_SWAP(&self->c13, &self->c31);
	HYP_SWAP(&self->c23, &self->c32);

	return self;
}


#ifndef HYP_NO_STDIO
/**
 * @ingroup matrix4
 * @brief Prints out the matrix using column and row notation
 *
 */
HYPAPI void _matrix4_print_with_columnrow_indexer(struct matrix4 *self)
{
	printf("%10f, %10f, %10f, %10f\r\n", self->c00, self->c10, self->c20, self->c30);
	printf("%10f, %10f, %10f, %10f\r\n", self->c01, self->c11, self->c21, self->c31);
	printf("%10f, %10f, %10f, %10f\r\n", self->c02, self->c12, self->c22, self->c32);
	printf("%10f, %10f, %10f, %10f\r\n", self->c03, self->c13, self->c23, self->c33);
}


/**
 * @ingroup matrix4
 * @brief Prints out the matrix using row and column notation
 *
 */
HYPAPI void _matrix4_print_with_rowcolumn_indexer(struct matrix4 *self)
{
	printf("%10f, %10f, %10f, %10f\r\n", self->r00, self->r01, self->r02, self->r03);
	printf("%10f, %10f, %10f, %10f\r\n", self->r10, self->r11, self->r12, self->r13);
	printf("%10f, %10f, %10f, %10f\r\n", self->r20, self->r21, self->r22, self->r23);
	printf("%10f, %10f, %10f, %10f\r\n", self->r30, self->r31, self->r32, self->r33);
}
#endif


/**
 * @ingroup matrix4
 * @brief Randomly fills the matrix with values. Good for testing.
 *
 */
HYPAPI struct matrix4 *_matrix4_set_random(struct matrix4 *self)
{
	uint8_t i;

	for (i = 0; i < 16; i++) {
		self->m[i] = HYP_RANDOM_FLOAT;
	}

	return self;
}


/**
 * @ingroup matrix4
 * @brief converts the quaternion to a 4x4 rotation matrix (column major,
 * right hand rule)
 *
 */
HYPAPI struct matrix4 *matrix4_make_transformation_rotationq(struct matrix4 *self, const struct quaternion *qT)
{
	struct matrix4 *m;
	const struct quaternion *q;

	q = qT;
	m = self;

	matrix4_identity(m);

	m->m[0] = 1.0f - 2.0f * (q->y * q->y + q->z * q->z);
	m->m[4] = 2.0f * (q->x * q->y - q->z * q->w);
	m->m[8] = 2.0f * (q->x * q->z + q->y * q->w);
	m->m[1] = 2.0f * (q->x * q->y + q->z * q->w);
	m->m[5] = 1.0f - 2.0f * (q->x * q->x + q->z * q->z);
	m->m[9] = 2.0f * (q->y * q->z - q->x * q->w);
	m->m[2] = 2.0f * (q->x * q->z - q->y * q->w);
	m->m[6] = 2.0f * (q->y * q->z + q->x * q->w);
	m->m[10] = 1.0f - 2.0f * (q->x * q->x + q->y * q->y);

	return self;
}


/**
 * @ingroup matrix4
 * @brief creates a translation matrix.  It's opinionated about what that means.
 *
 */
HYPAPI struct matrix4 *matrix4_make_transformation_translationv3(struct matrix4 *self, const struct vector3 *translation)
{
	matrix4_identity(self);

	self->c30 = translation->x;
	self->c31 = translation->y;
	self->c32 = translation->z;

	return self;
}


/**
 * @ingroup matrix4
 * @brief creates a scaling matrix.  It's opinionated about what that means.
 *
 */
HYPAPI struct matrix4 *matrix4_make_transformation_scalingv3(struct matrix4 *self, const struct vector3 *scale)
{
	matrix4_identity(self);

	self->c00 = scale->x;
	self->c11 = scale->y;
	self->c22 = scale->z;

	return self;
}


/**
 * @ingroup matrix4
 * @brief creates a rotation matrix about the x.  It's opinionated about
 * what that means.
 *
 * multiply this matrix by another matrix to rotate the other matrix
 */
HYPAPI struct matrix4 *matrix4_make_transformation_rotationf_x(struct matrix4 *m, HYP_FLOAT angle)
{
	HYP_FLOAT c = HYP_COS(angle);
	HYP_FLOAT s = HYP_SIN(angle);

	matrix4_identity(m);

	m->r11 = c;
	m->r12 = s;
	m->r21 = -s;
	m->r22 = c;

	return m;
}


/**
 * @ingroup matrix4
 * @brief creates a rotation matrix about the y.  It's opinionated about
 * what that means.
 *
 * multiply this matrix by another matrix to rotate the other matrix
 */
HYPAPI struct matrix4 *matrix4_make_transformation_rotationf_y(struct matrix4 *m, HYP_FLOAT angle)
{
	HYP_FLOAT c = HYP_COS(angle);
	HYP_FLOAT s = HYP_SIN(angle);

	matrix4_identity(m);

	/* assuming col-major */
	m->r00 = c;
	m->r02 = -s;
	m->r20 = s;
	m->r22 = c;

	return m;
}


/**
 * @ingroup matrix4
 * @brief creates a rotation matrix about the z.  It's opinionated about
 * what that means.
 *
 * multiply this matrix by another matrix to rotate the other matrix
 */
HYPAPI struct matrix4 *matrix4_make_transformation_rotationf_z(struct matrix4 *m, HYP_FLOAT angle)
{
	HYP_FLOAT c = HYP_COS(angle);
	HYP_FLOAT s = HYP_SIN(angle);

	matrix4_identity(m);

	/* assuming col-major */
	m->r00 = c;
	m->r01 = s;
	m->r10 = -s;
	m->r11 = c;

	return m;
}


/**
 * @ingroup matrix4
 * @brief Creates a temporary translation matrix and then multiplies self
 * by that.  Opinionated function about what translation means.
 *
 * @param self The transformation matrix being translated
 * @param translation the translation vector
 *
 */
HYPAPI struct matrix4 *matrix4_translatev3(struct matrix4 *self, const struct vector3 *translation)
{
	struct matrix4 translationMatrix;

	return matrix4_multiply(self,
		matrix4_make_transformation_translationv3(&translationMatrix, translation));
}


/**
 * @ingroup matrix4
 * @brief Creates a temporary rotation matrix and then multiplies self by that.
 * Opinionated function about what rotation means.
 *
 * @param self The transformation matrix being rotated
 * @param axis the axis to rotate the matrix around
 * @param angle the angle of rotation in radians
 *
 */
HYPAPI struct matrix4 *matrix4_rotatev3(struct matrix4 *self, const struct vector3 *axis, HYP_FLOAT angle)
{
	struct matrix4 rotationMatrix;
	struct quaternion q;

	return matrix4_multiply(self,
				matrix4_make_transformation_rotationq(&rotationMatrix,
								      quaternion_set_from_axis_anglev3(&q, axis, angle)));
}


/**
 * @ingroup matrix4
 * @brief Creates a temporary scaling matrix and then multiplies self by that.
 * Opinionated function about what scaling means.
 *
 * @param self The transformation matrix being scaled
 * @param scale the scaling vector
 *
 */
HYPAPI struct matrix4 *matrix4_scalev3(struct matrix4 *self, const struct vector3 *scale)
{
	struct matrix4 scalingMatrix;

	return matrix4_multiply(self,
		matrix4_make_transformation_scalingv3(&scalingMatrix, scale));
}


/**
 * @ingroup matrix4
 * @brief Finds the determinant of a matrix
 *
 * @param self The transformation matrix being questioned
 *
 */
HYPAPI HYP_FLOAT matrix4_determinant(const struct matrix4 *self)
{
	HYP_FLOAT determinant;

	determinant =
	  A4(11, 22, 33, 44) + A4(11, 23, 34, 42) + A4(11, 24, 32, 43)
	+ A4(12, 21, 34, 43) + A4(12, 23, 31, 44) + A4(12, 24, 33, 41)
	+ A4(13, 21, 32, 44) + A4(13, 22, 34, 41) + A4(13, 24, 31, 42)
	+ A4(14, 21, 33, 42) + A4(14, 22, 31, 43) + A4(14, 23, 32, 41)
	- A4(11, 22, 34, 43) - A4(11, 23, 32, 44) - A4(11, 24, 33, 42)
	- A4(12, 21, 33, 44) - A4(12, 23, 34, 41) - A4(12, 24, 31, 43)
	- A4(13, 21, 34, 42) - A4(13, 22, 31, 44) - A4(13, 24, 32, 41)
	- A4(14, 21, 32, 43) - A4(14, 22, 33, 41) - A4(14, 23, 31, 42)
	;

	return determinant;
}


/**
 * @ingroup matrix4
 * @brief Invert a matrix
 *
 * @param self The transformation matrix being inverted
 *
 */
HYPAPI struct matrix4 *matrix4_invert(struct matrix4 *self)
{
	struct matrix4 inverse;
	uint8_t i;

	if (matrix4_inverse(self, &inverse)) {
		for (i = 0; i < 16; i++) {
			self->m[i] = inverse.m[i];
		}
	}

	return self;
}


/**
 * @ingroup matrix4
 * @brief Find the inverse of the matrix
 *
 * @param self The transformation matrix being examined
 * @param mR the inverse of the matrix is returned here
 *
 */
HYPAPI struct matrix4 *matrix4_inverse(const struct matrix4 *self, struct matrix4 *mR)
{
	struct matrix4 inverse;
	HYP_FLOAT determinant;
	uint8_t i;

	determinant = matrix4_determinant(self);

	/* calculated early for a quick exit if no determinant exists */
	if (scalar_equalsf(determinant, 0.0f)) {
		return NULL;
	}

	determinant = 1.0f / determinant;

	matrix4_identity(&inverse);

	B(11) = A3(22, 33, 44) + A3(23, 34, 42) + A3(24, 32, 43) - A3(22, 34, 43) - A3(23, 32, 44) - A3(24, 33, 42);
	B(12) = A3(12, 34, 43) + A3(13, 32, 44) + A3(14, 33, 42) - A3(12, 33, 44) - A3(13, 34, 42) - A3(14, 32, 43);
	B(13) = A3(12, 23, 44) + A3(13, 24, 42) + A3(14, 22, 43) - A3(12, 24, 43) - A3(13, 22, 44) - A3(14, 23, 42);
	B(14) = A3(12, 24, 33) + A3(13, 22, 34) + A3(14, 23, 32) - A3(12, 23, 34) - A3(13, 24, 32) - A3(14, 22, 33);
	B(21) = A3(21, 34, 43) + A3(23, 31, 44) + A3(24, 33, 41) - A3(21, 33, 44) - A3(23, 34, 41) - A3(24, 31, 43);
	B(22) = A3(11, 33, 44) + A3(13, 34, 41) + A3(14, 31, 43) - A3(11, 34, 43) - A3(13, 31, 44) - A3(14, 33, 41);
	B(23) = A3(11, 24, 43) + A3(13, 21, 44) + A3(14, 23, 41) - A3(11, 23, 44) - A3(13, 24, 41) - A3(14, 21, 43);
	B(24) = A3(11, 23, 34) + A3(13, 24, 31) + A3(14, 21, 33) - A3(11, 24, 33) - A3(13, 21, 34) - A3(14, 23, 31);
	B(31) = A3(21, 32, 44) + A3(22, 34, 41) + A3(24, 31, 42) - A3(21, 34, 42) - A3(22, 31, 44) - A3(24, 32, 41);
	B(32) = A3(11, 34, 42) + A3(12, 31, 44) + A3(14, 32, 41) - A3(11, 32, 44) - A3(12, 34, 41) - A3(14, 31, 42);
	B(33) = A3(11, 22, 44) + A3(12, 24, 41) + A3(14, 21, 42) - A3(11, 24, 42) - A3(12, 21, 44) - A3(14, 22, 41);
	B(34) = A3(11, 24, 32) + A3(12, 21, 34) + A3(14, 22, 31) - A3(11, 22, 34) - A3(12, 24, 31) - A3(14, 21, 32);
	B(41) = A3(21, 33, 42) + A3(22, 31, 43) + A3(23, 32, 41) - A3(21, 32, 43) - A3(22, 33, 41) - A3(23, 31, 42);
	B(42) = A3(11, 32, 43) + A3(12, 33, 41) + A3(13, 31, 42) - A3(11, 33, 42) - A3(12, 31, 43) - A3(13, 32, 41);
	B(43) = A3(11, 23, 42) + A3(12, 21, 43) + A3(13, 22, 41) - A3(11, 22, 43) - A3(12, 23, 41) - A3(13, 21, 42);
	B(44) = A3(11, 22, 33) + A3(12, 23, 31) + A3(13, 21, 32) - A3(11, 23, 32) - A3(12, 21, 33) - A3(13, 22, 31);

	/* divide the determinant */
	for (i = 0; i < 16; i++) {
		mR->m[i] = inverse.m[i] * determinant;
	}

	return mR;
}


/**
 * @ingroup quaternion
 * @brief Initializes the vector portion of the quaternion with 0.0 and the
 * scalar portion with 1.0.
 * The resulting quaternion has a norm of 1.0
 *
 * While Hypatia as well as other math libraries call this the 'identity'
 * quaternion, this is not an accurate term.  Since the principle of a
 * quaternion with no rotation works similar to an identity matrix, we let
 * the term pass.
 *
 * @snippet test_quaternion.c quaternion identity example
 */
HYPAPI struct quaternion *quaternion_identity(struct quaternion *self)
{
	self->x = 0.0f;
	self->y = 0.0f;
	self->z = 0.0f;
	self->w = 1.0f;

	return self;
}


/**
 * @ingroup quaternion
 * @brief initializes the quaternion with the passed in x, y, z, w
 */
HYPAPI struct quaternion *quaternion_setf4(struct quaternion *self, HYP_FLOAT x, HYP_FLOAT y, HYP_FLOAT z, HYP_FLOAT w)
{
	self->x = x;
	self->y = y;
	self->z = z;
	self->w = w;

	return self;
}


/**
 * @ingroup quaternion
 * @brief initializes the quaternion by copying the data from qT.  This is
 * effectively a copy function.
 */
HYPAPI struct quaternion *quaternion_set(struct quaternion *self, const struct quaternion *qT)
{
	return quaternion_setf4(self, qT->x, qT->y, qT->z, qT->w);
}


/**
 * @ingroup quaternion
 * @brief Sets the values in the quaternion, in place, based on the axis and
 * angle.
 *
 * @param self the quaternion that will become initialized with the values of
 * the axis and angle
 * @param x the x axis
 * @param y the y axis
 * @param z the z axis
 * @param angle the angle is in radians
 *
 * q = cos(a/2) + i ( x * sin(a/2)) + j (y * sin(a/2)) + k ( z * sin(a/2))
 *
 */
HYPAPI struct quaternion *quaternion_set_from_axis_anglef3(struct quaternion *self, HYP_FLOAT x, HYP_FLOAT y, HYP_FLOAT z, HYP_FLOAT angle)
{
	HYP_FLOAT s = HYP_SIN(angle / 2.0f);
	HYP_FLOAT c = HYP_COS(angle / 2.0f);

	self->x = x * s;
	self->y = y * s;
	self->z = z * s;
	self->w = c;

	/* reduce rounding errors caused by sin/cos */
	return quaternion_normalize(self);
}


/**
 * @ingroup quaternion
 * @brief Sets the values in the quaternion, in place, based on the axis and
 * angle.
 *
 * @param self the quaternion that will become initialized with the values of
 * the axis and angle
 * @param axis the reference axis
 * @param angle the angle is in radians
 *
 */
HYPAPI struct quaternion *quaternion_set_from_axis_anglev3(struct quaternion *self, const struct vector3 *axis, HYP_FLOAT angle)
{
	return quaternion_set_from_axis_anglef3(self, axis->x, axis->y, axis->z, angle);
}


/**
 * @ingroup quaternion
 * @brief Sets the quaterion using euler angles.  This is an opinionated method
 * (opinionated about which axis is yaw, pitch, roll and
 * what is left/right/up/down)
 *
 * @param self the quaternion
 * @param ax the x axis
 * @param ay the y axis
 * @param az the z axis
 *
 */
HYPAPI struct quaternion *quaternion_set_from_euler_anglesf3(struct quaternion *self, HYP_FLOAT ax, HYP_FLOAT ay, HYP_FLOAT az)
{
	self->w = HYP_COS(az / 2.0f) * HYP_COS(ay / 2.0f) * HYP_COS(ax / 2.0f) + HYP_SIN(az / 2.0f) * HYP_SIN(ay / 2.0f) * HYP_SIN(ax / 2.0f);
	self->x = HYP_COS(az / 2.0f) * HYP_COS(ay / 2.0f) * HYP_SIN(ax / 2.0f) - HYP_SIN(az / 2.0f) * HYP_SIN(ay / 2.0f) * HYP_COS(ax / 2.0f);
	self->y = HYP_COS(az / 2.0f) * HYP_SIN(ay / 2.0f) * HYP_COS(ax / 2.0f) + HYP_SIN(az / 2.0f) * HYP_COS(ay / 2.0f) * HYP_SIN(ax / 2.0f);
	self->z = HYP_SIN(az / 2.0f) * HYP_COS(ay / 2.0f) * HYP_COS(ax / 2.0f) - HYP_COS(az / 2.0f) * HYP_SIN(ay / 2.0f) * HYP_SIN(ax / 2.0f);

	quaternion_normalize(self);

	return self;
}


/**
 * @ingroup quaternion
 * @brief Gets the euler angles from the quaternion.  This is an opinionated method
 * (opinionated about which axis is yaw, pitch, roll and
 * what is left/right/up/down)
 *
 * @param self the quaternion
 * @param ax the x axis
 * @param ay the y axis
 * @param az the z axis
 *
 */
HYPAPI void quaternion_get_euler_anglesf3(const struct quaternion *self, HYP_FLOAT *ax, HYP_FLOAT *ay, HYP_FLOAT *az)
{
	HYP_FLOAT qx, qy, qz, qw;

	qw = self->w;
	qx = self->x;
	qy = self->y;
	qz = self->z;

	*ax = HYP_ATAN2(qy * qz + qw * qx, 0.5f - ((qx * qx) + (qy * qy)));
	*ay = HYP_ASIN(-2.0f * ((qx * qz) - (qw * qy)));
	*az = HYP_ATAN2(((qx * qy) + (qw * qz)), 0.5f - ((qy * qy) + (qz * qz)));
}


/**
 * @ingroup quaternion
 * @brief Checks for mathematical equality within EPSILON.
 *
 */
HYPAPI int quaternion_equals(const struct quaternion *self, const struct quaternion *qT)
{
	return HYP_ABS(self->x - qT->x) < HYP_EPSILON &&
		HYP_ABS(self->y - qT->y) < HYP_EPSILON &&
		HYP_ABS(self->z - qT->z) < HYP_EPSILON &&
		HYP_ABS(self->w - qT->w) < HYP_EPSILON;
}


/**
 * @ingroup quaternion
 * @brief Calculates the norm of the quaternion
 *
 * \f$\|q\| = x^2+y^2+z^2+w^2\f$.
 *
 */
HYPAPI HYP_FLOAT quaternion_norm(const struct quaternion *self)
{
	return (self->x * self->x) + (self->y * self->y) + (self->z * self->z) + (self->w * self->w);
}


/**
 * @ingroup quaternion
 * @brief Calculates the magnitude of the quaternion
 *
 * The magnitude is the sqrt of the norm
 *
 * \f$\sqrt{\|q\|}\f$
 */
HYPAPI HYP_FLOAT quaternion_magnitude(const struct quaternion *self)
{
	return HYP_SQRT(quaternion_norm(self));
}


/**
 * @ingroup quaternion
 * @brief Turns the quaternion into its conjugate form
 *
 * \f$q'=-x, -y, -z, w\f$
 *
 * @snippet test_quaternion.c quaternion conjugate example
 */
HYPAPI struct quaternion *quaternion_conjugate(struct quaternion *self)
{
	self->x = -self->x;
	self->y = -self->y;
	self->z = -self->z;
	/* doesn't switch the sign of w (that would be negate) */

	return self;
}


/**
 * @ingroup quaternion
 * @brief negates all parts of the quaternion
 *
 */
HYPAPI struct quaternion *quaternion_negate(struct quaternion *self)
{
	self->x = -self->x;
	self->y = -self->y;
	self->z = -self->z;
	self->w = -self->w;

	return self;
}


/**
 * @ingroup quaternion
 * @brief Calculates the inverse of the quaternion
 *
 * \f$q^{-1} = (q')/\|q\|\f$
 *
 * inverse = conjugate(q) / norm(q)
 *
 * The inverse can be checked by multiplying the inverse against the original
 * quaternion. The result should be the identity.
 *
 * @snippet test_quaternion.c quaternion inverse example
 */
HYPAPI struct quaternion *quaternion_inverse(struct quaternion *self)
{
	HYP_FLOAT norm;

	norm = quaternion_norm(self);

	if (scalar_equalsf(norm, 0.0f)) {
		/* avoid divide by zero */
		return self;
	}

	quaternion_conjugate(self);

	if (scalar_equalsf(norm, 1.0f)) {
		/* we're done */
		return self;
	}

	self->x /= norm;
	self->y /= norm;
	self->z /= norm;
	self->w /= norm;

	return self;
}


/**
 * @ingroup quaternion
 * @brief Constrains the values to sane values.  If this is done enough times
 * during the course of program execution, it has the effect of keeping the
 * values on the manifold.
 *
 */
HYPAPI struct quaternion *quaternion_normalize(struct quaternion *self)
{
	HYP_FLOAT mag;

	mag = quaternion_magnitude(self);

	if (scalar_equalsf(mag, 0.0f)) {
		/* can't normalize a zero
		 * avoid divide by zero
		 */
		return self;
	}

	self->x /= mag;
	self->y /= mag;
	self->z /= mag;
	self->w /= mag;

	return self;
}


/**
 * @ingroup quaternion
 * @brief if the norm is 1.0, then the quaternion is said to be a 'unit
 * quaternion'
 */
HYPAPI short quaternion_is_unit(struct quaternion *self)
{
	return scalar_equalsf(1.0f, quaternion_norm(self));
}


/**
 * @ingroup quaternion
 * @brief if the scalar is 0.0 (w == 0.0), then the quaternion is said to be a
 * 'pure quaternion'
 */
HYPAPI short quaternion_is_pure(struct quaternion *self)
{
	return scalar_equalsf(self->w, 0.0f);
}


/**
 * @ingroup quaternion
 * @brief Normalized LERP.  This calls lerp and then normalizes the quaternion
 * (pulls it back on the manifold)
 *
 * @param percent This refers to how far along we want to go toward end.  1.0
 * means go to the end. 0.0 means start.  1.1 is nonsense. Negative is
 * nonsense.
 * @param start The quaternion representing the starting orientation
 * @param end The quaternion representing the final or ending orientation
 * @param qR The resulting new orientation.
 *
 */
HYPAPI struct quaternion *quaternion_nlerp(const struct quaternion *start, const struct quaternion *end, HYP_FLOAT percent, struct quaternion *qR)
{
	quaternion_lerp(start, end, percent, qR);
	quaternion_normalize(qR);
	return qR;
}


/**
 * @ingroup quaternion
 * @brief Linear interpolates between two quaternions.  It will cause the
 * resulting quaternion to move in a straight line "through the 3-sphere"
 *
 * @param percent This refers to how far along we want to go toward end. 1.0
 * means go to the end. 0.0 means start. 1.1 is nonsense. Negative is nonsense.
 * @param start The quaternion representing the starting orientation
 * @param end The quaternion representing the final or ending orientation
 * @param qR The resulting new orientation.
 *
 */
HYPAPI struct quaternion *quaternion_lerp(const struct quaternion *start, const struct quaternion *end, HYP_FLOAT percent, struct quaternion *qR)
{
	HYP_FLOAT f1, f2;

	/* if percent is 0, return start */
	if (scalar_equalsf(percent, 0.0f)) {
		quaternion_set(qR, start);
		return qR;
	}

	/* if percent is 1 return end */
	if (scalar_equalsf(percent, 1.0f)) {
		quaternion_set(qR, end);
		return qR;
	}

	f1 = 1.0f - percent;
	f2 = percent;

	/* this expanded form avoids calling quaternion_multiply and
	 * quaternion_add
	 */
	qR->w = f1 * start->w + f2 * end->w;
	qR->x = f1 * start->x + f2 * end->x;
	qR->y = f1 * start->y + f2 * end->y;
	qR->z = f1 * start->z + f2 * end->z;

	return qR;
}


/**
 * @ingroup quaternion
 * @brief This computes the SLERP between two quaternions.  It computes that
 * absolute final position interopolated between start and end.
 * This function computes shortest arc.
 * If the start and end result in a negative dot product (reversed direction)
 * then the SLERP will reverse direction.
 *
 * @param percent This refers to how far along we want to go toward end.
 * 1.0 means go to the end. 0.0 means start.  1.1 is nonsense. Negative is
 * nonsense.
 * @param start The quaternion representing the starting orientation
 * @param end The quaternion representing the final or ending orientation
 * @param qR The resulting new orientation.
 *
 */
HYPAPI struct quaternion *quaternion_slerp(const struct quaternion *start, const struct quaternion *end, HYP_FLOAT percent, struct quaternion *qR)
{
	HYP_FLOAT dot;
	HYP_FLOAT f1, f2;
	HYP_FLOAT theta;
	HYP_FLOAT s;
	struct quaternion qneg;

	/* if percent is 0, return start */
	if (scalar_equalsf(percent, 0.0f)) {
		quaternion_set(qR, start);
		return qR;
	}

	/* if percent is 1 return end */
	if (scalar_equalsf(percent, 1.0f)) {
		quaternion_set(qR, end);
		return qR;
	}

	/* how parallel are the quaternions (also the dot is the cosine) */
	dot = quaternion_dot_product(start, end);

	/* if they are close to parallel, use LERP
	 *	- This avoids div/0
	 *	- At small angles, the slerp and lerp are the same
	 */

	if (scalar_equalsf(dot, 1.0f)) {
		quaternion_lerp(start, end, percent, qR);
		return qR;
	}

	/* if dot is negative, they are "pointing" away from one another,
	 * use the shortest arc instead (reverse end and start)
	 * This has the effect of changing the direction of travel around
	 * the sphere beginning with "end" and going the other way around
	 * the sphere
	 */
	if (dot < 0.0f) {
		quaternion_set(&qneg, end);
		/*quaternion_conjugate(&qneg);*/
		quaternion_negate(&qneg);
		quaternion_slerp(start, &qneg, percent, qR);
		quaternion_negate(qR);
		/*quaternion_conjugate(qR);*/
		return qR;
	}

	/* keep the dot product in the range that acos can handle */
	/* (shouldn't get here) */
	HYP_CLAMP(dot, -1.0f, 1.0f);

	/* the angle between start and end in radians */
	theta = HYP_ACOS(dot);
	/* cache */
	s = HYP_SIN(theta);
	/* compute negative */
	f1 = HYP_SIN((1.0 - percent) * theta) / s;
	/* compute positive */
	f2 = HYP_SIN(percent * theta) / s;

	/* this expanded form avoids calling quaternion_multiply
	 * and quaternion_add
	 */
	qR->w = f1 * start->w + f2 * end->w;
	qR->x = f1 * start->x + f2 * end->x;
	qR->y = f1 * start->y + f2 * end->y;
	qR->z = f1 * start->z + f2 * end->z;

	return qR;
}


/**
 * @ingroup quaternion
 * @brief Used by SLERP. This treats the whole quaternion as a 4D vector and
 * computes a regular dot product on it
 *
 */
HYPAPI HYP_FLOAT quaternion_dot_product(const struct quaternion *self, const struct quaternion *qT)
{
	return (self->x * qT->x) + (self->y * qT->y) + (self->z * qT->z) + (self->w * qT->w);
}


/**
 * @ingroup quaternion
 * @brief in place adds each element of the quaternion by the other quaternion.
 * This has a "scaling" effect.
 * To add quaternions, add each element one by one like a vector.
 *
 */
HYPAPI struct quaternion *quaternion_add(struct quaternion *self, const struct quaternion *qT)
{
	self->x += qT->x;
	self->y += qT->y;
	self->z += qT->z;
	self->w += qT->w;

	return self;
}


/**
 * @ingroup quaternion
 * @brief in place subtracts each element of the quaternion by the scalar
 * value. This has a "scaling" effect.
 * to subtract quaternions, subtract each element one by one like a vector
 *
 */
HYPAPI struct quaternion *quaternion_subtract(struct quaternion *self, const struct quaternion *qT)
{
	self->x -= qT->x;
	self->y -= qT->y;
	self->z -= qT->z;
	self->w -= qT->w;

	return self;
}


/**
 * @ingroup quaternion
 * @brief in place multiplies each element of the quaternion by the scalar
 * value. This has a "scaling" effect.
 * to multiply by a scalar, apply it to element one by one like a vector
 *
 */
HYPAPI struct quaternion *quaternion_multiplyf(struct quaternion *self, HYP_FLOAT f)
{
	self->x *= f;
	self->y *= f;
	self->z *= f;
	self->w *= f;

	return self;
}


/**
 * @ingroup quaternion
 * @brief in place multiplies the quaternion by a quaternion
 */
HYPAPI struct quaternion *quaternion_multiply(struct quaternion *self, const struct quaternion *qT)
{
	/* qT is the multiplicand */

	struct quaternion r;

	r.x = self->w * qT->x + self->x * qT->w + self->y * qT->z - self->z * qT->y;
	r.y = self->w * qT->y - self->x * qT->z + self->y * qT->w + self->z * qT->x;
	r.z = self->w * qT->z + self->x * qT->y - self->y * qT->x + self->z * qT->w;
	r.w = self->w * qT->w - self->x * qT->x - self->y * qT->y - self->z * qT->z;

	quaternion_set(self, &r); /* overwrite/save it */

	return self;
}


/**
 * @ingroup quaternion
 * @brief multiplies the quaternion by the vector.
 */
HYPAPI struct quaternion *quaternion_multiplyv3(struct quaternion *self, const struct vector3 *vT)
{
	/* vT is the multiplicand */

	struct quaternion r;

	r.x = self->w * vT->x + self->y * vT->z - self->z * vT->y;
	r.y = self->w * vT->y - self->x * vT->z + self->z * vT->x;
	r.z = self->w * vT->z + self->x * vT->y - self->y * vT->x;
	r.w = self->x * vT->x - self->y * vT->y - self->z * vT->z;

	quaternion_set(self, &r); /* overwrite/save it */

	return self;
}


/**
 * @ingroup quaternion
 * @brief Retrieves the axis and angle from the quaternion, assumes normalized
 * quaternion
 *
 * @param self the quaternion
 * @param vR the axis that will be filled
 * @param angle the angle in radians; will be filled with the angle value
 *
 */
HYPAPI void quaternion_get_axis_anglev3(const struct quaternion *self, struct vector3 *vR, HYP_FLOAT *angle)
{
	/* scale is not same as magnitude */
	HYP_FLOAT scale = HYP_SQRT(1.0f - self->w * self->w);

	/* avoid divide by zero */
	if (scalar_equalsf(scale, 0.0f)) {
		vR->x = self->x;
		vR->y = self->y;
		vR->z = self->z;
	} else {
		vR->x = self->x / scale;
		vR->y = self->y / scale;
		vR->z = self->z / scale;
	}

	*angle = 2.0f * HYP_ACOS(self->w);
}


/**
 * @ingroup quaternion
 * @brief initializes the quaternion with random values, then normalizes it
 */
HYPAPI struct quaternion *_quaternion_set_random(struct quaternion *self)
{
	self->x = HYP_RANDOM_FLOAT;
	self->y = HYP_RANDOM_FLOAT;
	self->z = HYP_RANDOM_FLOAT;
	self->w = HYP_RANDOM_FLOAT;

	quaternion_normalize(self);

	return self;
}


#ifndef HYP_NO_STDIO
/**
 * @ingroup quaternion
 * @brief prints out the elements of the quaternion to stdout
 */
HYPAPI void _quaternion_print(const struct quaternion *self)
{
	printf("x:%10f, y:%10f, z:%10f, w:%10f\r\n", self->x, self->y, self->z, self->w);
}
#endif


/**
 * @ingroup quaternion
 * @brief Given two vectors, find a quaternion that will get you from one to
 * the other
 *
 * @param from the starting vector
 * @param to the ending vector
 * @param qR the resulting quaternion that gets you from the starting vector
 * to the ending vector
 */
HYPAPI struct quaternion *quaternion_get_rotation_tov3(const struct vector3 *from, const struct vector3 *to, struct quaternion *qR)
{
	/* this code avoids sqrt and cos and sin and would be nice to
	 * avoid division
	 */
	struct vector3 w;
	HYP_FLOAT dot;
	HYP_FLOAT norm;

	vector3_cross_product(&w, from, to);
	dot = vector3_dot_product(from, to);

	qR->x = w.x;
	qR->y = w.y;
	qR->z = w.z;
	qR->w = dot;

	norm = quaternion_norm(qR);
	qR->w += norm;

	/* normalization with avoidance of div/0 and reusing the norm */
	/* (already calculated above) */
	if (!scalar_equalsf(norm, 0.0f)) {
		qR->x /= norm;
		qR->y /= norm;
		qR->z /= norm;
		qR->w /= norm;
	}

	return qR;
}



/**
 * @ingroup experimental
 * @brief rotate a quaternion by a quaternion
 * (basically, multiply and then normalize)
 *
 * @param self the quaternion being rotated
 * @param qT the other quaternion
 *
 */
HYPAPI struct quaternion *quaternion_rotate_by_quaternion_EXP(struct quaternion *self, const struct quaternion *qT)
{
	/* self = self * qT */
	quaternion_multiply(self, qT);
	quaternion_normalize(self);

	return self;
}


/**
 * @ingroup experimental
 * @brief rotate a quaternion by the indicated axis and angle
 * first it makes a quaternion from the axis/angle
 * then it rotates the quaternion by that axis/angle
 *
 * @param self the quaternion being rotated
 * @param axis the starting point
 * @param angle the transformed point
 *
 */
HYPAPI struct quaternion *quaternion_rotate_by_axis_angle_EXP(struct quaternion *self, const struct vector3 *axis, HYP_FLOAT angle)
{
	struct quaternion qT;

	quaternion_set_from_axis_anglev3(&qT, axis, angle);
	quaternion_rotate_by_quaternion_EXP(self, &qT);

	return self;
}



/**
 * @ingroup experimental
 * @brief returns a score that seeks to describe the difference
 * between two quaternions
 *
 */
HYPAPI HYP_FLOAT quaternion_difference_EXP(const struct quaternion *q1, const struct quaternion *q2)
{
	struct quaternion diff;

	diff.x = q2->x - q1->x;
	diff.y = q2->y - q1->y;
	diff.z = q2->z - q1->z;
	diff.w = q2->w - q1->w;
	return quaternion_norm(&diff);
}


/**
 * @ingroup experimental
 * @brief this is an opinionated method (opinionated about what axis is
 * yaw, pitch, roll and what is left/right/up/down applies the rotations
 * in this order: x, y, z (yaw, pitch, roll)
 * rotates the quaternion by these angles
 *
 * @param self the quaternion being rotated
 * @param ax yaw
 * @param ay pitch
 * @param az roll
 *
 */
HYPAPI struct quaternion *quaternion_rotate_by_euler_angles_EXP(struct quaternion *self, HYP_FLOAT ax, HYP_FLOAT ay, HYP_FLOAT az)
{
	struct quaternion qT;

	/* make a quaternion from the eulers */
	quaternion_set_from_euler_anglesf3(&qT, ax, ay, az);

	/* rotate the quaternion by it */
	quaternion_rotate_by_quaternion_EXP(self, &qT);

	return self;
}


/**
 * @ingroup experimental
 * @brief This code is suspect.  Computes the cross-product on the vector
 * portion and then something that resembles a negated dot product on the
 * real portion.
 *
 */
HYPAPI struct quaternion quaternion_cross_product_EXP(const struct quaternion *self, const struct quaternion *vT)
{
	/*
	 * The code is suspect (missing w element in this whole thing)
	 * It is computing a cross-product on the vector portion and a
	 * negative dot product on the real portion.
	 */
	struct quaternion r;

	r.x = (self->y * vT->z) - (self->z * vT->y);
	r.y = (self->z * vT->x) - (self->x * vT->z);
	r.z = (self->x * vT->y) - (self->y * vT->x);
	r.w = -((self->x * vT->x) + (self->y * vT->y) + (self->z * vT->z));

	return r;
}


/**
 * @ingroup experimental
 * @brief Computes the angle between two quaternions
 *
 *  * \f$angle= 2 * acos((self \cdot qT) / (||self|| * ||qT||))\f$
 *
 */
HYPAPI HYP_FLOAT quaternion_angle_between_EXP(const struct quaternion *self, const struct quaternion *qT)
{
	HYP_FLOAT c; /* cosine */

	c = quaternion_dot_product(self, qT) / (quaternion_norm(self) * quaternion_norm(qT));

	return 2.0f * HYP_ACOS(c);
}


/**
 * @ingroup experimental
 * @brief This code is suspect. Treats two quaternions sort of like 2 vector4's
 * and then computes the cross-product between them.
 *
 */
HYPAPI void quaternion_axis_between_EXP(const struct quaternion *self, const struct quaternion *qT, struct quaternion *qR)
{
	struct quaternion axis;

	axis = quaternion_cross_product_EXP(self, qT);
	quaternion_set(qR, &axis);
	quaternion_normalize(qR);
}


/**
 * @ingroup experimental
 * @brief creates an perspective projection matrix using the RH system with a
 * FOV about the Y-axis.  It's opinionated about what that means.
 */
HYPAPI struct matrix4 *matrix4_projection_perspective_fovy_rh_EXP(struct matrix4 *self, HYP_FLOAT fovy, HYP_FLOAT aspect, HYP_FLOAT zNear, HYP_FLOAT zFar)
{
	HYP_FLOAT h;
	HYP_FLOAT w;
	HYP_FLOAT p;
	HYP_FLOAT q;

	h = HYP_COT(fovy) / 2.0f;
	w = h * aspect;

	p = zFar / (zNear - zFar);
	q = zNear * p;

	matrix4_zero(self);

	self->r00 = w;
	self->r11 = h;
	self->r22 = p;
	self->r23 = -1.0f; /* this is what makes this RH */
	self->r32 = q;

	return self;
}


/**
 * @ingroup experimental
 * @brief make an orthographic projection matrix with right handed coordinates
 */
HYPAPI struct matrix4 *matrix4_projection_ortho3d_rh_EXP(struct matrix4 *self,
							 HYP_FLOAT xmin, HYP_FLOAT xmax,
							 HYP_FLOAT ymin, HYP_FLOAT ymax,
							 HYP_FLOAT zNear, HYP_FLOAT zFar)
{
	HYP_FLOAT width;
	HYP_FLOAT height;

	matrix4_zero(self);

	width = xmax - xmin;
	height = ymax - ymin;

	self->c00 = 2.0f / width;
	self->c11 = 2.0f / height;
	self->c22 = 1.0f / (zFar - zNear);
	self->c23 = zNear / (zNear - zFar);
	self->c33 = 1.0f;

	return self;
}


/**
 * @ingroup experimental
 * @brief Multiply a matrix by a vector, returns a vector
 *
 * @param m The matrix being multiplied
 * @param vT The vector being multiplied
 * @param vR The result
 */
HYPAPI struct vector3 *matrix4_multiplyv3_EXP(const struct matrix4 *m, const struct vector3 *vT, struct vector3 *vR)
{
	vR->x = vT->x * m->c00 + vT->y * m->c01 + vT->z * m->c02 + m->c03;
	vR->y = vT->x * m->c10 + vT->y * m->c11 + vT->z * m->c12 + m->c13;
	vR->z = vT->x * m->c20 + vT->y * m->c21 + vT->z * m->c22 + m->c23;

	return vR;
}


/**
 * @ingroup experimental
 * @brief Opinionated function about what the axis means.  Sets the axis and
 * angle (used as a rotation matrix)
 *
 * @param self The matrix
 * @param x The x part of the axis
 * @param y The y part of the axis
 * @param z The z part of the axis
 * @param angle the angle in radians
 *
 */
HYPAPI struct matrix4 *matrix4_set_from_axisf3_angle_EXP(struct matrix4 *self, HYP_FLOAT x, HYP_FLOAT y, HYP_FLOAT z, const HYP_FLOAT angle)
{
	HYP_FLOAT c = HYP_COS(angle);
	HYP_FLOAT s = HYP_SIN(angle);

	self->c00 = (x * x) * (1.0f - c) + c;
	self->c01 = (x * y) * (1.0f - c) + (z * s);
	self->c02 = (x * z) * (1.0f - c) - (y * s);
	self->c03 = 0.0f;

	self->c10 = (y * x) * (1.0f - c) - (z * s);
	self->c11 = (y * y) * (1.0f - c) + c;
	self->c12 = (y * z) * (1.0f - c) + (x * s);
	self->c13 = 0.0f;

	self->c20 = (z * x) * (1.0f - c) + (y * s);
	self->c21 = (z * y) * (1.0f - c) - (x * s);
	self->c22 = (z * z) * (1.0f - c) + c;
	self->c23 = 0.0f;

	self->c30 = 0.0f;
	self->c31 = 0.0f;
	self->c32 = 0.0f;
	self->c33 = 1.0f;

	return self;
}


/**
 * @ingroup experimental
 * @brief Opinionated function about what the axis means.  Sets the axis and
 * angle (used as a rotation matrix)
 *
 * @param self The matrix
 * @param axis The axis
 * @param angle the angle in radians
 *
 */
HYPAPI struct matrix4 *matrix4_set_from_axisv3_angle_EXP(struct matrix4 *self, const struct vector3 *axis, HYP_FLOAT angle)
{
	return matrix4_set_from_axisf3_angle_EXP(self, axis->x, axis->y, axis->z, angle);
}


HYPAPI struct matrix4 *matrix4_set_from_euler_anglesf3_EXP(struct matrix4 *self, const HYP_FLOAT x, const HYP_FLOAT y, const HYP_FLOAT z)
{
	HYP_FLOAT A = HYP_COS(x);
	HYP_FLOAT B = HYP_SIN(x);
	HYP_FLOAT C = HYP_COS(y);
	HYP_FLOAT D = HYP_SIN(y);
	HYP_FLOAT E = HYP_COS(z);
	HYP_FLOAT F = HYP_SIN(z);

	HYP_FLOAT AD = A * D;
	HYP_FLOAT BD = B * D;

	self->c00 = C * E;
	self->c01 = -C * F;
	self->c02 = D;
	self->c03 = 0.0f;

	self->c10 = BD * E + A * F;
	self->c11 = -BD * F + A * E;
	self->c12 = -B * C;
	self->c13 = 0.0f;

	self->c20 = -AD * E + B * F;
	self->c21 = AD * F + B * E;
	self->c22 = A * C;
	self->c23 = 0.0f;

	self->c30 = 0.0f;
	self->c31 = 0.0f;
	self->c32 = 0.0f;
	self->c33 = 1.0f;

	return self;
}


HYPAPI struct vector3 *matrix4_get_translation_EXP(const struct matrix4 *self, struct vector3 *vT)
{
	vT->x = self->c30;
	vT->y = self->c31;
	vT->z = self->c32;

	return vT;
}


/**
 * @ingroup experimental
 * @brief creates a look at matrix using the RH system.
 */
HYPAPI struct matrix4 *matrix4_view_lookat_rh_EXP(struct matrix4 *self, const struct vector3 *eye, const struct vector3 *target, const struct vector3 *up)
{
	struct vector3 yaxis;
	struct vector3 zaxis;
	struct vector3 xaxis;

	zaxis.x = target->x - eye->x;
	zaxis.y = target->y - eye->y;
	zaxis.z = target->z - eye->z;
	vector3_normalize(&zaxis);

	/* xaxis = zaxis x up */
	vector3_cross_product(&xaxis, &zaxis, up);
	vector3_normalize(&xaxis);

	/* yaxis = xaxis x zaxis */
	vector3_cross_product(&yaxis, &xaxis, &zaxis);

	matrix4_identity(self);

	self->c00 = xaxis.x;
	self->c10 = xaxis.y;
	self->c20 = xaxis.z;

	self->c01 = yaxis.x;
	self->c11 = yaxis.y;
	self->c21 = yaxis.z;

	self->c02 = -zaxis.x;
	self->c12 = -zaxis.y;
	self->c22 = -zaxis.z;

	self->c30 = -vector3_dot_product(&xaxis, eye);
	self->c31 = -vector3_dot_product(&yaxis, eye);
	self->c32 = -vector3_dot_product(&zaxis, eye);

	return self;
}


/**
 * @ingroup matrix4
 * @brief Creates a temporary rotation matrix and then multiplies self by that.
 * Opinionated function about what rotation means.
 *
 * @param self The transformation matrix being rotated
 * @param vR the euler angles
 *
 */
HYPAPI struct matrix4 *matrix4_make_transformation_rotationv3_EXP(struct matrix4 *self, const struct vector3 *vR)
{
	struct matrix4 scratchMatrix;

	matrix4_identity(self);

	return matrix4_multiply(self,
		matrix4_set_from_euler_anglesf3_EXP(&scratchMatrix, vR->x, vR->y, vR->z));
}


HYPAPI struct matrix4 *matrix4_transformation_compose_EXP(struct matrix4 *self, const struct vector3 *scale, const struct quaternion *rotation, const struct vector3 *translation)
{
	struct matrix4 scaleM, rotateM;

	matrix4_identity(self);

	/* scale */
	matrix4_multiply(self, matrix4_make_transformation_scalingv3(&scaleM, scale));

	/* rotate */
	matrix4_multiply(self, matrix4_make_transformation_rotationq(&rotateM, rotation));

	/* translate */
	self->c30 = translation->x;
	self->c31 = translation->y;
	self->c32 = translation->z;

	return self;
}

HYPAPI uint8_t matrix4_transformation_decompose_EXP(struct matrix4 *self, struct vector3 *scale, struct quaternion *rotation, struct vector3 *translation)
{
	HYP_FLOAT signx, signy, signz;

	/* translation */
	translation->x = self->c30;
	translation->y = self->c31;
	translation->z = self->c32;

	/*
	 * self->c00 = scale->x;
	 * self->c11 = scale->y;
	 * self->c22 = scale->z;
	 */

	/* sign */
	signx = ((self->c00 * self->c01 * self->c02 * self->c03) < 0) ? -1.0f : 1.0f;
	signy = ((self->c10 * self->c11 * self->c12 * self->c13) < 0) ? -1.0f : 1.0f;
	signz = ((self->c20 * self->c21 * self->c22 * self->c23) < 0) ? -1.0f : 1.0f;

	/* scale */
	scale->x = signx * HYP_SQRT(self->c00 * self->c00 + self->c01 * self->c01 + self->c02 * self->c02);
	scale->y = signy * HYP_SQRT(self->c10 * self->c10 + self->c11 * self->c11 + self->c12 * self->c12);
	scale->z = signz * HYP_SQRT(self->c20 * self->c20 + self->c21 * self->c21 + self->c22 * self->c22);

	/* todo */
	quaternion_identity(rotation);

	return 1;
}
#endif /* HYPATIA_IMPLEMENTATION */
