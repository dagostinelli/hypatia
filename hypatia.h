/* SPDX-License-Identifier: MIT */

#ifndef HYPATIA_H_
#define HYPATIA_H_

#define HYPATIA_VERSION "2.1.0-dev"

#ifndef HYP_INLINE
#	ifdef _MSC_VER
#		define HYP_INLINE __inline
#	else
#		define HYP_INLINE __inline__
#	endif
#endif

/* with HYP_STATIC every function is private to the file that includes the
 * implementation; inline keeps compilers from warning about the ones that file
 * does not use
 */
#ifndef HYPAPI
#	ifdef HYP_STATIC
#		define HYPAPI static HYP_INLINE
#	else
#		define HYPAPI
#	endif
#endif

#ifndef HYP_FLOAT
#	ifdef HYPATIA_SINGLE_PRECISION_FLOATS
#		define HYP_FLOAT float
#	else
#		define HYP_FLOAT double
#	endif
#endif

/** @brief Makes a HYP_FLOAT constant from a floating point literal, in the
 * style of INT64_C in <stdint.h>.  It adds the suffix that matches HYP_FLOAT,
 * so the constant is rounded once, straight to the precision in use.
 */
#ifndef HYP_FLOAT_C
#	ifdef HYPATIA_SINGLE_PRECISION_FLOATS
#		define HYP_FLOAT_C(x) x ## f
#	else
#		define HYP_FLOAT_C(x) x
#	endif
#endif

#ifndef HYP_NO_C_MATH
#	include <math.h> /* sin, cos, acos, fmod */
#endif

#ifndef HYP_NO_STDIO
#	include <stdio.h> /* printf */
#endif

/** @defgroup _constants Constants */
/** @defgroup trig Trigonometry */
/** @defgroup reference_vectors Reference vectors */
/** @defgroup vector2 vector2 */
/** @defgroup vector3 vector3 */
/** @defgroup vector4 vector4 */
/** @defgroup quaternion quaternion */
/** @defgroup matrix Matrices */
/** @defgroup matrix2 matrix2 */
/** @defgroup matrix3 matrix3 */
/** @defgroup matrix4 matrix4 */
/** @defgroup experimental Experimental */

/**
 * @ingroup _constants
 * @{
 */

/** @brief PI to 100 digits (gets rounded off by the compiler) */
#ifndef HYP_PI
#	define HYP_PI HYP_FLOAT_C(3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679)
#endif
/** @brief Tau to 100 digits, which is 2 * PI */
#ifndef HYP_TAU
#	define HYP_TAU HYP_FLOAT_C(6.2831853071795864769252867665590057683943387987502116419498891846156328125724179972560696506842341359)
#endif
/** @brief Half of PI */
#ifndef HYP_PI_HALF
#	define HYP_PI_HALF HYP_FLOAT_C(1.570796326794896619231321691639751442099)
#endif
/** @brief PI * PI */
#ifndef HYP_PI_SQUARED
#	define HYP_PI_SQUARED HYP_FLOAT_C(9.869604401089358618834490999876151135314)
#endif
/** @brief e, the base of the natural logarithm */
#ifndef HYP_E
#	define HYP_E HYP_FLOAT_C(2.71828182845904523536028747135266249775724709369995)
#endif
/** @brief Radians per Degree = PI/180 */
#ifndef HYP_RAD_PER_DEG
#	define HYP_RAD_PER_DEG HYP_FLOAT_C(0.0174532925199432957692369076848861)
#endif
/** @brief Degrees per Radian = 180/PI */
#ifndef HYP_DEG_PER_RAD
#	define HYP_DEG_PER_RAD HYP_FLOAT_C(57.2957795130823208767981548141052)
#endif
#ifndef HYP_NO_DEPRECATED
/** @brief PI/180
 *
 * @deprecated Use HYP_RAD_PER_DEG.  HYP_PIOVER180 will be removed in a later
 * release; define HYP_NO_DEPRECATED to check that your code no longer uses it.
 */
#	ifndef HYP_PIOVER180
#		define HYP_PIOVER180  HYP_RAD_PER_DEG
#	endif
/** @brief 180/PI
 *
 * @deprecated Use HYP_DEG_PER_RAD.  HYP_PIUNDER180 will be removed in a later
 * release; define HYP_NO_DEPRECATED to check that your code no longer uses it.
 */
#	ifndef HYP_PIUNDER180
#		define HYP_PIUNDER180 HYP_DEG_PER_RAD
#	endif
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
/** @} */

/** @brief A macro that enabled you to override memset */
#ifndef HYP_MEMSET
#	include <string.h> /* memset */
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
	return (a > b) ? a : b;
}

/** @brief A macro that swaps \a a and \a b */
static HYP_INLINE void HYP_SWAP(HYP_FLOAT *a, HYP_FLOAT *b)
{
	HYP_FLOAT f = *a; *a = *b; *b = f;
}

#ifndef HYP_NO_DEPRECATED
/** @brief A macro that returns a random number in the range [-1, 1].
 * It is the difference of two rand() values divided by RAND_MAX, which gives
 * a triangular distribution centered on 0.
 *
 * @deprecated Use scalar_random_rangef(-1, 1).  To supply your own generator,
 * define HYP_RANDOM and HYP_RANDOM_MAX.  HYP_RANDOM_FLOAT will be removed in a
 * later release; define HYP_NO_DEPRECATED to check that your code no longer
 * uses it.
 */
#	ifndef HYP_RANDOM_FLOAT
#		include <stdlib.h> /* RAND_MAX, rand */
#		define HYP_RANDOM_FLOAT (((HYP_FLOAT)rand() - (HYP_FLOAT)rand()) / (HYP_FLOAT)RAND_MAX)
#	endif
#endif

#if defined(HYP_RANDOM) && !defined(HYP_RANDOM_MAX)
#	error "HYP_RANDOM needs HYP_RANDOM_MAX"
#elif !defined(HYP_RANDOM) && defined(HYP_RANDOM_MAX)
#	error "HYP_RANDOM_MAX needs HYP_RANDOM"
#endif

/** @brief A macro that returns a random integer from 0 to HYP_RANDOM_MAX.
 * It is the source for scalar_random_rangef.  Define HYP_RANDOM and HYP_RANDOM_MAX
 * before including this header to use a different generator.
 */
#ifndef HYP_RANDOM
#	include <stdlib.h> /* rand */
#	define HYP_RANDOM() rand()
#endif

/** @brief The largest value HYP_RANDOM can return */
#ifndef HYP_RANDOM_MAX
#	include <stdlib.h> /* RAND_MAX */
#	define HYP_RANDOM_MAX RAND_MAX
#endif

/** @brief A macro that converts an angle in degrees to an angle in radians */
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

/** @brief A macro that computes the floating-point remainder */
#ifndef HYP_FMOD
#	define HYP_FMOD(x, y) ((HYP_FLOAT)fmod(x, y))
#endif

/** @brief A macro that returns the absolute value */
static HYP_INLINE HYP_FLOAT HYP_ABS(HYP_FLOAT value)
{
	return (value < HYP_FLOAT_C(0.0)) ? -value : value;
}

/** @brief A macro that wraps a value around and around in the range [start, limit).
 * start must not be greater than limit; when they are equal the result is start.
 */
static HYP_INLINE HYP_FLOAT HYP_WRAP(HYP_FLOAT value, HYP_FLOAT start, HYP_FLOAT limit)
{
	HYP_FLOAT range = limit - start;
	HYP_FLOAT offset;
	HYP_FLOAT result;

	/* a range of zero width holds only start (written without == for
	 * -Wfloat-equal)
	 */
	if (!(range < HYP_FLOAT_C(0.0)) && !(range > HYP_FLOAT_C(0.0))) {
		return start;
	}

	offset = HYP_FMOD(value - start, range);

	/* fmod keeps the sign of its first argument */
	if (offset < HYP_FLOAT_C(0.0)) {
		offset += range;
	}

	result = start + offset;

	/* rounding can carry a value just below a multiple of the range up to
	 * limit, which is the same point as start
	 */
	if (result >= limit) {
		result = start;
	}

	return result;
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
struct matrix2;
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


HYPAPI short scalar_equalsf(const HYP_FLOAT f1, const HYP_FLOAT f2);
HYPAPI short scalar_equals_epsilonf(const HYP_FLOAT f1, const HYP_FLOAT f2, const HYP_FLOAT epsilon);
HYPAPI HYP_FLOAT scalar_random_rangef(HYP_FLOAT min, HYP_FLOAT max);

#define scalar_equals scalar_equalsf


/**
 * @ingroup trig
 * @{
 */

#ifndef HYP_SIN
#	define HYP_SIN(x) ((HYP_FLOAT)sin(x))
#endif
#ifndef HYP_COS
#	define HYP_COS(x) ((HYP_FLOAT)cos(x))
#endif
#ifndef HYP_TAN
#	define HYP_TAN(x) ((HYP_FLOAT)tan(x))
#endif
#ifndef HYP_ASIN
#	define HYP_ASIN(x) ((HYP_FLOAT)asin(x))
#endif
#ifndef HYP_ACOS
#	define HYP_ACOS(x) ((HYP_FLOAT)acos(x))
#endif
#ifndef HYP_ATAN2
#	define HYP_ATAN2(y, x) ((HYP_FLOAT)atan2(y, x))
#endif
#ifndef HYP_COT
#	define HYP_COT(a) (HYP_FLOAT_C(1.0) / HYP_TAN(a))
#endif

/** @} */


/**
 * @ingroup experimental
 * @{
 */

#ifndef HYP_NO_STDIO
HYPAPI void hyp_matrix2_print_with_columnrow_indexer(struct matrix2 *self);
HYPAPI void hyp_matrix2_print_with_rowcolumn_indexer(struct matrix2 *self);

HYPAPI void hyp_matrix3_print_with_columnrow_indexer(struct matrix3 *self);
HYPAPI void hyp_matrix3_print_with_rowcolumn_indexer(struct matrix3 *self);

HYPAPI void hyp_matrix4_print_with_columnrow_indexer(struct matrix4 *self);
HYPAPI void hyp_matrix4_print_with_rowcolumn_indexer(struct matrix4 *self);

HYPAPI void hyp_quaternion_print(const struct quaternion *self);

HYPAPI void hyp_vector3_print(const struct vector3 *self);

HYPAPI void hyp_vector2_print(const struct vector2 *self);

HYPAPI void hyp_vector4_print(const struct vector4 *self);
#endif

/** @} */

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
HYPAPI struct vector2 *vector2_set_random_unit(struct vector2 *self);
HYPAPI struct vector2 *vector2_set_random_in_disk(struct vector2 *self);
HYPAPI struct vector2 *vector2_negate(struct vector2 *self);
HYPAPI struct vector2 *vector2_lerp(const struct vector2 *start, const struct vector2 *end, HYP_FLOAT percent, struct vector2 *vR);
HYPAPI struct vector2 *vector2_clamp(struct vector2 *self, const struct vector2 *vMin, const struct vector2 *vMax);
HYPAPI struct vector2 *vector2_min(struct vector2 *self, const struct vector2 *vT);
HYPAPI struct vector2 *vector2_max(struct vector2 *self, const struct vector2 *vT);
HYPAPI struct vector2 *vector2_project(struct vector2 *self, const struct vector2 *onto);
HYPAPI struct vector2 *vector2_reflect(struct vector2 *self, const struct vector2 *normal);
HYPAPI struct vector2 *vector2_add(struct vector2 *self, const struct vector2 *vT);
HYPAPI struct vector2 *vector2_addf(struct vector2 *self, HYP_FLOAT fT);
HYPAPI struct vector2 *vector2_subtract(struct vector2 *self, const struct vector2 *vT);
HYPAPI struct vector2 *vector2_subtractf(struct vector2 *self, HYP_FLOAT fT);
HYPAPI struct vector2 *vector2_multiply(struct vector2 *self, const struct vector2 *vT);
HYPAPI struct vector2 *vector2_multiplyf(struct vector2 *self, HYP_FLOAT fT);
HYPAPI struct vector2 *vector2_multiplym2(struct vector2 *self, const struct matrix2 *mT);
HYPAPI struct vector2 *vector2_multiplym3(struct vector2 *self, const struct matrix3 *mT);
HYPAPI struct vector2 *vector2_divide(struct vector2 *self, const struct vector2 *vT);
HYPAPI struct vector2 *vector2_dividef(struct vector2 *self, HYP_FLOAT fT);

HYPAPI struct vector2 *vector2_normalize(struct vector2 *self);
HYPAPI HYP_FLOAT vector2_magnitude(const struct vector2 *self);
HYPAPI HYP_FLOAT vector2_distance(const struct vector2 *v1, const struct vector2 *v2);

HYPAPI HYP_FLOAT vector2_dot_product(const struct vector2 *self, const struct vector2 *vT);
HYPAPI HYP_FLOAT vector2_cross_product(const struct vector2 *vT1, const struct vector2 *vT2);

HYPAPI HYP_FLOAT vector2_angle_between(const struct vector2 *self, const struct vector2 *vT);

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
HYPAPI struct vector3 *vector3_set_random_unit(struct vector3 *self);
HYPAPI struct vector3 *vector3_set_random_in_ball(struct vector3 *self);
HYPAPI struct vector3 *vector3_set_random_in_cone(struct vector3 *self, const struct vector3 *axis, HYP_FLOAT angle);
HYPAPI struct vector3 *vector3_negate(struct vector3 *self);
HYPAPI struct vector3 *vector3_lerp(const struct vector3 *start, const struct vector3 *end, HYP_FLOAT percent, struct vector3 *vR);
HYPAPI struct vector3 *vector3_clamp(struct vector3 *self, const struct vector3 *vMin, const struct vector3 *vMax);
HYPAPI struct vector3 *vector3_min(struct vector3 *self, const struct vector3 *vT);
HYPAPI struct vector3 *vector3_max(struct vector3 *self, const struct vector3 *vT);
HYPAPI struct vector3 *vector3_project(struct vector3 *self, const struct vector3 *onto);
HYPAPI struct vector3 *vector3_reflect(struct vector3 *self, const struct vector3 *normal);
HYPAPI struct vector3 *vector3_refract(struct vector3 *self, const struct vector3 *normal, HYP_FLOAT eta);
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
HYPAPI struct vector4 *vector4_set_random_unit(struct vector4 *self);
HYPAPI struct vector4 *vector4_negate(struct vector4 *self);
HYPAPI struct vector4 *vector4_lerp(const struct vector4 *start, const struct vector4 *end, HYP_FLOAT percent, struct vector4 *vR);
HYPAPI struct vector4 *vector4_clamp(struct vector4 *self, const struct vector4 *vMin, const struct vector4 *vMax);
HYPAPI struct vector4 *vector4_min(struct vector4 *self, const struct vector4 *vT);
HYPAPI struct vector4 *vector4_max(struct vector4 *self, const struct vector4 *vT);
HYPAPI struct vector4 *vector4_project(struct vector4 *self, const struct vector4 *onto);
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


struct matrix2 {
	union {
		HYP_FLOAT m[4]; /* row-major numbering */
		struct {
			/* reference the matrix [row][column] */
			HYP_FLOAT m22[2][2];
		};
		struct {
			/* indexed (column-major numbering) */
			HYP_FLOAT i00, i02;
			HYP_FLOAT i01, i03;
		};
		struct {
			/* col-row */
			HYP_FLOAT c00, c10;
			HYP_FLOAT c01, c11;
		};
		struct {
			/* row-col */
			HYP_FLOAT r00, r01;
			HYP_FLOAT r10, r11;
		};
	};
};


HYPAPI int matrix2_equals(const struct matrix2 *self, const struct matrix2 *mT);

HYPAPI struct matrix2 *matrix2_zero(struct matrix2 *self);
HYPAPI struct matrix2 *matrix2_identity(struct matrix2 *self);
HYPAPI struct matrix2 *matrix2_set(struct matrix2 *self, const struct matrix2 *mT);
HYPAPI struct matrix2 *matrix2_add(struct matrix2 *self, const struct matrix2 *mT);
HYPAPI struct matrix2 *matrix2_subtract(struct matrix2 *self, const struct matrix2 *mT);

HYPAPI struct matrix2 *matrix2_multiply(struct matrix2 *self, const struct matrix2 *mT);
HYPAPI struct matrix2 *matrix2_multiplyf(struct matrix2 *self, HYP_FLOAT scalar);
HYPAPI struct vector2 *matrix2_multiplyv2(const struct matrix2 *self, const struct vector2 *vT, struct vector2 *vR);

HYPAPI struct matrix2 *matrix2_transpose(struct matrix2 *self);
HYPAPI HYP_FLOAT matrix2_determinant(const struct matrix2 *self);
HYPAPI struct matrix2 *matrix2_invert(struct matrix2 *self);
HYPAPI struct matrix2 *matrix2_inverse(const struct matrix2 *self, struct matrix2 *mR);
HYPAPI HYP_FLOAT matrix2_reciprocal_condition(const struct matrix2 *self);

HYPAPI struct matrix2 *matrix2_make_transformation_scalingv2(struct matrix2 *self, const struct vector2 *scale);
HYPAPI struct matrix2 *matrix2_make_transformation_rotationf_z(struct matrix2 *self, HYP_FLOAT angle);

HYPAPI struct matrix2 *matrix2_rotate(struct matrix2 *self, HYP_FLOAT angle);
HYPAPI struct matrix2 *matrix2_scalev2(struct matrix2 *self, const struct vector2 *scale);

HYPAPI struct matrix2 *hyp_matrix2_transpose_rowcolumn(struct matrix2 *self);
HYPAPI struct matrix2 *hyp_matrix2_transpose_columnrow(struct matrix2 *self);


#ifndef DOXYGEN_SHOULD_SKIP_THIS

/* BETA aliases */
#define mat2 struct matrix2

#define mat2_equals matrix2_equals
#define mat2_zero matrix2_zero
#define mat2_identity matrix2_identity
#define mat2_set matrix2_setm2
#define mat2_add matrix2_add
#define mat2_sub matrix2_subtract
#define mat2_mul matrix2_multiply
#define mat2_transpose matrix2_transpose

#define mat2_rotate matrix2_rotate
#define mat2_scalev2 matrix2_scalev2


/*#define m2 struct matrix2*/

#define m2_equals matrix2_equals
#define m2_zero matrix2_zero
#define m2_identity matrix2_identity
#define m2_set matrix2_set
#define m2_add matrix2_add
#define m2_sub matrix2_subtract
#define m2_mul matrix2_multiply
#define m2_transpose matrix2_transpose

#define m2_rotate matrix2_rotate
#define m2_scalev2 matrix2_scalev2

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
HYPAPI HYP_FLOAT matrix3_reciprocal_condition(const struct matrix3 *self);

HYPAPI struct matrix3 *matrix3_make_transformation_translationv2(struct matrix3 *self, const struct vector2 *translation);
HYPAPI struct matrix3 *matrix3_make_transformation_scalingv2(struct matrix3 *self, const struct vector2 *scale);
HYPAPI struct matrix3 *matrix3_make_transformation_rotationf_z(struct matrix3 *self, HYP_FLOAT angle);

HYPAPI struct matrix3 *matrix3_translatev2(struct matrix3 *self, const struct vector2 *translation);
HYPAPI struct matrix3 *matrix3_rotate(struct matrix3 *self, HYP_FLOAT angle);
HYPAPI struct matrix3 *matrix3_scalev2(struct matrix3 *self, const struct vector2 *scale);

HYPAPI struct matrix3 *hyp_matrix3_transpose_rowcolumn(struct matrix3 *self);
HYPAPI struct matrix3 *hyp_matrix3_transpose_columnrow(struct matrix3 *self);


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
HYPAPI HYP_FLOAT matrix4_reciprocal_condition(const struct matrix4 *self);
HYPAPI struct matrix4 *matrix4_normal_matrix(const struct matrix4 *self, struct matrix4 *mR);

HYPAPI struct matrix4 *matrix4_make_transformation_translationv3(struct matrix4 *self, const struct vector3 *translation);
HYPAPI struct matrix4 *matrix4_make_transformation_scalingv3(struct matrix4 *self, const struct vector3 *scale);
HYPAPI struct matrix4 *matrix4_make_transformation_rotationq(struct matrix4 *self, const struct quaternion *qT);
HYPAPI struct matrix4 *matrix4_make_transformation_rotationf_x(struct matrix4 *self, HYP_FLOAT angle);
HYPAPI struct matrix4 *matrix4_make_transformation_rotationf_y(struct matrix4 *self, HYP_FLOAT angle);
HYPAPI struct matrix4 *matrix4_make_transformation_rotationf_z(struct matrix4 *self, HYP_FLOAT angle);

HYPAPI struct matrix4 *matrix4_translatev3(struct matrix4 *self, const struct vector3 *translation);
HYPAPI struct matrix4 *matrix4_rotatev3(struct matrix4 *self, const struct vector3 *axis, HYP_FLOAT angle);
HYPAPI struct matrix4 *matrix4_scalev3(struct matrix4 *self, const struct vector3 *scale);

HYPAPI struct matrix4 *hyp_matrix4_transpose_rowcolumn(struct matrix4 *self);
HYPAPI struct matrix4 *hyp_matrix4_transpose_columnrow(struct matrix4 *self);


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
HYPAPI struct quaternion *quaternion_set_random_unit(struct quaternion *self);
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
HYPAPI struct quaternion *quaternion_set_from_matrix4(struct quaternion *self, const struct matrix4 *mT);
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


#include <stdint.h> /* uint8_t; C99, kept by design (see the README coding standard) */

HYPAPI struct quaternion *quaternion_rotate_by_quaternion(struct quaternion *self, const struct quaternion *qT);
HYPAPI struct quaternion *quaternion_rotate_by_axis_angle(struct quaternion *self, const struct vector3 *axis, HYP_FLOAT angle);
HYPAPI struct quaternion *quaternion_rotate_by_euler_angles(struct quaternion *self, HYP_FLOAT ax, HYP_FLOAT ay, HYP_FLOAT az);
HYPAPI HYP_FLOAT quaternion_difference(const struct quaternion *q1, const struct quaternion *q2);
HYPAPI HYP_FLOAT quaternion_angle_between(const struct quaternion *self, const struct quaternion *qT);
HYPAPI void quaternion_axis_between_EXP(const struct quaternion *self, const struct quaternion *qT, struct quaternion *qR);
HYPAPI struct matrix4 *matrix4_projection_perspective_fovy_rh(struct matrix4 *self, HYP_FLOAT fovy, HYP_FLOAT aspect, HYP_FLOAT zNear, HYP_FLOAT zFar);
HYPAPI struct matrix4 *matrix4_projection_ortho3d_rh(struct matrix4 *self, HYP_FLOAT xmin, HYP_FLOAT xmax, HYP_FLOAT ymin, HYP_FLOAT ymax, HYP_FLOAT zNear, HYP_FLOAT zFar);
HYPAPI struct matrix4 *matrix4_view_lookat_rh(struct matrix4 *self, const struct vector3 *eye, const struct vector3 *target, const struct vector3 *up);
HYPAPI struct matrix4 *matrix4_projection_perspective_fovy_lh(struct matrix4 *self, HYP_FLOAT fovy, HYP_FLOAT aspect, HYP_FLOAT zNear, HYP_FLOAT zFar);
HYPAPI struct matrix4 *matrix4_projection_perspective_fovy_infinite_rh(struct matrix4 *self, HYP_FLOAT fovy, HYP_FLOAT aspect, HYP_FLOAT zNear);
HYPAPI struct matrix4 *matrix4_projection_perspective_fovy_infinite_lh(struct matrix4 *self, HYP_FLOAT fovy, HYP_FLOAT aspect, HYP_FLOAT zNear);
HYPAPI struct matrix4 *matrix4_projection_frustum_rh(struct matrix4 *self, HYP_FLOAT xmin, HYP_FLOAT xmax, HYP_FLOAT ymin, HYP_FLOAT ymax, HYP_FLOAT zNear, HYP_FLOAT zFar);
HYPAPI struct matrix4 *matrix4_projection_frustum_lh(struct matrix4 *self, HYP_FLOAT xmin, HYP_FLOAT xmax, HYP_FLOAT ymin, HYP_FLOAT ymax, HYP_FLOAT zNear, HYP_FLOAT zFar);
HYPAPI struct vector3 *vector3_project_to_window(struct vector3 *self, const struct matrix4 *transform, const struct vector4 *viewport);
HYPAPI struct vector3 *vector3_unproject_from_window(struct vector3 *self, const struct matrix4 *transform, const struct vector4 *viewport);
HYPAPI struct matrix4 *matrix4_projection_ortho3d_lh(struct matrix4 *self, HYP_FLOAT xmin, HYP_FLOAT xmax, HYP_FLOAT ymin, HYP_FLOAT ymax, HYP_FLOAT zNear, HYP_FLOAT zFar);
HYPAPI struct matrix4 *matrix4_view_lookat_lh(struct matrix4 *self, const struct vector3 *eye, const struct vector3 *target, const struct vector3 *up);
HYPAPI struct quaternion quaternion_cross_product_EXP(const struct quaternion *self, const struct quaternion *vT);
HYPAPI struct matrix4 *matrix4_set_from_quaternion(struct matrix4 *self, const struct quaternion *qT);
HYPAPI struct matrix4 *matrix4_set_from_axisv3_angle(struct matrix4 *self, const struct vector3 *axis, HYP_FLOAT angle);
HYPAPI struct matrix4 *matrix4_set_from_axisf3_angle(struct matrix4 *self, HYP_FLOAT x, HYP_FLOAT y, HYP_FLOAT z, const HYP_FLOAT angle);
HYPAPI struct matrix4 *matrix4_set_from_euler_anglesf3(struct matrix4 *self, const HYP_FLOAT x, const HYP_FLOAT y, const HYP_FLOAT z);
HYPAPI struct vector3 *matrix4_get_translation(const struct matrix4 *self, struct vector3 *vT);
HYPAPI struct matrix4 *matrix4_make_transformation_rotationv3(struct matrix4 *self, const struct vector3 *vR);
HYPAPI struct matrix4 *matrix4_transformation_compose(struct matrix4 *self, const struct vector3 *scale, const struct quaternion *rotation, const struct vector3 *translation);
HYPAPI uint8_t matrix4_transformation_decompose(struct matrix4 *self, struct vector3 *scale, struct quaternion *rotation, struct vector3 *translation);

#endif /* HYPATIA_H_ */


#if !defined(HYP_NO_DEPRECATED) && !defined(DOXYGEN_SHOULD_SKIP_THIS)
#define matrix4_get_translation_EXP matrix4_get_translation
#define matrix4_make_transformation_rotationv3_EXP matrix4_make_transformation_rotationv3
#define matrix4_projection_ortho3d_rh_EXP matrix4_projection_ortho3d_rh
#define matrix4_projection_perspective_fovy_rh_EXP matrix4_projection_perspective_fovy_rh
#define matrix4_set_from_axisf3_angle_EXP matrix4_set_from_axisf3_angle
#define matrix4_set_from_axisv3_angle_EXP matrix4_set_from_axisv3_angle
#define matrix4_set_from_euler_anglesf3_EXP matrix4_set_from_euler_anglesf3
#define matrix4_transformation_compose_EXP matrix4_transformation_compose
#define matrix4_transformation_decompose_EXP matrix4_transformation_decompose
#define matrix4_view_lookat_rh_EXP matrix4_view_lookat_rh
#define quaternion_angle_between_EXP quaternion_angle_between
#define quaternion_difference_EXP quaternion_difference
#define quaternion_rotate_by_axis_angle_EXP quaternion_rotate_by_axis_angle
#define quaternion_rotate_by_euler_angles_EXP quaternion_rotate_by_euler_angles
#define quaternion_rotate_by_quaternion_EXP quaternion_rotate_by_quaternion
#define matrix4_multiplyv3_EXP matrix4_multiplyv3
#endif

#ifdef HYPATIA_IMPLEMENTATION
#ifndef HYPATIA_IMPLEMENTATION_H_
#define HYPATIA_IMPLEMENTATION_H_


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

/**
 * @brief Returns a random number in the range [min, max), evenly distributed.
 * The randomness comes from HYP_RANDOM.  Returns min when min is not less
 * than max or when max - min overflows.
 *
 */
HYPAPI HYP_FLOAT scalar_random_rangef(HYP_FLOAT min, HYP_FLOAT max)
{
	HYP_FLOAT unit;
	HYP_FLOAT value;

	/* an infinite max - min times 0 is NaN */
	if (!(min < max) || !((max - min) * HYP_FLOAT_C(0.0) <= HYP_FLOAT_C(0.0))) {
		return min;
	}

	/* the integer to HYP_FLOAT conversions need casts in single precision.
	 * There, unit can round up to exactly 1 and value up to exactly max, so
	 * draw again until both are below.
	 */
	do {
		unit = (HYP_FLOAT)HYP_RANDOM() / ((HYP_FLOAT)HYP_RANDOM_MAX + HYP_FLOAT_C(1.0));
		value = min + (max - min) * unit;
	} while (!(unit < HYP_FLOAT_C(1.0)) || !(value < max));

	return value;
}


#ifndef HYP_NO_STDIO
/* prints prefix and then value.  value is a double because printf takes
 * floating point arguments as double; the conversion happens here, in one
 * place, together with the number format.
 */
static void hyp_print_value(const char *prefix, double value)
{
	printf("%s%10f", prefix, value);
}
#endif


/* divides the n components by their length and returns the length; returns 0
 * and leaves them unchanged when they are all zero or one is NaN.  Dividing by
 * the largest component first keeps the squares in range.  When a component is
 * infinite, the infinite components become +-1 and the others 0 before the
 * division, and the length is infinite.
 */
static HYP_FLOAT hyp_normalize(HYP_FLOAT *v, uint8_t n)
{
	HYP_FLOAT largest = HYP_FLOAT_C(0.0);
	HYP_FLOAT sum = HYP_FLOAT_C(0.0);
	HYP_FLOAT length;
	uint8_t i;

	/* the usual case: the sum of the squares is in range, and dividing by
	 * the length rounds each component once
	 */
	sum = v[0] * v[0] + v[1] * v[1];
	if (n == 3) {
		sum += v[2] * v[2];
	} else if (n == 4) {
		sum += v[2] * v[2] + v[3] * v[3];
	}
	if (sum > HYP_FLOAT_C(1e-30) && sum < HYP_FLOAT_C(1e30)) {
		length = HYP_SQRT(sum);
		for (i = 0; i < n; i++) {
			v[i] /= length;
		}
		return length;
	}
	sum = HYP_FLOAT_C(0.0);

	for (i = 0; i < n; i++) {
		if (HYP_ABS(v[i]) > largest) {
			largest = HYP_ABS(v[i]);
		} else if (!(HYP_ABS(v[i]) <= largest)) {
			return HYP_FLOAT_C(0.0); /* NaN */
		}
	}

	if (!(largest > HYP_FLOAT_C(0.0))) {
		return HYP_FLOAT_C(0.0);
	}

	for (i = 0; i < n; i++) {
		if (!(largest - largest <= HYP_FLOAT_C(0.0))) {
			/* largest is infinite */
			v[i] = (HYP_ABS(v[i]) < largest) ? HYP_FLOAT_C(0.0) : (v[i] > HYP_FLOAT_C(0.0)) ? HYP_FLOAT_C(1.0) : -HYP_FLOAT_C(1.0);
		} else {
			v[i] /= largest;
		}
		sum += v[i] * v[i];
	}

	length = HYP_SQRT(sum);

	for (i = 0; i < n; i++) {
		v[i] /= length;
	}

	return largest * length;
}


/* the length of the n (at most 4) components without overflow or underflow;
 * 0 when they are all zero or one is NaN
 */
static HYP_FLOAT hyp_length(const HYP_FLOAT *v, uint8_t n)
{
	HYP_FLOAT copy[4];
	uint8_t i;

	for (i = 0; i < n; i++) {
		copy[i] = v[i];
	}

	return hyp_normalize(copy, n);
}


static struct vector2 hyp_vector2_zero = { { {HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0)} } };
static struct vector2 hyp_vector2_one = { { {HYP_FLOAT_C(1.0), HYP_FLOAT_C(1.0)} } };
static struct vector2 hyp_vector2_unit_x = { { {HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0)} } };
static struct vector2 hyp_vector2_unit_y = { { {HYP_FLOAT_C(0.0), HYP_FLOAT_C(1.0)} } };
static struct vector2 hyp_vector2_unit_x_negative = { { {-HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0)} } };
static struct vector2 hyp_vector2_unit_y_negative = { { {HYP_FLOAT_C(0.0), -HYP_FLOAT_C(1.0)} } };


HYPAPI const struct vector2 *vector2_get_reference_vector2(int id)
{
	switch (id) {
	case HYP_REF_VECTOR2_ZERO:
		return &hyp_vector2_zero;
	case HYP_REF_VECTOR2_ONE:
		return &hyp_vector2_one;
	case HYP_REF_VECTOR2_UNIT_X:
		return &hyp_vector2_unit_x;
	case HYP_REF_VECTOR2_UNIT_Y:
		return &hyp_vector2_unit_y;
	case HYP_REF_VECTOR2_UNIT_X_NEGATIVE:
		return &hyp_vector2_unit_x_negative;
	case HYP_REF_VECTOR2_UNIT_Y_NEGATIVE:
		return &hyp_vector2_unit_y_negative;
	default:
		/* undefined case */
		return &hyp_vector2_zero;
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


/**
 * @ingroup vector2
 * @brief Sets the vector to a random unit vector, evenly distributed over the
 * circle.  Uses one draw from scalar_random_rangef: the angle.
 */
HYPAPI struct vector2 *vector2_set_random_unit(struct vector2 *self)
{
	HYP_FLOAT angle;

	angle = scalar_random_rangef(HYP_FLOAT_C(0.0), HYP_TAU);

	return vector2_setf2(self, HYP_COS(angle), HYP_SIN(angle));
}


/**
 * @ingroup vector2
 * @brief Sets the vector to a random point inside the unit circle, evenly
 * distributed over the disk.  Uses two draws from scalar_random_rangef, in this
 * order: u (the radius is sqrt(u)), then the angle.
 */
HYPAPI struct vector2 *vector2_set_random_in_disk(struct vector2 *self)
{
	HYP_FLOAT radius;
	HYP_FLOAT angle;

	/* the area inside radius r grows as r^2, so r = sqrt(u) spreads evenly */
	radius = HYP_SQRT(scalar_random_rangef(HYP_FLOAT_C(0.0), HYP_FLOAT_C(1.0)));
	angle = scalar_random_rangef(HYP_FLOAT_C(0.0), HYP_TAU);

	return vector2_setf2(self, radius * HYP_COS(angle), radius * HYP_SIN(angle));
}


HYPAPI struct vector2 *vector2_zero(struct vector2 *self)
{
	return vector2_setf2(self, HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0));
}


HYPAPI int vector2_equals(const struct vector2 *self, const struct vector2 *vT)
{
	return scalar_equals(self->x, vT->x) && scalar_equals(self->y, vT->y);
}


/**
 * @ingroup vector2
 * @brief Linear interpolation: start at percent 0, end at percent 1
 *
 * @param start the vector at percent 0
 * @param end the vector at percent 1
 * @param percent how far from start to end
 * @param vR the result
 */
HYPAPI struct vector2 *vector2_lerp(const struct vector2 *start, const struct vector2 *end, HYP_FLOAT percent, struct vector2 *vR)
{
	/* written so that percent 0 and 1 give start and end exactly */
	vR->v[0] = start->v[0] * (HYP_FLOAT_C(1.0) - percent) + end->v[0] * percent;
	vR->v[1] = start->v[1] * (HYP_FLOAT_C(1.0) - percent) + end->v[1] * percent;
	return vR;
}


/**
 * @ingroup vector2
 * @brief Clamps each component between the components of vMin and vMax
 */
HYPAPI struct vector2 *vector2_clamp(struct vector2 *self, const struct vector2 *vMin, const struct vector2 *vMax)
{
	self->v[0] = HYP_CLAMP(self->v[0], vMin->v[0], vMax->v[0]);
	self->v[1] = HYP_CLAMP(self->v[1], vMin->v[1], vMax->v[1]);
	return self;
}


/**
 * @ingroup vector2
 * @brief Sets each component to the smaller of self and vT
 */
HYPAPI struct vector2 *vector2_min(struct vector2 *self, const struct vector2 *vT)
{
	self->v[0] = HYP_MIN(self->v[0], vT->v[0]);
	self->v[1] = HYP_MIN(self->v[1], vT->v[1]);
	return self;
}


/**
 * @ingroup vector2
 * @brief Sets each component to the larger of self and vT
 */
HYPAPI struct vector2 *vector2_max(struct vector2 *self, const struct vector2 *vT)
{
	self->v[0] = HYP_MAX(self->v[0], vT->v[0]);
	self->v[1] = HYP_MAX(self->v[1], vT->v[1]);
	return self;
}


/**
 * @ingroup vector2
 * @brief Projects self onto the direction of onto: the part of self parallel
 * to onto.  Projecting onto the zero vector gives the zero vector.
 */
HYPAPI struct vector2 *vector2_project(struct vector2 *self, const struct vector2 *onto)
{
	struct vector2 direction;
	HYP_FLOAT norm = vector2_dot_product(onto, onto);
	HYP_FLOAT length;

	/* (self . onto) / |onto|^2 times onto, or through the unit vector when
	 * |onto|^2 overflows or underflows
	 */
	if (norm > HYP_FLOAT_C(1e-30) && norm < HYP_FLOAT_C(1e30)) {
		length = vector2_dot_product(self, onto) / norm;
		return vector2_multiplyf(vector2_set(self, onto), length);
	}

	if (!(hyp_normalize(vector2_set(&direction, onto)->v, 2) > HYP_FLOAT_C(0.0))) {
		return vector2_zero(self);
	}

	length = vector2_dot_product(self, &direction);

	return vector2_multiplyf(vector2_set(self, &direction), length);
}


/**
 * @ingroup vector2
 * @brief Reflects self off a surface with the given normal: self - 2 (self . n) n
 * for the unit normal n.  The normal does not need to be unit length; a zero
 * normal leaves self unchanged.
 */
HYPAPI struct vector2 *vector2_reflect(struct vector2 *self, const struct vector2 *normal)
{
	struct vector2 direction;
	HYP_FLOAT norm = vector2_dot_product(normal, normal);
	HYP_FLOAT s;

	/* 2 (self . normal) / |normal|^2 times normal, or through the unit normal
	 * when |normal|^2 overflows or underflows
	 */
	if (norm > HYP_FLOAT_C(1e-30) && norm < HYP_FLOAT_C(1e30)) {
		s = HYP_FLOAT_C(2.0) * vector2_dot_product(self, normal) / norm;
		vector2_set(&direction, normal);
	} else if (hyp_normalize(vector2_set(&direction, normal)->v, 2) > HYP_FLOAT_C(0.0)) {
		s = HYP_FLOAT_C(2.0) * vector2_dot_product(self, &direction);
	} else {
		return self;
	}

	return vector2_subtract(self, vector2_multiplyf(&direction, s));
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


/**
 * @ingroup vector2
 * @brief normalizes the vector; the zero vector is left unchanged
 */
HYPAPI struct vector2 *vector2_normalize(struct vector2 *self)
{
	hyp_normalize(self->v, 2);
	return self;
}


HYPAPI HYP_FLOAT vector2_dot_product(const struct vector2 *self, const struct vector2 *vT)
{
	return (self->x * vT->x) + (self->y * vT->y);
}


HYPAPI HYP_FLOAT vector2_cross_product(const struct vector2 *vT1, const struct vector2 *vT2)
{
	return (vT1->x * vT2->y) - (vT1->y * vT2->x);
}


/**
 * @ingroup vector2
 * @brief finds the angle between two vectors, in radians from 0 to pi.  The
 * vectors do not need to be unit length; the angle is 0 when either is zero.
 */
HYPAPI HYP_FLOAT vector2_angle_between(const struct vector2 *self, const struct vector2 *vT)
{
	struct vector2 a;
	struct vector2 b;

	/* as vector3_angle_between: atan2(|a x b|, a . b) of the unit vectors */
	hyp_normalize(vector2_set(&a, self)->v, 2);
	hyp_normalize(vector2_set(&b, vT)->v, 2);

	return HYP_ATAN2(HYP_ABS(vector2_cross_product(&a, &b)), vector2_dot_product(&a, &b));
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
HYPAPI struct vector2 *vector2_multiplym2(struct vector2 *self, const struct matrix2 *mT)
{
	struct vector2 vR;

	vector2_zero(&vR);

	matrix2_multiplyv2(mT, self, &vR);

	vector2_set(self, &vR);

	return self;
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
HYPAPI void hyp_vector2_print(const struct vector2 *self)
{
	hyp_print_value("x:", self->x);
	hyp_print_value(", y:", self->y);
	printf("\r\n");
}
#endif


static struct vector3 hyp_vector3_zero = { { {HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0)} } };
static struct vector3 hyp_vector3_one = { { {HYP_FLOAT_C(1.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(1.0)} } };
static struct vector3 hyp_vector3_unit_x = { { {HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0)} } };
static struct vector3 hyp_vector3_unit_y = { { {HYP_FLOAT_C(0.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0)} } };
static struct vector3 hyp_vector3_unit_z = { { {HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(1.0)} } };
static struct vector3 hyp_vector3_unit_x_negative = { { {-HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0)} } };
static struct vector3 hyp_vector3_unit_y_negative = { { {HYP_FLOAT_C(0.0), -HYP_FLOAT_C(1.0),  HYP_FLOAT_C(0.0)} } };
static struct vector3 hyp_vector3_unit_z_negative = { { {HYP_FLOAT_C(0.0),  HYP_FLOAT_C(0.0), -HYP_FLOAT_C(1.0)} } };


HYPAPI const struct vector3 *vector3_get_reference_vector3(int id)
{
	switch (id) {
	case HYP_REF_VECTOR3_ZERO:
		return &hyp_vector3_zero;
	case HYP_REF_VECTOR3_ONE:
		return &hyp_vector3_one;
	case HYP_REF_VECTOR3_UNIT_X:
		return &hyp_vector3_unit_x;
	case HYP_REF_VECTOR3_UNIT_Y:
		return &hyp_vector3_unit_y;
	case HYP_REF_VECTOR3_UNIT_Z:
		return &hyp_vector3_unit_z;
	case HYP_REF_VECTOR3_UNIT_X_NEGATIVE:
		return &hyp_vector3_unit_x_negative;
	case HYP_REF_VECTOR3_UNIT_Y_NEGATIVE:
		return &hyp_vector3_unit_y_negative;
	case HYP_REF_VECTOR3_UNIT_Z_NEGATIVE:
		return &hyp_vector3_unit_z_negative;
	default:
		/* undefined case */
		return &hyp_vector3_zero;
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
 * @brief Sets the vector to a random unit vector, evenly distributed over the
 * sphere.  Uses two draws from scalar_random_rangef, in this order: z, then
 * the angle around the z axis.
 */
HYPAPI struct vector3 *vector3_set_random_unit(struct vector3 *self)
{
	HYP_FLOAT z;
	HYP_FLOAT angle;
	HYP_FLOAT r;

	/* the height on a sphere is evenly distributed (Archimedes) */
	z = scalar_random_rangef(HYP_FLOAT_C(-1.0), HYP_FLOAT_C(1.0));
	angle = scalar_random_rangef(HYP_FLOAT_C(0.0), HYP_TAU);
	r = HYP_SQRT(HYP_FLOAT_C(1.0) - z * z);

	return vector3_setf3(self, r * HYP_COS(angle), r * HYP_SIN(angle), z);
}


/**
 * @ingroup vector3
 * @brief Sets the vector to a random point inside the unit sphere, evenly
 * distributed over the ball.  Uses five draws from scalar_random_rangef, in
 * this order: the direction (as vector3_set_random_unit), then three for the
 * radius.
 */
HYPAPI struct vector3 *vector3_set_random_in_ball(struct vector3 *self)
{
	HYP_FLOAT radius;

	vector3_set_random_unit(self);

	/* the volume inside radius r grows as r^3, and the largest of three even
	 * draws is below r with probability r^3
	 */
	radius = scalar_random_rangef(HYP_FLOAT_C(0.0), HYP_FLOAT_C(1.0));
	radius = HYP_MAX(radius, scalar_random_rangef(HYP_FLOAT_C(0.0), HYP_FLOAT_C(1.0)));
	radius = HYP_MAX(radius, scalar_random_rangef(HYP_FLOAT_C(0.0), HYP_FLOAT_C(1.0)));

	return vector3_multiplyf(self, radius);
}


/**
 * @ingroup vector3
 * @brief Sets the vector to a random unit vector within angle (radians, 0 to
 * pi) of axis, evenly distributed over that cap of the sphere.  axis does not need to
 * be unit length; a zero axis means +Z.  Uses two draws from
 * scalar_random_rangef, in this order: the height along the axis, then the
 * angle around it.
 */
HYPAPI struct vector3 *vector3_set_random_in_cone(struct vector3 *self, const struct vector3 *axis, HYP_FLOAT angle)
{
	struct quaternion to_axis;
	HYP_FLOAT z;
	HYP_FLOAT around;
	HYP_FLOAT r;

	/* the height on a sphere is evenly distributed (Archimedes), so an even
	 * height in [cos(angle), 1] spreads evenly over the cap about +Z
	 */
	z = scalar_random_rangef(HYP_COS(HYP_CLAMP(angle, HYP_FLOAT_C(0.0), HYP_PI)), HYP_FLOAT_C(1.0));
	around = scalar_random_rangef(HYP_FLOAT_C(0.0), HYP_TAU);
	r = HYP_SQRT(HYP_FLOAT_C(1.0) - z * z);
	vector3_setf3(self, r * HYP_COS(around), r * HYP_SIN(around), z);

	/* then turn +Z onto the axis */
	quaternion_get_rotation_tov3(HYP_VECTOR3_UNIT_Z, axis, &to_axis);

	return vector3_rotate_by_quaternion(self, &to_axis);
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
	return vector3_setf3(self, HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0));
}


/**
 * @ingroup vector3
 * @brief compares two vectors.  Uses epsilon to deal with rounding errors
 */
HYPAPI int vector3_equals(const struct vector3 *self, const struct vector3 *vT)
{
	return scalar_equalsf(self->x, vT->x) &&
		scalar_equalsf(self->y, vT->y) &&
		scalar_equalsf(self->z, vT->z);
}


/**
 * @ingroup vector3
 * @brief Linear interpolation: start at percent 0, end at percent 1
 *
 * @param start the vector at percent 0
 * @param end the vector at percent 1
 * @param percent how far from start to end
 * @param vR the result
 */
HYPAPI struct vector3 *vector3_lerp(const struct vector3 *start, const struct vector3 *end, HYP_FLOAT percent, struct vector3 *vR)
{
	/* written so that percent 0 and 1 give start and end exactly */
	vR->v[0] = start->v[0] * (HYP_FLOAT_C(1.0) - percent) + end->v[0] * percent;
	vR->v[1] = start->v[1] * (HYP_FLOAT_C(1.0) - percent) + end->v[1] * percent;
	vR->v[2] = start->v[2] * (HYP_FLOAT_C(1.0) - percent) + end->v[2] * percent;
	return vR;
}


/**
 * @ingroup vector3
 * @brief Clamps each component between the components of vMin and vMax
 */
HYPAPI struct vector3 *vector3_clamp(struct vector3 *self, const struct vector3 *vMin, const struct vector3 *vMax)
{
	self->v[0] = HYP_CLAMP(self->v[0], vMin->v[0], vMax->v[0]);
	self->v[1] = HYP_CLAMP(self->v[1], vMin->v[1], vMax->v[1]);
	self->v[2] = HYP_CLAMP(self->v[2], vMin->v[2], vMax->v[2]);
	return self;
}


/**
 * @ingroup vector3
 * @brief Sets each component to the smaller of self and vT
 */
HYPAPI struct vector3 *vector3_min(struct vector3 *self, const struct vector3 *vT)
{
	self->v[0] = HYP_MIN(self->v[0], vT->v[0]);
	self->v[1] = HYP_MIN(self->v[1], vT->v[1]);
	self->v[2] = HYP_MIN(self->v[2], vT->v[2]);
	return self;
}


/**
 * @ingroup vector3
 * @brief Sets each component to the larger of self and vT
 */
HYPAPI struct vector3 *vector3_max(struct vector3 *self, const struct vector3 *vT)
{
	self->v[0] = HYP_MAX(self->v[0], vT->v[0]);
	self->v[1] = HYP_MAX(self->v[1], vT->v[1]);
	self->v[2] = HYP_MAX(self->v[2], vT->v[2]);
	return self;
}


/**
 * @ingroup vector3
 * @brief Projects self onto the direction of onto: the part of self parallel
 * to onto.  Projecting onto the zero vector gives the zero vector.
 */
HYPAPI struct vector3 *vector3_project(struct vector3 *self, const struct vector3 *onto)
{
	struct vector3 direction;
	HYP_FLOAT norm = vector3_dot_product(onto, onto);
	HYP_FLOAT length;

	/* (self . onto) / |onto|^2 times onto, or through the unit vector when
	 * |onto|^2 overflows or underflows
	 */
	if (norm > HYP_FLOAT_C(1e-30) && norm < HYP_FLOAT_C(1e30)) {
		length = vector3_dot_product(self, onto) / norm;
		return vector3_multiplyf(vector3_set(self, onto), length);
	}

	if (!(hyp_normalize(vector3_set(&direction, onto)->v, 3) > HYP_FLOAT_C(0.0))) {
		return vector3_zero(self);
	}

	length = vector3_dot_product(self, &direction);

	return vector3_multiplyf(vector3_set(self, &direction), length);
}


/**
 * @ingroup vector3
 * @brief Reflects self off a surface with the given normal: self - 2 (self . n) n
 * for the unit normal n.  The normal does not need to be unit length; a zero
 * normal leaves self unchanged.
 */
HYPAPI struct vector3 *vector3_reflect(struct vector3 *self, const struct vector3 *normal)
{
	struct vector3 direction;
	HYP_FLOAT norm = vector3_dot_product(normal, normal);
	HYP_FLOAT s;

	/* 2 (self . normal) / |normal|^2 times normal, or through the unit normal
	 * when |normal|^2 overflows or underflows
	 */
	if (norm > HYP_FLOAT_C(1e-30) && norm < HYP_FLOAT_C(1e30)) {
		s = HYP_FLOAT_C(2.0) * vector3_dot_product(self, normal) / norm;
		vector3_set(&direction, normal);
	} else if (hyp_normalize(vector3_set(&direction, normal)->v, 3) > HYP_FLOAT_C(0.0)) {
		s = HYP_FLOAT_C(2.0) * vector3_dot_product(self, &direction);
	} else {
		return self;
	}

	return vector3_subtract(self, vector3_multiplyf(&direction, s));
}


/**
 * @ingroup vector3
 * @brief Refracts the direction self through a surface with the given normal,
 * by Snell's law.  eta is the ratio of the refractive indices, from the side
 * self comes from to the other (1 / 1.5 from air into glass).  The normal faces
 * the side self comes from (normal . self < 0).
 *
 * Neither vector needs to be unit length.  The result is the unit direction of
 * the refracted ray, or the zero vector for total internal reflection or a zero
 * self.  A zero normal leaves the direction unchanged.
 */
HYPAPI struct vector3 *vector3_refract(struct vector3 *self, const struct vector3 *normal, HYP_FLOAT eta)
{
	struct vector3 incident;
	struct vector3 n;
	HYP_FLOAT d;
	HYP_FLOAT k;

	if (!(hyp_normalize(vector3_set(&incident, self)->v, 3) > HYP_FLOAT_C(0.0))) {
		return vector3_zero(self);
	}
	if (!(hyp_normalize(vector3_set(&n, normal)->v, 3) > HYP_FLOAT_C(0.0))) {
		return vector3_set(self, &incident);
	}

	/* the cosine of the angle of incidence is -d; k is the square of the
	 * cosine of the angle of refraction
	 */
	d = vector3_dot_product(&n, &incident);
	k = HYP_FLOAT_C(1.0) - eta * eta * (HYP_FLOAT_C(1.0) - d * d);
	if (k < HYP_FLOAT_C(0.0)) {
		return vector3_zero(self);
	}

	/* eta incident - (eta d + sqrt(k)) n */
	vector3_multiplyf(vector3_set(self, &incident), eta);
	return vector3_subtract(self, vector3_multiplyf(&n, eta * d + HYP_SQRT(k)));
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
 * @brief divides each component of the vector by a scalar
 */
HYPAPI struct vector3 *vector3_dividef(struct vector3 *self, HYP_FLOAT fT)
{
	self->v[0] /= fT;
	self->v[1] /= fT;
	self->v[2] /= fT;
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
 * @brief normalizes the vector; the zero vector is left unchanged
 */
HYPAPI struct vector3 *vector3_normalize(struct vector3 *self)
{
	hyp_normalize(self->v, 3);
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
 * @brief finds the angle between two vectors, in radians from 0 to pi.  The
 * vectors do not need to be unit length; the angle is 0 when either is zero.
 *
 */
HYPAPI HYP_FLOAT vector3_angle_between(const struct vector3 *vT1, const struct vector3 *vT2)
{
	struct vector3 a;
	struct vector3 b;
	struct vector3 cross;

	/* atan2(|a x b|, a . b) of the unit vectors stays accurate for parallel
	 * and nearly parallel vectors, where acos of the cosine does not (and
	 * gives NaN when rounding puts the cosine above 1)
	 */
	hyp_normalize(vector3_set(&a, vT1)->v, 3);
	hyp_normalize(vector3_set(&b, vT2)->v, 3);
	vector3_cross_product(&cross, &a, &b);

	return HYP_ATAN2(vector3_magnitude(&cross), vector3_dot_product(&a, &b));
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
HYPAPI void hyp_vector3_print(const struct vector3 *self)
{
	hyp_print_value("x:", self->x);
	hyp_print_value(", y:", self->y);
	hyp_print_value(", z:", self->z);
	printf("\r\n");
}
#endif


/**
 * @ingroup vector3
 * @brief Rotate a point by the quaternion.  Returns the rotated point.
 *
 * \f$self= qT * self * qT^{-1}\f$
 *
 * qT does not need to be unit length: the rotation is that of the unit
 * quaternion in its direction, as matrix4_set_from_quaternion.  The zero
 * quaternion leaves the point unchanged.
 *
 * @param self the starting point
 * @param qT the quaternion
 *
 */
HYPAPI struct vector3 *vector3_rotate_by_quaternion(struct vector3 *self, const struct quaternion *qT)
{
	struct quaternion unit;
	struct vector3 u;
	struct vector3 uv;
	HYP_FLOAT norm = quaternion_norm(qT);
	HYP_FLOAT parallel;
	HYP_FLOAT along;
	HYP_FLOAT across;

	/* (2 (u . v) u + (w^2 - u . u) v + 2 w (u x v)) / |q|^2, with u the vector
	 * part: the rotation by q / |q|, which q * v * conjugate(q) also scales by
	 * |q|^2.  Outside the normal range of |q|^2 (zero, overflow, underflow,
	 * NaN) normalize q first.
	 */
	quaternion_set(&unit, qT);
	if (!(norm > HYP_FLOAT_C(1e-30)) || !(norm < HYP_FLOAT_C(1e30))) {
		if (!(hyp_normalize(unit.q, 4) > HYP_FLOAT_C(0.0))) {
			return self;
		}
		norm = HYP_FLOAT_C(1.0);
	}

	vector3_setf3(&u, unit.x, unit.y, unit.z);
	vector3_cross_product(&uv, &u, self);
	parallel = HYP_FLOAT_C(2.0) * vector3_dot_product(&u, self);
	along = unit.w * unit.w - vector3_dot_product(&u, &u);
	across = HYP_FLOAT_C(2.0) * unit.w;

	self->x = (parallel * u.x + along * self->x + across * uv.x) / norm;
	self->y = (parallel * u.y + along * self->y + across * uv.y) / norm;
	self->z = (parallel * u.z + along * self->z + across * uv.z) / norm;

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

	self->x = q.x;
	self->y = q.y;
	self->z = q.z;

	return self;
}


static struct vector4 hyp_vector4_zero = { { {HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0)} } };
static struct vector4 hyp_vector4_one = { { {HYP_FLOAT_C(1.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(1.0)} } };
static struct vector4 hyp_vector4_unit_x = { { {HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0)} } };
static struct vector4 hyp_vector4_unit_y = { { {HYP_FLOAT_C(0.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0)} } };
static struct vector4 hyp_vector4_unit_z = { { {HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0)} } };
static struct vector4 hyp_vector4_unit_x_negative = { { {-HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0)} } };
static struct vector4 hyp_vector4_unit_y_negative = { { {HYP_FLOAT_C(0.0), -HYP_FLOAT_C(1.0),  HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0)} } };
static struct vector4 hyp_vector4_unit_z_negative = { { {HYP_FLOAT_C(0.0),  HYP_FLOAT_C(0.0), -HYP_FLOAT_C(1.0), HYP_FLOAT_C(0.0)} } };


HYPAPI const struct vector4 *vector4_get_reference_vector4(int id)
{
	switch (id) {
	case HYP_REF_VECTOR4_ZERO:
		return &hyp_vector4_zero;
	case HYP_REF_VECTOR4_ONE:
		return &hyp_vector4_one;
	case HYP_REF_VECTOR4_UNIT_X:
		return &hyp_vector4_unit_x;
	case HYP_REF_VECTOR4_UNIT_Y:
		return &hyp_vector4_unit_y;
	case HYP_REF_VECTOR4_UNIT_Z:
		return &hyp_vector4_unit_z;
	case HYP_REF_VECTOR4_UNIT_X_NEGATIVE:
		return &hyp_vector4_unit_x_negative;
	case HYP_REF_VECTOR4_UNIT_Y_NEGATIVE:
		return &hyp_vector4_unit_y_negative;
	case HYP_REF_VECTOR4_UNIT_Z_NEGATIVE:
		return &hyp_vector4_unit_z_negative;
	default:
		/* undefined case */
		return &hyp_vector4_zero;
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


/* fills a, b, c, d with a random point evenly distributed over the 4D unit
 * sphere (Shoemake).  Draws in this order: u, then angle a, then angle b.
 */
static void hyp_set_random_unit4(HYP_FLOAT *a, HYP_FLOAT *b, HYP_FLOAT *c, HYP_FLOAT *d)
{
	HYP_FLOAT u;
	HYP_FLOAT angle_a;
	HYP_FLOAT angle_b;
	HYP_FLOAT s1;
	HYP_FLOAT s2;

	u = scalar_random_rangef(HYP_FLOAT_C(0.0), HYP_FLOAT_C(1.0));
	angle_a = scalar_random_rangef(HYP_FLOAT_C(0.0), HYP_TAU);
	angle_b = scalar_random_rangef(HYP_FLOAT_C(0.0), HYP_TAU);
	s1 = HYP_SQRT(HYP_FLOAT_C(1.0) - u);
	s2 = HYP_SQRT(u);

	*a = s1 * HYP_SIN(angle_a);
	*b = s1 * HYP_COS(angle_a);
	*c = s2 * HYP_SIN(angle_b);
	*d = s2 * HYP_COS(angle_b);
}


/**
 * @ingroup vector4
 * @brief Sets the vector to a random unit vector, evenly distributed over the
 * 4D unit sphere.  Uses three draws from scalar_random_rangef, in this order:
 * u, then two angles.
 */
HYPAPI struct vector4 *vector4_set_random_unit(struct vector4 *self)
{
	hyp_set_random_unit4(&self->x, &self->y, &self->z, &self->w);
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
	self->w = vT->w;
	return self;
}


/**
 * @ingroup vector4
 * @brief initializes the vertex with zeros
 */
HYPAPI struct vector4 *vector4_zero(struct vector4 *self)
{
	return vector4_setf4(self, HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0), HYP_FLOAT_C(0.0));
}


/**
 * @ingroup vector4
 * @brief compares two vectors.  Uses epsilon to deal with rounding errors
 */
HYPAPI int vector4_equals(const struct vector4 *self, const struct vector4 *vT)
{
	return scalar_equalsf(self->x, vT->x) &&
		scalar_equalsf(self->y, vT->y) &&
		scalar_equalsf(self->z, vT->z) &&
		scalar_equalsf(self->w, vT->w);
}


/**
 * @ingroup vector4
 * @brief Linear interpolation: start at percent 0, end at percent 1
 *
 * @param start the vector at percent 0
 * @param end the vector at percent 1
 * @param percent how far from start to end
 * @param vR the result
 */
HYPAPI struct vector4 *vector4_lerp(const struct vector4 *start, const struct vector4 *end, HYP_FLOAT percent, struct vector4 *vR)
{
	/* written so that percent 0 and 1 give start and end exactly */
	vR->v[0] = start->v[0] * (HYP_FLOAT_C(1.0) - percent) + end->v[0] * percent;
	vR->v[1] = start->v[1] * (HYP_FLOAT_C(1.0) - percent) + end->v[1] * percent;
	vR->v[2] = start->v[2] * (HYP_FLOAT_C(1.0) - percent) + end->v[2] * percent;
	vR->v[3] = start->v[3] * (HYP_FLOAT_C(1.0) - percent) + end->v[3] * percent;
	return vR;
}


/**
 * @ingroup vector4
 * @brief Clamps each component between the components of vMin and vMax
 */
HYPAPI struct vector4 *vector4_clamp(struct vector4 *self, const struct vector4 *vMin, const struct vector4 *vMax)
{
	self->v[0] = HYP_CLAMP(self->v[0], vMin->v[0], vMax->v[0]);
	self->v[1] = HYP_CLAMP(self->v[1], vMin->v[1], vMax->v[1]);
	self->v[2] = HYP_CLAMP(self->v[2], vMin->v[2], vMax->v[2]);
	self->v[3] = HYP_CLAMP(self->v[3], vMin->v[3], vMax->v[3]);
	return self;
}


/**
 * @ingroup vector4
 * @brief Sets each component to the smaller of self and vT
 */
HYPAPI struct vector4 *vector4_min(struct vector4 *self, const struct vector4 *vT)
{
	self->v[0] = HYP_MIN(self->v[0], vT->v[0]);
	self->v[1] = HYP_MIN(self->v[1], vT->v[1]);
	self->v[2] = HYP_MIN(self->v[2], vT->v[2]);
	self->v[3] = HYP_MIN(self->v[3], vT->v[3]);
	return self;
}


/**
 * @ingroup vector4
 * @brief Sets each component to the larger of self and vT
 */
HYPAPI struct vector4 *vector4_max(struct vector4 *self, const struct vector4 *vT)
{
	self->v[0] = HYP_MAX(self->v[0], vT->v[0]);
	self->v[1] = HYP_MAX(self->v[1], vT->v[1]);
	self->v[2] = HYP_MAX(self->v[2], vT->v[2]);
	self->v[3] = HYP_MAX(self->v[3], vT->v[3]);
	return self;
}


/**
 * @ingroup vector4
 * @brief Projects self onto the direction of onto: the part of self parallel
 * to onto.  Projecting onto the zero vector gives the zero vector.
 */
HYPAPI struct vector4 *vector4_project(struct vector4 *self, const struct vector4 *onto)
{
	struct vector4 direction;
	HYP_FLOAT norm = vector4_dot_product(onto, onto);
	HYP_FLOAT length;

	/* (self . onto) / |onto|^2 times onto, or through the unit vector when
	 * |onto|^2 overflows or underflows
	 */
	if (norm > HYP_FLOAT_C(1e-30) && norm < HYP_FLOAT_C(1e30)) {
		length = vector4_dot_product(self, onto) / norm;
		return vector4_multiplyf(vector4_set(self, onto), length);
	}

	if (!(hyp_normalize(vector4_set(&direction, onto)->v, 4) > HYP_FLOAT_C(0.0))) {
		return vector4_zero(self);
	}

	length = vector4_dot_product(self, &direction);

	return vector4_multiplyf(vector4_set(self, &direction), length);
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
 * @brief divides each component of the vector by a scalar
 */
HYPAPI struct vector4 *vector4_dividef(struct vector4 *self, HYP_FLOAT fT)
{
	self->v[0] /= fT;
	self->v[1] /= fT;
	self->v[2] /= fT;
	self->v[3] /= fT;
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
 * @brief normalizes the vector; the zero vector is left unchanged
 */
HYPAPI struct vector4 *vector4_normalize(struct vector4 *self)
{
	hyp_normalize(self->v, 4);
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
	vR->w = 0;
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
HYPAPI void hyp_vector4_print(const struct vector4 *self)
{
	hyp_print_value("x:", self->x);
	hyp_print_value(", y:", self->y);
	hyp_print_value(", z:", self->z);
	hyp_print_value(", w:", self->w);
	printf("\r\n");
}
#endif


/**
 * @ingroup matrix2
 * @brief Initializes the matrix with 0.0 in every element.
 */
HYPAPI struct matrix2 *matrix2_zero(struct matrix2 *self)
{
	HYP_MEMSET(self, 0, sizeof(struct matrix2));
	return self;
}


/**
 * @ingroup matrix2
 * @brief Initializes the matrix as an identity matrix.
 */
HYPAPI struct matrix2 *matrix2_identity(struct matrix2 *m)
{
	matrix2_zero(m);
	m->c00 = HYP_FLOAT_C(1.0);
	m->c11 = HYP_FLOAT_C(1.0);

	return m;
}


/**
 * @ingroup matrix2
 * @brief Initializes the matrix by copying mT
 *
 * @param self The matrix to initialize
 * @param mT The matrix to copy
 */
HYPAPI struct matrix2 *matrix2_set(struct matrix2 *self, const struct matrix2 *mT)
{
	uint8_t i;

	for (i = 0; i < 4; i++) {
		self->m[i] = mT->m[i];
	}

	return self;
}


/**
 * @ingroup matrix2
 * @brief Compares every element of the matrix.  Uses HYP_EPSILON for precision.
 * returns 1 if equal, 0 if different
 */
HYPAPI int matrix2_equals(const struct matrix2 *self, const struct matrix2 *mT)
{
	uint8_t i;

	for (i = 0; i < 4; i++) {
		if (scalar_equalsf(self->m[i], mT->m[i]) == 0) {
			return 0;
		}
	}

	return 1;
}


/**
 * @ingroup matrix2
 * @brief "add row and column to row and column"
 *
 * @param self The matrix being changed
 * @param mT The matrix to add
 */
HYPAPI struct matrix2 *matrix2_add(struct matrix2 *self, const struct matrix2 *mT)
{
	/* "add row and column to row and column" */
	uint8_t i;

	for (i = 0; i < 4; i++) {
		self->m[i] += mT->m[i];
	}

	return self;
}


/**
 * @ingroup matrix2
 * @brief "subtract row and column from row and column"
 *
 * @param self The matrix being changed
 * @param mT The matrix to subtract from self (self = self - mT)
 */
HYPAPI struct matrix2 *matrix2_subtract(struct matrix2 *self, const struct matrix2 *mT)
{
	/* "subtract row and column from row and column" */
	uint8_t i;

	for (i = 0; i < 4; i++) {
		self->m[i] -= mT->m[i];
	}

	return self;
}


/**
 * @ingroup matrix2
 * @brief Multiply a matrix by a scalar, returns the matrix changed
 *
 * @param self The matrix being changed
 * @param scalar The scalar factor being multiplied in
 */
HYPAPI struct matrix2 *matrix2_multiplyf(struct matrix2 *self, HYP_FLOAT scalar)
{
	uint8_t i;

	for (i = 0; i < 4; i++) {
		self->m[i] *= scalar;
	}

	return self;
}


/**
 * @ingroup matrix2
 * @brief Multiply a matrix by a matrix, returns the matrix changed
 *
 * @param self the matrix being changed
 * @param mT The matrix being multiplied into self
 *
 * self = mT * self: mT is applied after self
 */
HYPAPI struct matrix2 *matrix2_multiply(struct matrix2 *self, const struct matrix2 *mT)
{
	/* mT is the multiplicand */

	struct matrix2 r;

	matrix2_identity(&r);

	/* first row */
	r.r00 = self->c00 * mT->c00 + self->c01 * mT->c10;
	r.r01 = self->c10 * mT->c00 + self->c11 * mT->c10;

	/* second row */
	r.r10 = self->c00 * mT->c01 + self->c01 * mT->c11;
	r.r11 = self->c10 * mT->c01 + self->c11 * mT->c11;

	matrix2_set(self, &r); /* overwrite/save it */

	return self;
}


/**
 * @ingroup matrix2
 * @brief Multiply a vector by a matrix
 *
 * @param self The matrix used to do the multiplication
 * @param vT The vector being multiplied
 * @param vR The vector returned
 */
HYPAPI struct vector2 *matrix2_multiplyv2(const struct matrix2 *self, const struct vector2 *vT, struct vector2 *vR)
{
	vR->x = self->r00 * vT->x + self->r01 * vT->y;
	vR->y = self->r10 * vT->x + self->r11 * vT->y;

	return vR;
}


/**
 * @ingroup matrix2
 * @brief Transpose the matrix
 *
 * @param self The matrix being changed
 */
HYPAPI struct matrix2 *matrix2_transpose(struct matrix2 *self)
{
	return hyp_matrix2_transpose_columnrow(self);
}


/* swaps the row and column */
HYPAPI struct matrix2 *hyp_matrix2_transpose_rowcolumn(struct matrix2 *self)
{
	HYP_SWAP(&self->r01, &self->r10);

	return self;
}


/* swaps the columns and row */
HYPAPI struct matrix2 *hyp_matrix2_transpose_columnrow(struct matrix2 *self)
{
	HYP_SWAP(&self->c01, &self->c10);

	return self;
}


#ifndef HYP_NO_STDIO
/* prints out the matrix using column and row notation */
HYPAPI void hyp_matrix2_print_with_columnrow_indexer(struct matrix2 *self)
{
	hyp_print_value("", self->c00);
	hyp_print_value(", ", self->c10);
	printf("\r\n");
	hyp_print_value("", self->c01);
	hyp_print_value(", ", self->c11);
	printf("\r\n");
}
#endif


#ifndef HYP_NO_STDIO
/* prints out the matrix using row and column notation */
HYPAPI void hyp_matrix2_print_with_rowcolumn_indexer(struct matrix2 *self)
{
	hyp_print_value("", self->r00);
	hyp_print_value(", ", self->r01);
	printf("\r\n");
	hyp_print_value("", self->r10);
	hyp_print_value(", ", self->r11);
	printf("\r\n");
}
#endif

/**
 * @ingroup matrix
 * @brief creates a scaling matrix.  It's opinionated about what that means.
 *
 */
HYPAPI struct matrix2 *matrix2_make_transformation_scalingv2(struct matrix2 *self, const struct vector2 *scale)
{
	matrix2_identity(self);

	self->r00 = scale->x;
	self->r11 = scale->y;

	return self;
}


/**
 * @ingroup matrix2
 * @brief creates a rotation matrix about the z.  It's opinionated about what
 * that means.
 *
 * multiply this matrix by another matrix to rotate the other matrix
 */
HYPAPI struct matrix2 *matrix2_make_transformation_rotationf_z(struct matrix2 *m, HYP_FLOAT angle)
{
	HYP_FLOAT c = HYP_COS(angle);
	HYP_FLOAT s = HYP_SIN(angle);

	matrix2_identity(m);

	m->r00 = c;
	m->r01 = -s;
	m->r10 = s;
	m->r11 = c;

	return m;
}


/**
 * @ingroup matrix2
 * @brief Creates a temporary rotation matrix and then multiplies self by that.
 * Opinionated function about what rotation means.  It always rotates about
 * the z which it assumes is coming out of the screen.
 *
 * @param self The transformation matrix being rotated
 * @param angle the angle of rotation in radians
 *
 */
HYPAPI struct matrix2 *matrix2_rotate(struct matrix2 *self, HYP_FLOAT angle)
{
	struct matrix2 rotationMatrix;

	return matrix2_multiply(self,
		matrix2_make_transformation_rotationf_z(&rotationMatrix, angle));
}


/**
 * @ingroup matrix2
 * @brief Creates a temporary scaling matrix and then multiplies self by that.
 * Opinionated function about what scaling means.
 *
 * @param self The transformation matrix being scaled
 * @param scale the scaling vector
 *
 */
HYPAPI struct matrix2 *matrix2_scalev2(struct matrix2 *self, const struct vector2 *scale)
{
	struct matrix2 scalingMatrix;

	return matrix2_multiply(self,
		matrix2_make_transformation_scalingv2(&scalingMatrix, scale));
}


/* the largest sum of absolute values over the rows of m[] (n by n): the
 * matrix norm used by the condition estimates
 */
static HYP_FLOAT hyp_matrix_row_norm(const HYP_FLOAT *m, uint8_t n)
{
	HYP_FLOAT largest = HYP_FLOAT_C(0.0);
	HYP_FLOAT sum;
	uint8_t i;
	uint8_t j;

	for (i = 0; i < n; i++) {
		sum = HYP_FLOAT_C(0.0);
		for (j = 0; j < n; j++) {
			sum += HYP_ABS(m[i * n + j]);
		}
		largest = HYP_MAX(largest, sum);
	}

	return largest;
}


/**
 * @ingroup matrix2
 * @brief Finds the determinant of a matrix
 *
 * @param self The transformation matrix being questioned
 *
 */
HYPAPI HYP_FLOAT matrix2_determinant(const struct matrix2 *self)
{
	HYP_FLOAT determinant;

	determinant =
	  self->r00 * self->r11
	- self->r10 * self->r01
	;

	return determinant;
}


/**
 * @ingroup matrix2
 * @brief Invert a matrix.  Returns NULL, and leaves the matrix unchanged, when
 * it has no inverse (see matrix2_inverse).
 *
 * @param self The transformation matrix being inverted
 *
 */
HYPAPI struct matrix2 *matrix2_invert(struct matrix2 *self)
{
	struct matrix2 inverse;
	uint8_t i;

	if (matrix2_inverse(self, &inverse) == NULL) {
		return NULL;
	}

	for (i = 0; i < 4; i++) {
		self->m[i] = inverse.m[i];
	}

	return self;
}


/**
 * @ingroup matrix2
 * @brief Find the inverse of the matrix
 *
 * Returns NULL only when the determinant is exactly zero.  A matrix that is
 * close to having no inverse is still inverted, and the result can contain
 * very large values; the reciprocal_condition function estimates how reliable
 * the inverse is.
 *
 * @param self The transformation matrix being examined
 * @param mR the inverse of the matrix is returned here
 *
 */
HYPAPI struct matrix2 *matrix2_inverse(const struct matrix2 *self, struct matrix2 *mR)
{
	struct matrix2 inverse;
	HYP_FLOAT determinant;
	uint8_t i;

	determinant = matrix2_determinant(self);

	/* only an exactly zero determinant has no inverse (written without ==
	 * for -Wfloat-equal; a NaN determinant also returns NULL)
	 */
	if (!(determinant < HYP_FLOAT_C(0.0)) && !(determinant > HYP_FLOAT_C(0.0))) {
		return NULL;
	}

	/* find the adjugate of self */
	inverse.c00 = self->c11;
	inverse.c01 = -self->c01;
	inverse.c10 = -self->c10;
	inverse.c11 = self->c00;

	/* divide rather than multiply by 1 / determinant, which overflows when the
	 * determinant is very small and the inverse is not
	 */
	for (i = 0; i < 4; i++) {
		mR->m[i] = inverse.m[i] / determinant;
	}

	return mR;
}


/**
 * @ingroup matrix2
 * @brief Estimates how reliable the inverse is: 1 / (|M| |inverse(M)|) in the
 * row-sum norm, as LAPACK's rcond.  1 for the identity and its multiples, near
 * 0 for a nearly singular matrix, whose inverse can be dominated by rounding,
 * and 0 when the matrix has no inverse.  About -log10 of it is the number of
 * digits the inverse can lose.
 *
 * @param self The matrix being examined
 */
HYPAPI HYP_FLOAT matrix2_reciprocal_condition(const struct matrix2 *self)
{
	struct matrix2 inverse;

	if (matrix2_inverse(self, &inverse) == NULL) {
		return HYP_FLOAT_C(0.0);
	}

	return HYP_FLOAT_C(1.0) / (hyp_matrix_row_norm(self->m, 2) * hyp_matrix_row_norm(inverse.m, 2));
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
	matrix3_zero(m);
	m->c00 = HYP_FLOAT_C(1.0);
	m->c11 = HYP_FLOAT_C(1.0);
	m->c22 = HYP_FLOAT_C(1.0);

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
 * self = mT * self: mT is applied after self
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
	/* the vector is (x, y, 1) */
	vR->x = self->r00 * vT->x + self->r01 * vT->y + self->r02;
	vR->y = self->r10 * vT->x + self->r11 * vT->y + self->r12;

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
	return hyp_matrix3_transpose_columnrow(self);
}


/* swaps the row and column */
HYPAPI struct matrix3 *hyp_matrix3_transpose_rowcolumn(struct matrix3 *self)
{
	HYP_SWAP(&self->r01, &self->r10);
	HYP_SWAP(&self->r02, &self->r20);
	HYP_SWAP(&self->r12, &self->r21);

	return self;
}


/* swaps the columns and row */
HYPAPI struct matrix3 *hyp_matrix3_transpose_columnrow(struct matrix3 *self)
{
	HYP_SWAP(&self->c01, &self->c10);
	HYP_SWAP(&self->c02, &self->c20);
	HYP_SWAP(&self->c12, &self->c21);

	return self;
}


#ifndef HYP_NO_STDIO
/* prints out the matrix using column and row notation */
HYPAPI void hyp_matrix3_print_with_columnrow_indexer(struct matrix3 *self)
{
	hyp_print_value("", self->c00);
	hyp_print_value(", ", self->c10);
	hyp_print_value(", ", self->c20);
	printf("\r\n");
	hyp_print_value("", self->c01);
	hyp_print_value(", ", self->c11);
	hyp_print_value(", ", self->c21);
	printf("\r\n");
	hyp_print_value("", self->c02);
	hyp_print_value(", ", self->c12);
	hyp_print_value(", ", self->c22);
	printf("\r\n");
}
#endif


#ifndef HYP_NO_STDIO
/* prints out the matrix using row and column notation */
HYPAPI void hyp_matrix3_print_with_rowcolumn_indexer(struct matrix3 *self)
{
	hyp_print_value("", self->r00);
	hyp_print_value(", ", self->r01);
	hyp_print_value(", ", self->r02);
	printf("\r\n");
	hyp_print_value("", self->r10);
	hyp_print_value(", ", self->r11);
	hyp_print_value(", ", self->r12);
	printf("\r\n");
	hyp_print_value("", self->r20);
	hyp_print_value(", ", self->r21);
	hyp_print_value(", ", self->r22);
	printf("\r\n");
}
#endif

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
	m->r01 = -s;
	m->r10 = s;
	m->r11 = c;

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


#define HYP_CAT(a, b) HYP_PRIMITIVE_CAT(a, b)
#define HYP_PRIMITIVE_CAT(a, b) a ## b
#define HYP_DEC(x) HYP_PRIMITIVE_CAT(HYP_DEC_, x)
#define HYP_DEC_11 00
#define HYP_DEC_12 01
#define HYP_DEC_13 02
#define HYP_DEC_14 03
#define HYP_DEC_21 10
#define HYP_DEC_22 11
#define HYP_DEC_23 12
#define HYP_DEC_24 13
#define HYP_DEC_31 20
#define HYP_DEC_32 21
#define HYP_DEC_33 22
#define HYP_DEC_34 23
#define HYP_DEC_41 30
#define HYP_DEC_42 31
#define HYP_DEC_43 32
#define HYP_DEC_44 33
#define HYP_A(x) HYP_CAT(self->r,  HYP_DEC(x))
#define HYP_B(x) HYP_CAT(inverse.r, HYP_DEC(x))
#define HYP_A2(x1, x2) (HYP_A(x1) * HYP_A(x2))


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
	  (HYP_A(11) * (HYP_A2(22, 33) - HYP_A2(32, 23)))
	- (HYP_A(12) * (HYP_A2(21, 33) - HYP_A2(31, 23)))
	+ (HYP_A(13) * (HYP_A2(21, 32) - HYP_A2(31, 22)))
	;

	return determinant;
}


/**
 * @ingroup matrix3
 * @brief Invert a matrix.  Returns NULL, and leaves the matrix unchanged, when
 * it has no inverse (see matrix3_inverse).
 *
 * @param self The transformation matrix being inverted
 *
 */
HYPAPI struct matrix3 *matrix3_invert(struct matrix3 *self)
{
	struct matrix3 inverse;
	uint8_t i;

	if (matrix3_inverse(self, &inverse) == NULL) {
		return NULL;
	}

	for (i = 0; i < 9; i++) {
		self->m[i] = inverse.m[i];
	}

	return self;
}


/**
 * @ingroup matrix3
 * @brief Find the inverse of the matrix
 *
 * Returns NULL only when the determinant is exactly zero.  A matrix that is
 * close to having no inverse is still inverted, and the result can contain
 * very large values; the reciprocal_condition function estimates how reliable
 * the inverse is.
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

	/* only an exactly zero determinant has no inverse (written without ==
	 * for -Wfloat-equal; a NaN determinant also returns NULL)
	 */
	if (!(determinant < HYP_FLOAT_C(0.0)) && !(determinant > HYP_FLOAT_C(0.0))) {
		return NULL;
	}

	matrix3_identity(&inverse);

	/* find the adjugate of self */
	HYP_B(11) = HYP_A2(22, 33) - HYP_A2(32, 23);
	HYP_B(12) = HYP_A2(32, 13) - HYP_A2(12, 33);
	HYP_B(13) = HYP_A2(12, 23) - HYP_A2(22, 13);

	HYP_B(21) = HYP_A2(23, 31) - HYP_A2(33, 21);
	HYP_B(22) = HYP_A2(33, 11) - HYP_A2(13, 31);
	HYP_B(23) = HYP_A2(13, 21) - HYP_A2(23, 11);

	HYP_B(31) = HYP_A2(21, 32) - HYP_A2(31, 22);
	HYP_B(32) = HYP_A2(31, 12) - HYP_A2(11, 32);
	HYP_B(33) = HYP_A2(11, 22) - HYP_A2(21, 12);

	/* divide rather than multiply by 1 / determinant, which overflows when the
	 * determinant is very small and the inverse is not
	 */
	for (i = 0; i < 9; i++) {
		mR->m[i] = inverse.m[i] / determinant;
	}

	return mR;
}


/**
 * @ingroup matrix3
 * @brief Estimates how reliable the inverse is: 1 / (|M| |inverse(M)|) in the
 * row-sum norm, as LAPACK's rcond.  1 for the identity and its multiples, near
 * 0 for a nearly singular matrix, whose inverse can be dominated by rounding,
 * and 0 when the matrix has no inverse.  About -log10 of it is the number of
 * digits the inverse can lose.
 *
 * @param self The matrix being examined
 */
HYPAPI HYP_FLOAT matrix3_reciprocal_condition(const struct matrix3 *self)
{
	struct matrix3 inverse;

	if (matrix3_inverse(self, &inverse) == NULL) {
		return HYP_FLOAT_C(0.0);
	}

	return HYP_FLOAT_C(1.0) / (hyp_matrix_row_norm(self->m, 3) * hyp_matrix_row_norm(inverse.m, 3));
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
	matrix4_zero(m);
	m->c00 = HYP_FLOAT_C(1.0);
	m->c11 = HYP_FLOAT_C(1.0);
	m->c22 = HYP_FLOAT_C(1.0);
	m->c33 = HYP_FLOAT_C(1.0);

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
 * self = mT * self: mT is applied after self
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
	r.r33 = self->c30 * mT->c03 + self->c31 * mT->c13 + self->c32 * mT->c23 + self->c33 * mT->c33;

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
	/* the vector is (x, y, 0, 1) */
	vR->x = vT->x * self->r00 + vT->y * self->r01 + self->r03;
	vR->y = vT->x * self->r10 + vT->y * self->r11 + self->r13;

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
	return hyp_matrix4_transpose_columnrow(self);
}


/* swaps the row and column */
HYPAPI struct matrix4 *hyp_matrix4_transpose_rowcolumn(struct matrix4 *self)
{
	HYP_SWAP(&self->r01, &self->r10);
	HYP_SWAP(&self->r02, &self->r20);
	HYP_SWAP(&self->r03, &self->r30);
	HYP_SWAP(&self->r12, &self->r21);
	HYP_SWAP(&self->r13, &self->r31);
	HYP_SWAP(&self->r23, &self->r32);

	return self;
}


/* swaps the columns and row */
HYPAPI struct matrix4 *hyp_matrix4_transpose_columnrow(struct matrix4 *self)
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
/* prints out the matrix using column and row notation */
HYPAPI void hyp_matrix4_print_with_columnrow_indexer(struct matrix4 *self)
{
	hyp_print_value("", self->c00);
	hyp_print_value(", ", self->c10);
	hyp_print_value(", ", self->c20);
	hyp_print_value(", ", self->c30);
	printf("\r\n");
	hyp_print_value("", self->c01);
	hyp_print_value(", ", self->c11);
	hyp_print_value(", ", self->c21);
	hyp_print_value(", ", self->c31);
	printf("\r\n");
	hyp_print_value("", self->c02);
	hyp_print_value(", ", self->c12);
	hyp_print_value(", ", self->c22);
	hyp_print_value(", ", self->c32);
	printf("\r\n");
	hyp_print_value("", self->c03);
	hyp_print_value(", ", self->c13);
	hyp_print_value(", ", self->c23);
	hyp_print_value(", ", self->c33);
	printf("\r\n");
}


/* prints out the matrix using row and column notation */
HYPAPI void hyp_matrix4_print_with_rowcolumn_indexer(struct matrix4 *self)
{
	hyp_print_value("", self->r00);
	hyp_print_value(", ", self->r01);
	hyp_print_value(", ", self->r02);
	hyp_print_value(", ", self->r03);
	printf("\r\n");
	hyp_print_value("", self->r10);
	hyp_print_value(", ", self->r11);
	hyp_print_value(", ", self->r12);
	hyp_print_value(", ", self->r13);
	printf("\r\n");
	hyp_print_value("", self->r20);
	hyp_print_value(", ", self->r21);
	hyp_print_value(", ", self->r22);
	hyp_print_value(", ", self->r23);
	printf("\r\n");
	hyp_print_value("", self->r30);
	hyp_print_value(", ", self->r31);
	hyp_print_value(", ", self->r32);
	hyp_print_value(", ", self->r33);
	printf("\r\n");
}
#endif


/**
 * @ingroup matrix4
 * @brief converts the quaternion to a 4x4 rotation matrix, applied as M * v
 * (right hand rule); the same as matrix4_set_from_quaternion
 *
 */
HYPAPI struct matrix4 *matrix4_make_transformation_rotationq(struct matrix4 *self, const struct quaternion *qT)
{
	return matrix4_set_from_quaternion(self, qT);
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
	m->r12 = -s;
	m->r21 = s;
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

	m->r00 = c;
	m->r02 = s;
	m->r20 = -s;
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

	m->r00 = c;
	m->r01 = -s;
	m->r10 = s;
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


/* the 4x4 in 2x2 blocks, self = [A B; C D], with X# the adjugate of X: the
 * determinants of the blocks (|A|, |B|, |C|, |D|), A# B, D# C, and the
 * determinant |A| |D| + |B| |C| - trace(A# B D# C).  The inverse and the
 * determinant share it, so the two agree.
 */
static HYP_FLOAT hyp_matrix4_blocks(const struct matrix4 *m, HYP_FLOAT *blocks, HYP_FLOAT *ab, HYP_FLOAT *dc)
{
	blocks[0] = m->r00 * m->r11 - m->r01 * m->r10;
	blocks[1] = m->r02 * m->r13 - m->r03 * m->r12;
	blocks[2] = m->r20 * m->r31 - m->r21 * m->r30;
	blocks[3] = m->r22 * m->r33 - m->r23 * m->r32;

	/* A# = [r11 -r01; -r10 r00] */
	ab[0] = m->r11 * m->r02 - m->r01 * m->r12;
	ab[1] = m->r11 * m->r03 - m->r01 * m->r13;
	ab[2] = m->r00 * m->r12 - m->r10 * m->r02;
	ab[3] = m->r00 * m->r13 - m->r10 * m->r03;

	/* D# = [r33 -r23; -r32 r22] */
	dc[0] = m->r33 * m->r20 - m->r23 * m->r30;
	dc[1] = m->r33 * m->r21 - m->r23 * m->r31;
	dc[2] = m->r22 * m->r30 - m->r32 * m->r20;
	dc[3] = m->r22 * m->r31 - m->r32 * m->r21;

	return (blocks[0] * blocks[3] + blocks[1] * blocks[2])
		- ((ab[0] * dc[0] + ab[1] * dc[2]) + (ab[2] * dc[1] + ab[3] * dc[3]));
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
	HYP_FLOAT blocks[4];
	HYP_FLOAT ab[4];
	HYP_FLOAT dc[4];

	return hyp_matrix4_blocks(self, blocks, ab, dc);
}


/**
 * @ingroup matrix4
 * @brief Invert a matrix.  Returns NULL, and leaves the matrix unchanged, when
 * it has no inverse (see matrix4_inverse).
 *
 * @param self The transformation matrix being inverted
 *
 */
HYPAPI struct matrix4 *matrix4_invert(struct matrix4 *self)
{
	struct matrix4 inverse;
	uint8_t i;

	if (matrix4_inverse(self, &inverse) == NULL) {
		return NULL;
	}

	for (i = 0; i < 16; i++) {
		self->m[i] = inverse.m[i];
	}

	return self;
}


/**
 * @ingroup matrix4
 * @brief Find the inverse of the matrix
 *
 * Returns NULL only when the determinant is exactly zero.  A matrix that is
 * close to having no inverse is still inverted, and the result can contain
 * very large values; the reciprocal_condition function estimates how reliable
 * the inverse is.
 *
 * @param self The transformation matrix being examined
 * @param mR the inverse of the matrix is returned here
 *
 */
HYPAPI struct matrix4 *matrix4_inverse(const struct matrix4 *self, struct matrix4 *mR)
{
	HYP_FLOAT blocks[4];
	HYP_FLOAT ab[4];
	HYP_FLOAT dc[4];
	HYP_FLOAT x[4];
	HYP_FLOAT determinant;
	struct matrix4 inverse;
	uint8_t i;

	determinant = hyp_matrix4_blocks(self, blocks, ab, dc);

	/* only an exactly zero determinant has no inverse (written without ==
	 * for -Wfloat-equal; a NaN determinant also returns NULL)
	 */
	if (!(determinant < HYP_FLOAT_C(0.0)) && !(determinant > HYP_FLOAT_C(0.0))) {
		return NULL;
	}

	/* the blocks of the inverse, times the determinant:
	 * [ (|D| A - B D# C)#  (|B| C - D (A# B)#)# ;
	 *   (|C| B - A (D# C)#)#  (|A| D - C A# B)# ]
	 */
	x[0] = blocks[3] * self->r00 - (self->r02 * dc[0] + self->r03 * dc[2]);
	x[1] = blocks[3] * self->r01 - (self->r02 * dc[1] + self->r03 * dc[3]);
	x[2] = blocks[3] * self->r10 - (self->r12 * dc[0] + self->r13 * dc[2]);
	x[3] = blocks[3] * self->r11 - (self->r12 * dc[1] + self->r13 * dc[3]);
	inverse.r00 = x[3];
	inverse.r01 = -x[1];
	inverse.r10 = -x[2];
	inverse.r11 = x[0];

	x[0] = blocks[0] * self->r22 - (self->r20 * ab[0] + self->r21 * ab[2]);
	x[1] = blocks[0] * self->r23 - (self->r20 * ab[1] + self->r21 * ab[3]);
	x[2] = blocks[0] * self->r32 - (self->r30 * ab[0] + self->r31 * ab[2]);
	x[3] = blocks[0] * self->r33 - (self->r30 * ab[1] + self->r31 * ab[3]);
	inverse.r22 = x[3];
	inverse.r23 = -x[1];
	inverse.r32 = -x[2];
	inverse.r33 = x[0];

	/* (A# B)# = [ab3 -ab1; -ab2 ab0] */
	x[0] = blocks[1] * self->r20 - (self->r22 * ab[3] - self->r23 * ab[2]);
	x[1] = blocks[1] * self->r21 - (self->r23 * ab[0] - self->r22 * ab[1]);
	x[2] = blocks[1] * self->r30 - (self->r32 * ab[3] - self->r33 * ab[2]);
	x[3] = blocks[1] * self->r31 - (self->r33 * ab[0] - self->r32 * ab[1]);
	inverse.r02 = x[3];
	inverse.r03 = -x[1];
	inverse.r12 = -x[2];
	inverse.r13 = x[0];

	/* (D# C)# = [dc3 -dc1; -dc2 dc0] */
	x[0] = blocks[2] * self->r02 - (self->r00 * dc[3] - self->r01 * dc[2]);
	x[1] = blocks[2] * self->r03 - (self->r01 * dc[0] - self->r00 * dc[1]);
	x[2] = blocks[2] * self->r12 - (self->r10 * dc[3] - self->r11 * dc[2]);
	x[3] = blocks[2] * self->r13 - (self->r11 * dc[0] - self->r10 * dc[1]);
	inverse.r20 = x[3];
	inverse.r21 = -x[1];
	inverse.r30 = -x[2];
	inverse.r31 = x[0];

	/* divide rather than multiply by 1 / determinant, which overflows when the
	 * determinant is very small and the inverse is not
	 */
	for (i = 0; i < 16; i++) {
		mR->m[i] = inverse.m[i] / determinant;
	}

	return mR;
}


/**
 * @ingroup matrix4
 * @brief Estimates how reliable the inverse is: 1 / (|M| |inverse(M)|) in the
 * row-sum norm, as LAPACK's rcond.  1 for the identity and its multiples, near
 * 0 for a nearly singular matrix, whose inverse can be dominated by rounding,
 * and 0 when the matrix has no inverse.  About -log10 of it is the number of
 * digits the inverse can lose.
 *
 * @param self The matrix being examined
 */
HYPAPI HYP_FLOAT matrix4_reciprocal_condition(const struct matrix4 *self)
{
	struct matrix4 inverse;

	if (matrix4_inverse(self, &inverse) == NULL) {
		return HYP_FLOAT_C(0.0);
	}

	return HYP_FLOAT_C(1.0) / (hyp_matrix_row_norm(self->m, 4) * hyp_matrix_row_norm(inverse.m, 4));
}


/**
 * @ingroup matrix4
 * @brief The normal matrix: the inverse transpose of the upper 3x3, with no
 * translation and 1 in r33.  Surface normals transformed by it stay
 * perpendicular to the surface under a non-uniform scale, where the matrix
 * itself would tilt them.  Returns NULL when the upper 3x3 has no inverse
 * (its determinant is exactly zero).
 *
 * @param self The transformation matrix
 * @param mR The normal matrix is returned here
 */
HYPAPI struct matrix4 *matrix4_normal_matrix(const struct matrix4 *self, struct matrix4 *mR)
{
	struct matrix4 cofactors;
	HYP_FLOAT determinant;
	uint8_t i;

	/* the inverse transpose is the cofactor matrix over the determinant */
	matrix4_identity(&cofactors);
	cofactors.r00 = self->r11 * self->r22 - self->r12 * self->r21;
	cofactors.r01 = self->r12 * self->r20 - self->r10 * self->r22;
	cofactors.r02 = self->r10 * self->r21 - self->r11 * self->r20;
	cofactors.r10 = self->r02 * self->r21 - self->r01 * self->r22;
	cofactors.r11 = self->r00 * self->r22 - self->r02 * self->r20;
	cofactors.r12 = self->r01 * self->r20 - self->r00 * self->r21;
	cofactors.r20 = self->r01 * self->r12 - self->r02 * self->r11;
	cofactors.r21 = self->r02 * self->r10 - self->r00 * self->r12;
	cofactors.r22 = self->r00 * self->r11 - self->r01 * self->r10;

	determinant = self->r00 * cofactors.r00 + self->r01 * cofactors.r01 + self->r02 * cofactors.r02;

	/* written without == for -Wfloat-equal; a NaN determinant also returns NULL */
	if (!(determinant < HYP_FLOAT_C(0.0)) && !(determinant > HYP_FLOAT_C(0.0))) {
		return NULL;
	}

	matrix4_identity(mR);
	for (i = 0; i < 3; i++) {
		mR->m44[i][0] = cofactors.m44[i][0] / determinant;
		mR->m44[i][1] = cofactors.m44[i][1] / determinant;
		mR->m44[i][2] = cofactors.m44[i][2] / determinant;
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
	self->x = HYP_FLOAT_C(0.0);
	self->y = HYP_FLOAT_C(0.0);
	self->z = HYP_FLOAT_C(0.0);
	self->w = HYP_FLOAT_C(1.0);

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
 * @brief Sets the quaternion to a random unit quaternion, which is a random
 * rotation evenly distributed over all rotations.  Uses three draws from
 * scalar_random_rangef, in this order: u, then two angles.
 */
HYPAPI struct quaternion *quaternion_set_random_unit(struct quaternion *self)
{
	hyp_set_random_unit4(&self->x, &self->y, &self->z, &self->w);
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
 * angle.  The axis does not need to be unit length; a zero axis gives the
 * identity.
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
	HYP_FLOAT s = HYP_SIN(angle / HYP_FLOAT_C(2.0));
	HYP_FLOAT c = HYP_COS(angle / HYP_FLOAT_C(2.0));
	struct vector3 axis;

	/* the formula needs a unit axis: normalizing the result instead would
	 * change the angle
	 */
	if (!(hyp_normalize(vector3_setf3(&axis, x, y, z)->v, 3) > HYP_FLOAT_C(0.0))) {
		return quaternion_identity(self);
	}
	x = axis.x;
	y = axis.y;
	z = axis.z;

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
 * angle.  The axis does not need to be unit length; a zero axis gives the
 * identity.
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
	self->w = HYP_COS(az / HYP_FLOAT_C(2.0)) * HYP_COS(ay / HYP_FLOAT_C(2.0)) * HYP_COS(ax / HYP_FLOAT_C(2.0)) + HYP_SIN(az / HYP_FLOAT_C(2.0)) * HYP_SIN(ay / HYP_FLOAT_C(2.0)) * HYP_SIN(ax / HYP_FLOAT_C(2.0));
	self->x = HYP_COS(az / HYP_FLOAT_C(2.0)) * HYP_COS(ay / HYP_FLOAT_C(2.0)) * HYP_SIN(ax / HYP_FLOAT_C(2.0)) - HYP_SIN(az / HYP_FLOAT_C(2.0)) * HYP_SIN(ay / HYP_FLOAT_C(2.0)) * HYP_COS(ax / HYP_FLOAT_C(2.0));
	self->y = HYP_COS(az / HYP_FLOAT_C(2.0)) * HYP_SIN(ay / HYP_FLOAT_C(2.0)) * HYP_COS(ax / HYP_FLOAT_C(2.0)) + HYP_SIN(az / HYP_FLOAT_C(2.0)) * HYP_COS(ay / HYP_FLOAT_C(2.0)) * HYP_SIN(ax / HYP_FLOAT_C(2.0));
	self->z = HYP_SIN(az / HYP_FLOAT_C(2.0)) * HYP_COS(ay / HYP_FLOAT_C(2.0)) * HYP_COS(ax / HYP_FLOAT_C(2.0)) - HYP_COS(az / HYP_FLOAT_C(2.0)) * HYP_SIN(ay / HYP_FLOAT_C(2.0)) * HYP_SIN(ax / HYP_FLOAT_C(2.0));

	quaternion_normalize(self);

	return self;
}


/**
 * @ingroup quaternion
 * @brief Sets the quaternion to the rotation in the upper 3x3 of a matrix4
 * (applied as M * v).  The upper 3x3 must be a rotation; for a matrix with
 * scale use matrix4_transformation_decompose.  The result is a unit
 * quaternion.
 *
 * @param self the quaternion
 * @param mT the rotation matrix
 */
HYPAPI struct quaternion *quaternion_set_from_matrix4(struct quaternion *self, const struct matrix4 *mT)
{
	HYP_FLOAT trace = mT->r00 + mT->r11 + mT->r22;
	HYP_FLOAT s;

	/* Shepperd: divide by the largest of 4w^2, 4x^2, 4y^2, 4z^2 so the square
	 * root and the division stay accurate
	 */
	if (trace > HYP_FLOAT_C(0.0)) {
		s = HYP_FLOAT_C(2.0) * HYP_SQRT(HYP_FLOAT_C(1.0) + trace);
		self->w = s / HYP_FLOAT_C(4.0);
		self->x = (mT->r21 - mT->r12) / s;
		self->y = (mT->r02 - mT->r20) / s;
		self->z = (mT->r10 - mT->r01) / s;
	} else if (mT->r00 > mT->r11 && mT->r00 > mT->r22) {
		s = HYP_FLOAT_C(2.0) * HYP_SQRT(HYP_FLOAT_C(1.0) + mT->r00 - mT->r11 - mT->r22);
		self->w = (mT->r21 - mT->r12) / s;
		self->x = s / HYP_FLOAT_C(4.0);
		self->y = (mT->r01 + mT->r10) / s;
		self->z = (mT->r02 + mT->r20) / s;
	} else if (mT->r11 > mT->r22) {
		s = HYP_FLOAT_C(2.0) * HYP_SQRT(HYP_FLOAT_C(1.0) + mT->r11 - mT->r00 - mT->r22);
		self->w = (mT->r02 - mT->r20) / s;
		self->x = (mT->r01 + mT->r10) / s;
		self->y = s / HYP_FLOAT_C(4.0);
		self->z = (mT->r12 + mT->r21) / s;
	} else {
		s = HYP_FLOAT_C(2.0) * HYP_SQRT(HYP_FLOAT_C(1.0) + mT->r22 - mT->r00 - mT->r11);
		self->w = (mT->r10 - mT->r01) / s;
		self->x = (mT->r02 + mT->r20) / s;
		self->y = (mT->r12 + mT->r21) / s;
		self->z = s / HYP_FLOAT_C(4.0);
	}

	return quaternion_normalize(self);
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

	*ax = HYP_ATAN2(qy * qz + qw * qx, HYP_FLOAT_C(0.5) - ((qx * qx) + (qy * qy)));
	*ay = HYP_ASIN(-HYP_FLOAT_C(2.0) * ((qx * qz) - (qw * qy)));
	*az = HYP_ATAN2(((qx * qy) + (qw * qz)), HYP_FLOAT_C(0.5) - ((qy * qy) + (qz * qz)));
}


/**
 * @ingroup quaternion
 * @brief Checks for mathematical equality within EPSILON.
 *
 */
HYPAPI int quaternion_equals(const struct quaternion *self, const struct quaternion *qT)
{
	return scalar_equalsf(self->x, qT->x) &&
		scalar_equalsf(self->y, qT->y) &&
		scalar_equalsf(self->z, qT->z) &&
		scalar_equalsf(self->w, qT->w);
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
	/* summed in pairs: a smaller rounding error than one running sum */
	return ((self->x * self->x) + (self->y * self->y)) + ((self->z * self->z) + (self->w * self->w));
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
	HYP_FLOAT norm = quaternion_norm(self);
	HYP_FLOAT length;

	/* conjugate / |q|^2 */
	if (norm > HYP_FLOAT_C(1e-30) && norm < HYP_FLOAT_C(1e30)) {
		quaternion_conjugate(self);
		self->x /= norm;
		self->y /= norm;
		self->z /= norm;
		self->w /= norm;
		return self;
	}

	/* outside that range |q|^2 overflows or underflows: (q / |q|) / |q|.
	 * Only the zero quaternion has no inverse; NaN is also left unchanged.
	 */
	length = hyp_normalize(self->q, 4);

	if (!(length > HYP_FLOAT_C(0.0))) {
		return self;
	}

	quaternion_conjugate(self);

	self->x /= length;
	self->y /= length;
	self->z /= length;
	self->w /= length;

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
	/* the zero quaternion is left unchanged */
	hyp_normalize(self->q, 4);
	return self;
}


/**
 * @ingroup quaternion
 * @brief if the norm is 1.0, then the quaternion is said to be a 'unit
 * quaternion'
 */
HYPAPI short quaternion_is_unit(struct quaternion *self)
{
	return scalar_equalsf(HYP_FLOAT_C(1.0), quaternion_norm(self));
}


/**
 * @ingroup quaternion
 * @brief if the scalar is 0.0 (w == 0.0), then the quaternion is said to be a
 * 'pure quaternion'.  w is compared relative to the size of the quaternion,
 * so a scaled quaternion gives the same answer.
 */
HYPAPI short quaternion_is_pure(struct quaternion *self)
{
	return HYP_ABS(self->w) <= HYP_EPSILON * hyp_length(self->q, 4);
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

	/* percent 0 and 1 give start and end exactly */
	f1 = HYP_FLOAT_C(1.0) - percent;
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
 * This function computes shortest arc: when start . end is negative it
 * moves toward -end (the same rotation), so percent 0 gives start exactly and
 * percent 1 gives end or -end.
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
	struct quaternion target;
	struct quaternion unit_start;
	struct quaternion unit_target;
	struct quaternion difference;
	struct quaternion sum;

	/* the sign of the dot product picks the shorter arc */
	quaternion_set(&target, end);
	dot = quaternion_dot_product(start, &target);

	/* q and -q are the same rotation: when the dot is negative, go toward
	 * -end, the shortest arc.  The result starts at start and ends at -end.
	 */
	if (dot < HYP_FLOAT_C(0.0)) {
		quaternion_negate(&target);
	}

	/* the angle between start and target on the 4D sphere, from their unit
	 * quaternions: with |s - t| and |s + t|, which stays accurate when they are
	 * nearly the same, where acos(dot) does not (and is NaN when rounding puts
	 * the dot above 1)
	 */
	quaternion_set(&unit_start, start);
	quaternion_set(&unit_target, &target);
	hyp_normalize(unit_start.q, 4);
	hyp_normalize(unit_target.q, 4);
	quaternion_subtract(quaternion_set(&difference, &unit_start), &unit_target);
	quaternion_add(quaternion_set(&sum, &unit_start), &unit_target);
	theta = HYP_FLOAT_C(2.0) * HYP_ATAN2(quaternion_magnitude(&difference), quaternion_magnitude(&sum));
	s = HYP_SIN(theta);

	/* the same direction: nothing to turn */
	if (!(s > HYP_FLOAT_C(0.0))) {
		quaternion_lerp(start, &target, percent, qR);
		return qR;
	}

	f1 = HYP_SIN((HYP_FLOAT_C(1.0) - percent) * theta) / s;
	f2 = HYP_SIN(percent * theta) / s;

	/* this expanded form avoids calling quaternion_multiply
	 * and quaternion_add
	 */
	qR->w = f1 * start->w + f2 * target.w;
	qR->x = f1 * start->x + f2 * target.x;
	qR->y = f1 * start->y + f2 * target.y;
	qR->z = f1 * start->z + f2 * target.z;

	return qR;
}


/**
 * @ingroup quaternion
 * @brief The dot product of the quaternions as 4D vectors.  For unit
 * quaternions it is the cosine of half the angle between the two rotations
 * (q and -q are the same rotation, so its sign picks the shorter arc).  Used by
 * SLERP.
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

	/* summed in pairs: a smaller rounding error than one running sum */
	r.x = (self->w * qT->x + self->x * qT->w) + (self->y * qT->z - self->z * qT->y);
	r.y = (self->w * qT->y + self->y * qT->w) + (self->z * qT->x - self->x * qT->z);
	r.z = (self->w * qT->z + self->z * qT->w) + (self->x * qT->y - self->y * qT->x);
	r.w = (self->w * qT->w - self->x * qT->x) - (self->y * qT->y + self->z * qT->z);

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
	r.w = - self->x * vT->x - self->y * vT->y - self->z * vT->z;

	quaternion_set(self, &r); /* overwrite/save it */

	return self;
}


/**
 * @ingroup quaternion
 * @brief Retrieves the axis and angle from the quaternion.  The quaternion does
 * not need to be unit length.  The axis is unit length; for the identity, which
 * has no axis, it is zero and the angle is 0.  The angle is in [0, pi]; q and -q
 * give the same result.
 *
 * @param self the quaternion
 * @param vR the axis that will be filled
 * @param angle the angle in radians; will be filled with the angle value
 *
 */
HYPAPI void quaternion_get_axis_anglev3(const struct quaternion *self, struct vector3 *vR, HYP_FLOAT *angle)
{
	HYP_FLOAT length;
	HYP_FLOAT w = self->w;

	/* |(x, y, z)| = |q| sin(angle / 2) and w = |q| cos(angle / 2); atan2 stays
	 * accurate for small angles, where sqrt(1 - w^2) and acos(w) do not
	 */
	vector3_setf3(vR, self->x, self->y, self->z);
	length = hyp_normalize(vR->v, 3);

	if (!(length > HYP_FLOAT_C(0.0))) {
		vector3_zero(vR);
	}

	/* q and -q are the same rotation: use the one with w >= 0, whose angle
	 * is at most pi
	 */
	if (w < HYP_FLOAT_C(0.0)) {
		vector3_negate(vR);
		w = -w;
	}

	*angle = HYP_FLOAT_C(2.0) * HYP_ATAN2(length, w);
}


#ifndef HYP_NO_STDIO
/* prints out the elements of the quaternion to stdout */
HYPAPI void hyp_quaternion_print(const struct quaternion *self)
{
	hyp_print_value("x:", self->x);
	hyp_print_value(", y:", self->y);
	hyp_print_value(", z:", self->z);
	hyp_print_value(", w:", self->w);
	printf("\r\n");
}
#endif


/**
 * @ingroup quaternion
 * @brief Given two vectors, find a quaternion that will get you from one to
 * the other
 *
 * The result is the unit quaternion for the shortest rotation that turns the
 * direction of from into the direction of to.  The vectors do not need to be
 * unit length.  For opposite vectors it is a half turn about an axis
 * perpendicular to from.  If either vector has zero length there is no
 * direction to rotate, and the result is the identity.
 *
 * @param from the starting vector
 * @param to the ending vector
 * @param qR the resulting quaternion that gets you from the starting vector
 * to the ending vector
 */
HYPAPI struct quaternion *quaternion_get_rotation_tov3(const struct vector3 *from, const struct vector3 *to, struct quaternion *qR)
{
	struct vector3 f;
	struct vector3 t;
	struct vector3 sum;
	struct vector3 difference;
	struct vector3 axis;
	HYP_FLOAT half_cos;
	HYP_FLOAT half_sin;
	HYP_FLOAT axis_length;

	/* a zero length vector has no direction */
	vector3_set(&f, from);
	vector3_set(&t, to);
	if (!(hyp_normalize(f.v, 3) > HYP_FLOAT_C(0.0)) || !(hyp_normalize(t.v, 3) > HYP_FLOAT_C(0.0))) {
		return quaternion_identity(qR);
	}

	/* for unit vectors at angle a: |f + t| = 2 cos(a/2) and |f - t| = 2 sin(a/2).
	 * Both stay accurate when the vectors are nearly opposite, where f . t
	 * does not.
	 */
	vector3_add(vector3_set(&sum, &f), &t);
	vector3_subtract(vector3_set(&difference, &f), &t);
	half_cos = vector3_magnitude(&sum) / HYP_FLOAT_C(2.0);
	half_sin = vector3_magnitude(&difference) / HYP_FLOAT_C(2.0);

	/* f x (f + t) has the direction of f x t and stays accurate when f + t
	 * is small
	 */
	vector3_cross_product(&axis, &f, &sum);
	axis_length = hyp_normalize(axis.v, 3);

	if (!(axis_length > HYP_FLOAT_C(0.0))) {
		if (half_cos > HYP_FLOAT_C(0.0)) {
			/* same direction */
			return quaternion_identity(qR);
		}

		/* opposite: a half turn about an axis perpendicular to f, using the
		 * coordinate axis least aligned with it
		 */
		if (HYP_ABS(f.x) <= HYP_ABS(f.y) && HYP_ABS(f.x) <= HYP_ABS(f.z)) {
			vector3_cross_product(&axis, &f, HYP_VECTOR3_UNIT_X);
		} else if (HYP_ABS(f.y) <= HYP_ABS(f.z)) {
			vector3_cross_product(&axis, &f, HYP_VECTOR3_UNIT_Y);
		} else {
			vector3_cross_product(&axis, &f, HYP_VECTOR3_UNIT_Z);
		}

		hyp_normalize(axis.v, 3);
		half_sin = HYP_FLOAT_C(1.0);
	}

	qR->x = axis.x * half_sin;
	qR->y = axis.y * half_sin;
	qR->z = axis.z * half_sin;
	qR->w = half_cos;

	/* half_cos and half_sin each round: make the result exactly unit length */
	hyp_normalize(qR->q, 4);

	return qR;
}



/**
 * @ingroup quaternion
 * @brief rotate a quaternion by a quaternion
 * (basically, multiply and then normalize)
 *
 * @param self the quaternion being rotated
 * @param qT the other quaternion
 *
 */
HYPAPI struct quaternion *quaternion_rotate_by_quaternion(struct quaternion *self, const struct quaternion *qT)
{
	/* self = self * qT */
	quaternion_multiply(self, qT);
	quaternion_normalize(self);

	return self;
}


/**
 * @ingroup quaternion
 * @brief rotate a quaternion by the indicated axis and angle
 * first it makes a quaternion from the axis/angle
 * then it rotates the quaternion by that axis/angle
 *
 * @param self the quaternion being rotated
 * @param axis the axis of rotation (any length)
 * @param angle the angle in radians (right-hand rule)
 *
 */
HYPAPI struct quaternion *quaternion_rotate_by_axis_angle(struct quaternion *self, const struct vector3 *axis, HYP_FLOAT angle)
{
	struct quaternion qT;

	quaternion_set_from_axis_anglev3(&qT, axis, angle);
	quaternion_rotate_by_quaternion(self, &qT);

	return self;
}



/**
 * @ingroup quaternion
 * @brief returns a score that seeks to describe the difference
 * between two quaternions: the squared distance between them, 0 for the same
 * rotation.  q and -q are the same rotation.
 *
 */
HYPAPI HYP_FLOAT quaternion_difference(const struct quaternion *q1, const struct quaternion *q2)
{
	struct quaternion diff;
	struct quaternion sum;

	quaternion_subtract(quaternion_set(&diff, q2), q1);
	quaternion_add(quaternion_set(&sum, q2), q1);

	/* the distance to q1 or to -q1, whichever is closer */
	return HYP_MIN(quaternion_norm(&diff), quaternion_norm(&sum));
}


/**
 * @ingroup quaternion
 * @brief rotates the quaternion by Euler angles, in radians: about X first,
 * then Y, then Z (the order of quaternion_set_from_euler_anglesf3)
 *
 * @param self the quaternion being rotated
 * @param ax the angle about X
 * @param ay the angle about Y
 * @param az the angle about Z
 *
 */
HYPAPI struct quaternion *quaternion_rotate_by_euler_angles(struct quaternion *self, HYP_FLOAT ax, HYP_FLOAT ay, HYP_FLOAT az)
{
	struct quaternion qT;

	/* make a quaternion from the eulers */
	quaternion_set_from_euler_anglesf3(&qT, ax, ay, az);

	/* rotate the quaternion by it */
	quaternion_rotate_by_quaternion(self, &qT);

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
 * @ingroup quaternion
 * @brief Computes the angle of the rotation from self to qT, in radians
 * (0 to pi).  q and -q are the same rotation, and the lengths do not matter.
 * Returns 0 when either quaternion has zero length.
 *
 * For unit a and b with a . b >= 0 and angle t: |a - b| = 2 sin(t/4) and
 * |a + b| = 2 cos(t/4), so \f$t = 4 \, atan2(|a - b|, |a + b|)\f$, which stays
 * accurate for small angles where acos does not.
 */
HYPAPI HYP_FLOAT quaternion_angle_between(const struct quaternion *self, const struct quaternion *qT)
{
	struct quaternion a;
	struct quaternion b;
	struct quaternion sum;
	struct quaternion difference;

	if (!(hyp_normalize(quaternion_set(&a, self)->q, 4) > HYP_FLOAT_C(0.0))
	    || !(hyp_normalize(quaternion_set(&b, qT)->q, 4) > HYP_FLOAT_C(0.0))) {
		return HYP_FLOAT_C(0.0);
	}

	/* q and -q are the same rotation: take the closer one */
	if (quaternion_dot_product(&a, &b) < HYP_FLOAT_C(0.0)) {
		quaternion_negate(&b);
	}

	quaternion_add(quaternion_set(&sum, &a), &b);
	quaternion_subtract(quaternion_set(&difference, &a), &b);

	return HYP_FLOAT_C(4.0) * HYP_ATAN2(quaternion_magnitude(&difference), quaternion_magnitude(&sum));
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
 * @ingroup matrix4
 * @brief creates a perspective projection matrix for right-handed coordinates
 * (the camera looks down -Z), with fovy the vertical field of view in radians.
 * Depth maps to 0 at zNear and 1 at zFar (-1 and 1 with
 * HYP_DEPTH_MINUS_ONE_TO_ONE).  Apply it as M * v (matrix4_multiplyv4) and
 * divide by w.
 */
HYPAPI struct matrix4 *matrix4_projection_perspective_fovy_rh(struct matrix4 *self, HYP_FLOAT fovy, HYP_FLOAT aspect, HYP_FLOAT zNear, HYP_FLOAT zFar)
{
	HYP_FLOAT h;
	HYP_FLOAT w;
	HYP_FLOAT p;
	HYP_FLOAT q;

	h = HYP_COT(fovy / HYP_FLOAT_C(2.0));
	w = h / aspect;

#ifdef HYP_DEPTH_MINUS_ONE_TO_ONE
	p = (zFar + zNear) / (zNear - zFar);
	q = HYP_FLOAT_C(2.0) * zFar * zNear / (zNear - zFar);
#else
	p = zFar / (zNear - zFar);
	q = zNear * p;
#endif

	matrix4_zero(self);

	self->r00 = w;
	self->r11 = h;
	self->r22 = p;
	self->r23 = q;
	self->r32 = -HYP_FLOAT_C(1.0); /* w = -z: in front of the camera z is negative */

	return self;
}


/**
 * @ingroup matrix4
 * @brief make an orthographic projection matrix for right-handed coordinates
 * (the camera looks down -Z).  The box maps to -1..1 in x and y; depth maps
 * to 0 at zNear and 1 at zFar (-1 and 1 with HYP_DEPTH_MINUS_ONE_TO_ONE).
 * Apply it as M * v.
 */
HYPAPI struct matrix4 *matrix4_projection_ortho3d_rh(struct matrix4 *self,
							 HYP_FLOAT xmin, HYP_FLOAT xmax,
							 HYP_FLOAT ymin, HYP_FLOAT ymax,
							 HYP_FLOAT zNear, HYP_FLOAT zFar)
{
	HYP_FLOAT width;
	HYP_FLOAT height;

	matrix4_zero(self);

	width = xmax - xmin;
	height = ymax - ymin;

	self->r00 = HYP_FLOAT_C(2.0) / width;
	self->r03 = -(xmax + xmin) / width;
	self->r11 = HYP_FLOAT_C(2.0) / height;
	self->r13 = -(ymax + ymin) / height;
#ifdef HYP_DEPTH_MINUS_ONE_TO_ONE
	self->r22 = HYP_FLOAT_C(2.0) / (zNear - zFar);
	self->r23 = (zFar + zNear) / (zNear - zFar);
#else
	self->r22 = HYP_FLOAT_C(1.0) / (zNear - zFar);
	self->r23 = zNear / (zNear - zFar);
#endif
	self->r33 = HYP_FLOAT_C(1.0);

	return self;
}


/**
 * @ingroup matrix4
 * @brief Opinionated function about what the axis means.  Sets the axis and
 * angle (used as a rotation matrix).  The axis does not need to be unit
 * length; a zero axis gives the identity.
 *
 * @param self The matrix
 * @param x The x part of the axis
 * @param y The y part of the axis
 * @param z The z part of the axis
 * @param angle the angle in radians
 *
 */
HYPAPI struct matrix4 *matrix4_set_from_axisf3_angle(struct matrix4 *self, HYP_FLOAT x, HYP_FLOAT y, HYP_FLOAT z, const HYP_FLOAT angle)
{
	HYP_FLOAT c = HYP_COS(angle);
	HYP_FLOAT s = HYP_SIN(angle);
	struct vector3 axis;

	/* the formula needs a unit axis */
	if (!(hyp_normalize(vector3_setf3(&axis, x, y, z)->v, 3) > HYP_FLOAT_C(0.0))) {
		return matrix4_identity(self);
	}
	x = axis.x;
	y = axis.y;
	z = axis.z;

	self->c00 = (x * x) * (HYP_FLOAT_C(1.0) - c) + c;
	self->c01 = (x * y) * (HYP_FLOAT_C(1.0) - c) + (z * s);
	self->c02 = (x * z) * (HYP_FLOAT_C(1.0) - c) - (y * s);
	self->c03 = HYP_FLOAT_C(0.0);

	self->c10 = (y * x) * (HYP_FLOAT_C(1.0) - c) - (z * s);
	self->c11 = (y * y) * (HYP_FLOAT_C(1.0) - c) + c;
	self->c12 = (y * z) * (HYP_FLOAT_C(1.0) - c) + (x * s);
	self->c13 = HYP_FLOAT_C(0.0);

	self->c20 = (z * x) * (HYP_FLOAT_C(1.0) - c) + (y * s);
	self->c21 = (z * y) * (HYP_FLOAT_C(1.0) - c) - (x * s);
	self->c22 = (z * z) * (HYP_FLOAT_C(1.0) - c) + c;
	self->c23 = HYP_FLOAT_C(0.0);

	self->c30 = HYP_FLOAT_C(0.0);
	self->c31 = HYP_FLOAT_C(0.0);
	self->c32 = HYP_FLOAT_C(0.0);
	self->c33 = HYP_FLOAT_C(1.0);

	return self;
}


/**
 * @ingroup matrix4
 * @brief Opinionated function about what the axis means.  Sets the axis and
 * angle (used as a rotation matrix).  The axis does not need to be unit
 * length; a zero axis gives the identity.
 *
 * @param self The matrix
 * @param axis The axis
 * @param angle the angle in radians
 *
 */
HYPAPI struct matrix4 *matrix4_set_from_axisv3_angle(struct matrix4 *self, const struct vector3 *axis, HYP_FLOAT angle)
{
	return matrix4_set_from_axisf3_angle(self, axis->x, axis->y, axis->z, angle);
}


/**
 * @ingroup matrix4
 * @brief Sets the matrix to the rotation described by the quaternion
 *
 * The whole matrix is set: the rotation, no translation, and 1 in c33.
 * Applied with vector3_multiplym4, the matrix rotates a vector the same way as
 * vector3_rotate_by_quaternion does (right hand rule), the same convention as
 * matrix4_set_from_axisf3_angle and matrix4_make_transformation_rotationq.
 *
 * The quaternion does not need to be unit length: only its direction is used.
 * A quaternion with all four components exactly zero describes no rotation and
 * gives the identity.
 *
 * @param self The matrix
 * @param qT The quaternion
 *
 */
HYPAPI struct matrix4 *matrix4_set_from_quaternion(struct matrix4 *self, const struct quaternion *qT)
{
	struct quaternion q;
	HYP_FLOAT norm = quaternion_norm(qT);
	HYP_FLOAT s;
	HYP_FLOAT xx, yy, zz, xy, xz, yz, xw, yw, zw;

	/* 2 / |q|^2 makes the result a pure rotation for any length of qT; when
	 * |q|^2 overflows or underflows, normalize q instead
	 */
	quaternion_set(&q, qT);
	if (norm > HYP_FLOAT_C(1e-30) && norm < HYP_FLOAT_C(1e30)) {
		s = HYP_FLOAT_C(2.0) / norm;
	} else if (hyp_normalize(q.q, 4) > HYP_FLOAT_C(0.0)) {
		s = HYP_FLOAT_C(2.0);
	} else {
		return matrix4_identity(self);
	}

	xx = s * q.x * q.x;
	yy = s * q.y * q.y;
	zz = s * q.z * q.z;
	xy = s * q.x * q.y;
	xz = s * q.x * q.z;
	yz = s * q.y * q.z;
	xw = s * q.x * q.w;
	yw = s * q.y * q.w;
	zw = s * q.z * q.w;

	self->c00 = HYP_FLOAT_C(1.0) - yy - zz;
	self->c01 = xy + zw;
	self->c02 = xz - yw;
	self->c03 = HYP_FLOAT_C(0.0);

	self->c10 = xy - zw;
	self->c11 = HYP_FLOAT_C(1.0) - xx - zz;
	self->c12 = yz + xw;
	self->c13 = HYP_FLOAT_C(0.0);

	self->c20 = xz + yw;
	self->c21 = yz - xw;
	self->c22 = HYP_FLOAT_C(1.0) - xx - yy;
	self->c23 = HYP_FLOAT_C(0.0);

	self->c30 = HYP_FLOAT_C(0.0);
	self->c31 = HYP_FLOAT_C(0.0);
	self->c32 = HYP_FLOAT_C(0.0);
	self->c33 = HYP_FLOAT_C(1.0);

	return self;
}


/**
 * @ingroup matrix4
 * @brief Sets the matrix to the rotation by Euler angles, in radians: about X
 * first, then Y, then Z (the order of quaternion_set_from_euler_anglesf3).
 * The whole matrix is set: no translation and 1 in r33.
 *
 * @param self The matrix
 * @param x the angle about X
 * @param y the angle about Y
 * @param z the angle about Z
 */
HYPAPI struct matrix4 *matrix4_set_from_euler_anglesf3(struct matrix4 *self, const HYP_FLOAT x, const HYP_FLOAT y, const HYP_FLOAT z)
{
	HYP_FLOAT cx = HYP_COS(x);
	HYP_FLOAT sx = HYP_SIN(x);
	HYP_FLOAT cy = HYP_COS(y);
	HYP_FLOAT sy = HYP_SIN(y);
	HYP_FLOAT cz = HYP_COS(z);
	HYP_FLOAT sz = HYP_SIN(z);

	matrix4_identity(self);

	/* Rz * Ry * Rx, applied as M * v */
	self->r00 = cz * cy;
	self->r01 = cz * sy * sx - sz * cx;
	self->r02 = cz * sy * cx + sz * sx;

	self->r10 = sz * cy;
	self->r11 = sz * sy * sx + cz * cx;
	self->r12 = sz * sy * cx - cz * sx;

	self->r20 = -sy;
	self->r21 = cy * sx;
	self->r22 = cy * cx;

	return self;
}


/**
 * @ingroup matrix4
 * @brief Gets the translation of the matrix (r03, r13, r23)
 *
 * @param self The matrix
 * @param vT The translation is returned here
 */
HYPAPI struct vector3 *matrix4_get_translation(const struct matrix4 *self, struct vector3 *vT)
{
	vT->x = self->c30;
	vT->y = self->c31;
	vT->z = self->c32;

	return vT;
}


/**
 * @ingroup matrix4
 * @brief creates a look at matrix using the RH system.
 */
HYPAPI struct matrix4 *matrix4_view_lookat_rh(struct matrix4 *self, const struct vector3 *eye, const struct vector3 *target, const struct vector3 *up)
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
	/* the third row is -zaxis */
	self->c32 = vector3_dot_product(&zaxis, eye);

	return self;
}


/**
 * @ingroup matrix4
 * @brief creates a perspective projection matrix for left-handed coordinates
 * (the camera looks down +Z), with fovy the vertical field of view in radians.
 * Depth maps to 0 at zNear and 1 at zFar (-1 and 1 with
 * HYP_DEPTH_MINUS_ONE_TO_ONE).  Apply it as M * v (matrix4_multiplyv4) and
 * divide by w.
 */
HYPAPI struct matrix4 *matrix4_projection_perspective_fovy_lh(struct matrix4 *self, HYP_FLOAT fovy, HYP_FLOAT aspect, HYP_FLOAT zNear, HYP_FLOAT zFar)
{
	HYP_FLOAT h = HYP_COT(fovy / HYP_FLOAT_C(2.0));

	matrix4_zero(self);

	self->r00 = h / aspect;
	self->r11 = h;
#ifdef HYP_DEPTH_MINUS_ONE_TO_ONE
	self->r22 = (zFar + zNear) / (zFar - zNear);
	self->r23 = -HYP_FLOAT_C(2.0) * zFar * zNear / (zFar - zNear);
#else
	self->r22 = zFar / (zFar - zNear);
	self->r23 = -zNear * zFar / (zFar - zNear);
#endif
	self->r32 = HYP_FLOAT_C(1.0); /* w = z: in front of the camera z is positive */

	return self;
}


/**
 * @ingroup matrix4
 * @brief creates a perspective projection matrix with no far plane for
 * right-handed coordinates (the camera looks down -Z): the limit of
 * matrix4_projection_perspective_fovy_rh as zFar goes to infinity.  Depth maps
 * to 0 at zNear and approaches 1 far away (-1 and 1 with
 * HYP_DEPTH_MINUS_ONE_TO_ONE).  Apply it as M * v and divide by w.
 */
HYPAPI struct matrix4 *matrix4_projection_perspective_fovy_infinite_rh(struct matrix4 *self, HYP_FLOAT fovy, HYP_FLOAT aspect, HYP_FLOAT zNear)
{
	HYP_FLOAT h = HYP_COT(fovy / HYP_FLOAT_C(2.0));

	matrix4_zero(self);

	self->r00 = h / aspect;
	self->r11 = h;
	self->r22 = -HYP_FLOAT_C(1.0);
#ifdef HYP_DEPTH_MINUS_ONE_TO_ONE
	self->r23 = -HYP_FLOAT_C(2.0) * zNear;
#else
	self->r23 = -zNear;
#endif
	self->r32 = -HYP_FLOAT_C(1.0); /* w = -z */

	return self;
}


/**
 * @ingroup matrix4
 * @brief creates a perspective projection matrix with no far plane for
 * left-handed coordinates (the camera looks down +Z): the limit of
 * matrix4_projection_perspective_fovy_lh as zFar goes to infinity.  Depth maps
 * to 0 at zNear and approaches 1 far away (-1 and 1 with
 * HYP_DEPTH_MINUS_ONE_TO_ONE).  Apply it as M * v and divide by w.
 */
HYPAPI struct matrix4 *matrix4_projection_perspective_fovy_infinite_lh(struct matrix4 *self, HYP_FLOAT fovy, HYP_FLOAT aspect, HYP_FLOAT zNear)
{
	HYP_FLOAT h = HYP_COT(fovy / HYP_FLOAT_C(2.0));

	matrix4_zero(self);

	self->r00 = h / aspect;
	self->r11 = h;
	self->r22 = HYP_FLOAT_C(1.0);
#ifdef HYP_DEPTH_MINUS_ONE_TO_ONE
	self->r23 = -HYP_FLOAT_C(2.0) * zNear;
#else
	self->r23 = -zNear;
#endif
	self->r32 = HYP_FLOAT_C(1.0); /* w = z */

	return self;
}


/**
 * @ingroup matrix4
 * @brief creates a perspective projection matrix for right-handed coordinates
 * (the camera looks down -Z) from the bounds of the view at the near plane,
 * which need not be centered (off-axis projections).  xmin..xmax and
 * ymin..ymax map to -1..1; depth maps to 0 at zNear and 1 at zFar (-1 and 1
 * with HYP_DEPTH_MINUS_ONE_TO_ONE).  With centered bounds it is the same as
 * matrix4_projection_perspective_fovy_rh.  Apply it as M * v and divide by w.
 */
HYPAPI struct matrix4 *matrix4_projection_frustum_rh(struct matrix4 *self, HYP_FLOAT xmin, HYP_FLOAT xmax, HYP_FLOAT ymin, HYP_FLOAT ymax, HYP_FLOAT zNear, HYP_FLOAT zFar)
{
	HYP_FLOAT width = xmax - xmin;
	HYP_FLOAT height = ymax - ymin;

	matrix4_zero(self);

	self->r00 = HYP_FLOAT_C(2.0) * zNear / width;
	self->r02 = (xmax + xmin) / width;
	self->r11 = HYP_FLOAT_C(2.0) * zNear / height;
	self->r12 = (ymax + ymin) / height;
#ifdef HYP_DEPTH_MINUS_ONE_TO_ONE
	self->r22 = (zFar + zNear) / (zNear - zFar);
	self->r23 = HYP_FLOAT_C(2.0) * zFar * zNear / (zNear - zFar);
#else
	self->r22 = zFar / (zNear - zFar);
	self->r23 = zNear * zFar / (zNear - zFar);
#endif
	self->r32 = -HYP_FLOAT_C(1.0); /* w = -z */

	return self;
}


/**
 * @ingroup matrix4
 * @brief creates a perspective projection matrix for left-handed coordinates
 * (the camera looks down +Z) from the bounds of the view at the near plane,
 * which need not be centered (off-axis projections).  xmin..xmax and
 * ymin..ymax map to -1..1; depth maps to 0 at zNear and 1 at zFar (-1 and 1
 * with HYP_DEPTH_MINUS_ONE_TO_ONE).  With centered bounds it is the same as
 * matrix4_projection_perspective_fovy_lh.  Apply it as M * v and divide by w.
 */
HYPAPI struct matrix4 *matrix4_projection_frustum_lh(struct matrix4 *self, HYP_FLOAT xmin, HYP_FLOAT xmax, HYP_FLOAT ymin, HYP_FLOAT ymax, HYP_FLOAT zNear, HYP_FLOAT zFar)
{
	HYP_FLOAT width = xmax - xmin;
	HYP_FLOAT height = ymax - ymin;

	matrix4_zero(self);

	self->r00 = HYP_FLOAT_C(2.0) * zNear / width;
	self->r02 = -(xmax + xmin) / width;
	self->r11 = HYP_FLOAT_C(2.0) * zNear / height;
	self->r12 = -(ymax + ymin) / height;
#ifdef HYP_DEPTH_MINUS_ONE_TO_ONE
	self->r22 = (zFar + zNear) / (zFar - zNear);
	self->r23 = -HYP_FLOAT_C(2.0) * zFar * zNear / (zFar - zNear);
#else
	self->r22 = zFar / (zFar - zNear);
	self->r23 = -zNear * zFar / (zFar - zNear);
#endif
	self->r32 = HYP_FLOAT_C(1.0); /* w = z */

	return self;
}


/**
 * @ingroup matrix4
 * @brief Maps the point self to window coordinates: transform (projection *
 * view * model, applied as M * v), the divide by w, then the viewport (x, y,
 * width, height).  Window x and y grow right and up from (x, y); window depth
 * is 0 at the near plane and 1 at the far plane with either depth convention.
 *
 * Returns NULL, and leaves self unchanged, when w is exactly 0 (a point in
 * the plane of the eye).
 */
HYPAPI struct vector3 *vector3_project_to_window(struct vector3 *self, const struct matrix4 *transform, const struct vector4 *viewport)
{
	struct vector4 point;
	struct vector4 clip;

	vector4_setf4(&point, self->x, self->y, self->z, HYP_FLOAT_C(1.0));
	matrix4_multiplyv4(transform, &point, &clip);
	/* w exactly 0 (written without == for -Wfloat-equal; NaN also returns NULL) */
	if (!(clip.w < HYP_FLOAT_C(0.0)) && !(clip.w > HYP_FLOAT_C(0.0))) {
		return NULL;
	}

	self->x = viewport->x + viewport->z * (clip.x / clip.w + HYP_FLOAT_C(1.0)) / HYP_FLOAT_C(2.0);
	self->y = viewport->y + viewport->w * (clip.y / clip.w + HYP_FLOAT_C(1.0)) / HYP_FLOAT_C(2.0);
#ifdef HYP_DEPTH_MINUS_ONE_TO_ONE
	self->z = (clip.z / clip.w + HYP_FLOAT_C(1.0)) / HYP_FLOAT_C(2.0);
#else
	self->z = clip.z / clip.w;
#endif

	return self;
}


/**
 * @ingroup matrix4
 * @brief Maps window coordinates back to a point: the inverse of
 * vector3_project_to_window for the same transform and viewport.  A window
 * depth of 0 gives the point on the near plane, 1 the point on the far plane.
 *
 * Returns NULL, and leaves self unchanged, when transform has no inverse or the
 * point is at infinity (w exactly 0).
 */
HYPAPI struct vector3 *vector3_unproject_from_window(struct vector3 *self, const struct matrix4 *transform, const struct vector4 *viewport)
{
	struct matrix4 inverse;
	struct vector4 ndc;
	struct vector4 point;

	if (matrix4_inverse(transform, &inverse) == NULL) {
		return NULL;
	}

	ndc.x = HYP_FLOAT_C(2.0) * (self->x - viewport->x) / viewport->z - HYP_FLOAT_C(1.0);
	ndc.y = HYP_FLOAT_C(2.0) * (self->y - viewport->y) / viewport->w - HYP_FLOAT_C(1.0);
#ifdef HYP_DEPTH_MINUS_ONE_TO_ONE
	ndc.z = HYP_FLOAT_C(2.0) * self->z - HYP_FLOAT_C(1.0);
#else
	ndc.z = self->z;
#endif
	ndc.w = HYP_FLOAT_C(1.0);

	matrix4_multiplyv4(&inverse, &ndc, &point);
	/* w exactly 0 (written without == for -Wfloat-equal; NaN also returns NULL) */
	if (!(point.w < HYP_FLOAT_C(0.0)) && !(point.w > HYP_FLOAT_C(0.0))) {
		return NULL;
	}

	return vector3_setf3(self, point.x / point.w, point.y / point.w, point.z / point.w);
}


/**
 * @ingroup matrix4
 * @brief make an orthographic projection matrix for left-handed coordinates
 * (the camera looks down +Z).  The box maps to -1..1 in x and y; depth maps
 * to 0 at zNear and 1 at zFar (-1 and 1 with HYP_DEPTH_MINUS_ONE_TO_ONE).
 * Apply it as M * v.
 */
HYPAPI struct matrix4 *matrix4_projection_ortho3d_lh(struct matrix4 *self,
							 HYP_FLOAT xmin, HYP_FLOAT xmax,
							 HYP_FLOAT ymin, HYP_FLOAT ymax,
							 HYP_FLOAT zNear, HYP_FLOAT zFar)
{
	HYP_FLOAT width = xmax - xmin;
	HYP_FLOAT height = ymax - ymin;

	matrix4_zero(self);

	self->r00 = HYP_FLOAT_C(2.0) / width;
	self->r03 = -(xmax + xmin) / width;
	self->r11 = HYP_FLOAT_C(2.0) / height;
	self->r13 = -(ymax + ymin) / height;
#ifdef HYP_DEPTH_MINUS_ONE_TO_ONE
	self->r22 = HYP_FLOAT_C(2.0) / (zFar - zNear);
	self->r23 = -(zFar + zNear) / (zFar - zNear);
#else
	self->r22 = HYP_FLOAT_C(1.0) / (zFar - zNear);
	self->r23 = -zNear / (zFar - zNear);
#endif
	self->r33 = HYP_FLOAT_C(1.0);

	return self;
}


/**
 * @ingroup matrix4
 * @brief creates a view matrix for left-handed coordinates: the eye goes to
 * the origin and looks down +Z, with up toward +Y.  Apply it as M * v.
 */
HYPAPI struct matrix4 *matrix4_view_lookat_lh(struct matrix4 *self, const struct vector3 *eye, const struct vector3 *target, const struct vector3 *up)
{
	struct vector3 xaxis;
	struct vector3 yaxis;
	struct vector3 zaxis;

	zaxis.x = target->x - eye->x;
	zaxis.y = target->y - eye->y;
	zaxis.z = target->z - eye->z;
	vector3_normalize(&zaxis);

	/* xaxis = up x zaxis */
	vector3_cross_product(&xaxis, up, &zaxis);
	vector3_normalize(&xaxis);

	/* yaxis = zaxis x xaxis */
	vector3_cross_product(&yaxis, &zaxis, &xaxis);

	matrix4_identity(self);

	/* the rows are the camera axes */
	self->r00 = xaxis.x;
	self->r01 = xaxis.y;
	self->r02 = xaxis.z;
	self->r03 = -vector3_dot_product(&xaxis, eye);

	self->r10 = yaxis.x;
	self->r11 = yaxis.y;
	self->r12 = yaxis.z;
	self->r13 = -vector3_dot_product(&yaxis, eye);

	self->r20 = zaxis.x;
	self->r21 = zaxis.y;
	self->r22 = zaxis.z;
	self->r23 = -vector3_dot_product(&zaxis, eye);

	return self;
}


/**
 * @ingroup matrix4
 * @brief Sets self to the rotation by the Euler angles in vR (radians), as
 * matrix4_set_from_euler_anglesf3: about X first, then Y, then Z.
 *
 * @param self The matrix
 * @param vR the euler angles
 *
 */
HYPAPI struct matrix4 *matrix4_make_transformation_rotationv3(struct matrix4 *self, const struct vector3 *vR)
{
	return matrix4_set_from_euler_anglesf3(self, vR->x, vR->y, vR->z);
}


/**
 * @ingroup matrix4
 * @brief Sets the matrix to scale, then rotate, then translate (a TRS
 * matrix), applied as M * v.  matrix4_transformation_decompose splits it back.
 *
 * @param self The matrix
 * @param scale The scale
 * @param rotation The rotation, as a unit quaternion
 * @param translation The translation
 */
HYPAPI struct matrix4 *matrix4_transformation_compose(struct matrix4 *self, const struct vector3 *scale, const struct quaternion *rotation, const struct vector3 *translation)
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

/**
 * @ingroup matrix4
 * @brief Splits a matrix made by matrix4_transformation_compose (scale,
 * then rotate, then translate) back into its parts.  A mirror (a negative
 * determinant) comes back as a negative x scale.  Returns 0, with the identity
 * rotation, when a scale is zero and the rotation cannot be recovered.
 * Shear is not supported.
 *
 * @param self The matrix
 * @param scale The scale
 * @param rotation The rotation, as a unit quaternion
 * @param translation The translation
 */
HYPAPI uint8_t matrix4_transformation_decompose(struct matrix4 *self, struct vector3 *scale, struct quaternion *rotation, struct vector3 *translation)
{
	struct matrix4 unscaled;
	struct vector3 x, y, z;
	struct vector3 cross;

	/* translation */
	translation->x = self->r03;
	translation->y = self->r13;
	translation->z = self->r23;

	/* each column of the 3x3 is a rotated axis times its scale */
	vector3_setf3(&x, self->r00, self->r10, self->r20);
	vector3_setf3(&y, self->r01, self->r11, self->r21);
	vector3_setf3(&z, self->r02, self->r12, self->r22);
	scale->x = hyp_normalize(x.v, 3);
	scale->y = hyp_normalize(y.v, 3);
	scale->z = hyp_normalize(z.v, 3);

	if (!(scale->x > HYP_FLOAT_C(0.0)) || !(scale->y > HYP_FLOAT_C(0.0)) || !(scale->z > HYP_FLOAT_C(0.0))) {
		quaternion_identity(rotation);
		return 0;
	}

	/* a negative determinant is a mirror: put it on x */
	if (vector3_dot_product(&x, vector3_cross_product(&cross, &y, &z)) < HYP_FLOAT_C(0.0)) {
		scale->x = -scale->x;
		vector3_negate(&x);
	}

	/* the rotation matrix: the unit columns */
	matrix4_identity(&unscaled);
	unscaled.r00 = x.x;
	unscaled.r10 = x.y;
	unscaled.r20 = x.z;
	unscaled.r01 = y.x;
	unscaled.r11 = y.y;
	unscaled.r21 = y.z;
	unscaled.r02 = z.x;
	unscaled.r12 = z.y;
	unscaled.r22 = z.z;

	quaternion_set_from_matrix4(rotation, &unscaled);

	return 1;
}

#undef HYP_CAT
#undef HYP_PRIMITIVE_CAT
#undef HYP_DEC
#undef HYP_DEC_11
#undef HYP_DEC_12
#undef HYP_DEC_13
#undef HYP_DEC_14
#undef HYP_DEC_21
#undef HYP_DEC_22
#undef HYP_DEC_23
#undef HYP_DEC_24
#undef HYP_DEC_31
#undef HYP_DEC_32
#undef HYP_DEC_33
#undef HYP_DEC_34
#undef HYP_DEC_41
#undef HYP_DEC_42
#undef HYP_DEC_43
#undef HYP_DEC_44
#undef HYP_A
#undef HYP_B
#undef HYP_A2

#endif /* HYPATIA_IMPLEMENTATION_H_ */
#endif /* HYPATIA_IMPLEMENTATION */
