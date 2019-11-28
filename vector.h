#include "scalar.h"

#ifndef _BRENDER_VECTOR_H_

typedef struct br_vector2
{
	br_scalar v[2];
} br_vector2;

typedef struct br_vector3
{
	br_scalar v[3];
} br_vector3;

typedef struct br_vector4
{
	br_scalar v[4];
} br_vector4;

typedef struct br_fvector3
{
	br_fraction v[3];
} br_fvector3;

typedef struct
{
	br_vector3 min;
	br_vector3 max;
} br_bounds;

br_scalar BrFVector3Dot(const br_fvector3* v1, const br_fvector3* v2);

void BrVector2Sub(br_vector2* v1, const br_vector2* v2, const br_vector2* v3);
br_scalar BrVector2LengthSquared(const br_vector2* v1);
void BrVector2SetFloat(br_vector2* v1, float f1, float f2);
void BrVector2Scale(br_vector2* v1, const br_vector2* v2, br_scalar s);
void BrVector2Set(br_vector2* v1, br_scalar s1, br_scalar s2);
void BrVector2SetInt(br_vector2* v1, int i1, int i2);
void BrVector2Copy(br_vector2* v1, const br_vector2* v2);
void BrVector2Normalise(br_vector2* v1, const br_vector2* v2);
void BrVector2Accumulate(br_vector2* v1, const br_vector2* v2);
void BrVector2InvScale(br_vector2* v1, const br_vector2* v2, br_scalar s);
br_scalar BrVector2Dot(const br_vector2* v1, const br_vector2* v2);
br_scalar BrVector2Length(const br_vector2* v1);
void BrVector2Add(br_vector2* v1, const br_vector2* v2, const br_vector2* v3);

void BrVector3Negate(br_vector3* v1, const br_vector3* v2);
void BrVector3Add(br_vector3* v1, const br_vector3* v2, const br_vector3* v3);
void BrVector3Sub(br_vector3* v1, const br_vector3* v2, const br_vector3* v3);
void BrVector3Set(br_vector3* v1, br_scalar s1, br_scalar s2, br_scalar s3);
void BrVector3SetFloat(br_vector3* v1, float f1, float f2, float f3);
void BrVector3SetInt(br_vector3* v1, int i1, int i2, int i3);
void BrVector3Copy(br_vector3* v1, const br_vector3* v2);
void BrVector3Cross(br_vector3* v1, const br_vector3* v2, const br_vector3* v3);
void BrVector3Scale(br_vector3* v1, const br_vector3* v2, br_scalar s);
void BrVector3InvScale(br_vector3* v1, const br_vector3* v2, br_scalar s);
void BrVector3Normalise(br_vector3* v1, const br_vector3* v2);
void BrVector3NormaliseQuick(br_vector3* v1, const br_vector3* v2);
void BrVector3Accumulate(br_vector3* v1, const br_vector3* v2);
br_scalar BrVector3Dot(const br_vector3* v1, const br_vector3* v2);
br_scalar BrVector3Length(const br_vector3* v1);
br_scalar BrVector3LengthSquared(const br_vector3* v1);

char* BrVector3ToString(const br_vector3* v);

void BrVector4Copy(br_vector4* v1, const br_vector4* v2);

#define BR_VECTOR3(a,b,c) { { a, b, c } }

#define BR_VECTOR4(a,b,c,d) { { a, b, c, d } }

#define _BRENDER_VECTOR_H_
#endif
