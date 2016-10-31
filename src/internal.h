#ifndef _INC_HYPATIA_INTERNAL
#define _INC_HYPATIA_INTERNAL


#include <stdio.h>
#include <memory.h>
#include <math.h>
#include <stdint.h>

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


#define HYP_SIN(x) ((HYP_FLOAT)sin(x))
#define HYP_COS(x) ((HYP_FLOAT)cos(x))
#define HYP_TAN(x) ((HYP_FLOAT)tan(x))
#define HYP_ASIN(x) ((HYP_FLOAT)asin(x))
#define HYP_ACOS(x) ((HYP_FLOAT)acos(x))
#define HYP_ATAN2(y, x) ((HYP_FLOAT)atan2(y, x))
#define HYP_COT(a) (1.0f / HYP_TAN(a))

#define UNUSED_VARIABLE(x) (void)(x)

#define _SWAP(x, y) do { tmp = x; x = y; y = tmp; } while (0)


#endif /* _INC_HYPATIA_INTERNAL */
