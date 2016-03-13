/**
 * @file hypatia.h
 *
*/

#ifndef _INC_HYPATIA
#define _INC_HYPATIA

/* start with HYPAPI being off */
#ifndef HYPAPI
#	define HYPAPI
#endif


#include "config.h"


#if defined(_WIN32) || defined(_WIN64) || defined(_MSC_VER) || defined(WIN32)
#	undef HYPAPI
#	define HYPAPI __declspec(dllexport)
	//#ifndef HYP_EXPORTS_ON
	//#else
	//	#define HYPAPI __declspec(dllimport)
	//#endif
#else
#	undef HYPAPI
#	define HYPAPI
#endif

#ifdef HYPATIA_USE_SINGLE
#	define HYP_FLOAT float
#else
#	define HYP_FLOAT double
#endif

#include <math.h> /* sin, cos, acos */
#include <stdlib.h> /* RAND_MAX, rand */

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
/** @brief Epsilon.  This is the value that is used to determine how much rounding error is tolerated. */
#ifdef HYPATIA_USE_SINGLE
#	define HYP_EPSILON 1E-5
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
#define HYP_RANDOM_FLOAT (((HYP_FLOAT)rand()-(HYP_FLOAT)rand())/RAND_MAX)

/** @brief A macro that converts an angle in degress to an angle in radians */
#define HYP_DEG_TO_RAD(angle)  ((angle)*HYP_RAD_PER_DEG)

/** @brief A macro that converts an angle in radians to an angle in degrees */
#define HYP_RAD_TO_DEG(radians) ((radians)*HYP_DEG_PER_RAD)

/** @brief A macro that squares a value squared */
#define HYP_SQUARE(number) (number*number)

/** @brief A macro that returns the absolute value */
#define HYP_ABS(value) (((value) < 0) ? -(value) : (value))

/** @brief A macro that wraps a value around and around in a range */
#define HYP_WRAP(value, start, limit) (value = fmod(start + (value - start), (limit - start)))

/** @brief A macro that constrains the value between two limits \a a and \a b */
#define HYP_CLAMP(value, start, limit) (value = ( ( value < start ) ? start : ( value > limit ) ? limit : value ))

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/* forward declarations */
typedef struct tagvector2 vector2;
typedef struct tagvector3 vector3;
typedef struct tagvector4 vector4;
typedef struct tagmatrix3 matrix3;
typedef struct tagmatrix4 matrix4;
typedef struct tagquaternion quaternion;

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

#include "vector2.h"
#include "vector3.h"
#include "vector4.h"
#include "matrix3.h"
#include "matrix4.h"
#include "quaternion.h"
#include "experimental.h"

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#define HYP_REF_VECTOR2_ZERO 0
#define HYP_REF_VECTOR2_UNIT_X 1
#define HYP_REF_VECTOR2_UNIT_Y 2
#define HYP_REF_VECTOR2_UNIT_X_NEGATIVE 3
#define HYP_REF_VECTOR2_UNIT_Y_NEGATIVE 4
#define HYP_REF_VECTOR2_ONE 5

HYPAPI const vector2* vector2_get_reference_vector2(int id);

#define HYP_REF_VECTOR3_ZERO 0
#define HYP_REF_VECTOR3_UNIT_X 1
#define HYP_REF_VECTOR3_UNIT_Y 2
#define HYP_REF_VECTOR3_UNIT_Z 3
#define HYP_REF_VECTOR3_UNIT_X_NEGATIVE 4
#define HYP_REF_VECTOR3_UNIT_Y_NEGATIVE 5
#define HYP_REF_VECTOR3_UNIT_Z_NEGATIVE 6
#define HYP_REF_VECTOR3_ONE 7

HYPAPI const vector3* vector3_get_reference_vector3(int id);

#define HYP_REF_VECTOR4_ZERO 0
#define HYP_REF_VECTOR4_UNIT_X 1
#define HYP_REF_VECTOR4_UNIT_Y 2
#define HYP_REF_VECTOR4_UNIT_Z 3
#define HYP_REF_VECTOR4_UNIT_X_NEGATIVE 4
#define HYP_REF_VECTOR4_UNIT_Y_NEGATIVE 5
#define HYP_REF_VECTOR4_UNIT_Z_NEGATIVE 6
#define HYP_REF_VECTOR4_ONE 7

HYPAPI const vector4* vector4_get_reference_vector4(int id);

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

/**
 * @defgroup _reference_vectors3 Reference Vectors
 * @ingroup _vector3
 * These are the reference vectors.
 *
 * They can be used like so:
 * @code
 * vector3 x_axis;
 * vector3_set(&x_axis, HYP_VECTOR3_UNIT_X);
 * @endcode
 *
 * @{
 */

/** @brief {0,0,0} */
#define HYP_VECTOR3_ZERO vector3_get_reference_vector3(HYP_REF_VECTOR3_ZERO)
/** @brief {1,0,0} */
#define HYP_VECTOR3_UNIT_X vector3_get_reference_vector3(HYP_REF_VECTOR3_UNIT_X)
/** @brief {0,1,0} */
#define HYP_VECTOR3_UNIT_Y vector3_get_reference_vector3(HYP_REF_VECTOR3_UNIT_Y)
/** @brief {0,0,1} */
#define HYP_VECTOR3_UNIT_Z vector3_get_reference_vector3(HYP_REF_VECTOR3_UNIT_Z)
/** @brief {-1,0,0} */
#define HYP_VECTOR3_UNIT_X_NEGATIVE vector3_get_reference_vector3(HYP_REF_VECTOR3_UNIT_X_NEGATIVE)
/** @brief {0,-1,0} */
#define HYP_VECTOR3_UNIT_Y_NEGATIVE vector3_get_reference_vector3(HYP_REF_VECTOR3_UNIT_Y_NEGATIVE)
/** @brief {0,0,-1} */
#define HYP_VECTOR3_UNIT_Z_NEGATIVE vector3_get_reference_vector3(HYP_REF_VECTOR3_UNIT_Z_NEGATIVE)
/** @brief {1,1,1} */
#define HYP_VECTOR3_ONE vector3_get_reference_vector3(HYP_REF_VECTOR3_ONE)
/* @} */


/**
 * @defgroup _reference_vectors2 Reference Vectors
 * @ingroup _vector2
 * These are the reference vectors.
 *
 * They can be used like so:
 * @code
 * vector2 x_axis;
 * vector2_set(&x_axis, HYP_VECTOR2_UNIT_X);
 * @endcode
 *
 * @{
 */

/** @brief {0,0} */
#define HYP_VECTOR2_ZERO vector2_get_reference_vector2(HYP_REF_VECTOR2_ZERO)
/** @brief {1,0} */
#define HYP_VECTOR2_UNIT_X vector2_get_reference_vector2(HYP_REF_VECTOR2_UNIT_X)
/** @brief {0,1} */
#define HYP_VECTOR2_UNIT_Y vector2_get_reference_vector2(HYP_REF_VECTOR2_UNIT_Y)
/** @brief {-1,0} */
#define HYP_VECTOR2_UNIT_X_NEGATIVE vector2_get_reference_vector2(HYP_REF_VECTOR2_UNIT_X_NEGATIVE)
/** @brief {0,-1} */
#define HYP_VECTOR2_UNIT_Y_NEGATIVE vector2_get_reference_vector2(HYP_REF_VECTOR2_UNIT_Y_NEGATIVE)
/** @brief {1,1} */
#define HYP_VECTOR2_ONE vector2_get_reference_vector2(HYP_REF_VECTOR2_ONE)
/* @} */


HYPAPI short scalar_equals(const HYP_FLOAT f1, const HYP_FLOAT f2);
#define scalar_equalsf scalar_equals



#endif /* _INC_HYPATIA */
