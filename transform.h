#include "compiler.h"
#include "vector.h"
#include "angles.h"
#include "quat.h"

#ifndef _BRENDER_TRANSFORM_H_
#define _BRENDER_TRANSFORM_H_

struct br_transform;
typedef struct br_transform br_transform;

#include "matrix.h"

enum {
	BR_TRANSFORM_IDENTITY,
	BR_TRANSFORM_TRANSLATION,
	BR_TRANSFORM_EULER,
	BR_TRANSFORM_LOOK_UP,
	BR_TRANSFORM_QUAT,
	BR_TRANSFORM_MATRIX34,
	BR_TRANSFORM_MATRIX34_LP
};

struct br_transform
{
	br_uint_16 type;

	union t
	{
		br_matrix34 mat;

		struct translate
		{
			br_vector3 x;
			br_vector3 y;
			br_vector3 z;
			br_vector3 t;
		} translate;

		struct euler
		{
			br_vector3 t;
			br_euler e;
		} euler;

		struct look_up
		{
			br_vector3 t;
			br_vector3 look;
			br_vector3 up;
		} look_up;

		struct quat
		{
			br_vector3 t;
			br_quat q;
		} quat;
	} t;
};

void BrMatrix34Transform(br_matrix34* A, const br_transform* xform);
void BrTransformToMatrix34(br_matrix34* mat, const br_transform* xform);
void BrTransformToTransform(br_transform* dest, const br_transform* src);

#endif
