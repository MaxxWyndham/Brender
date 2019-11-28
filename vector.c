#include "brender.h"
#include "vector.h"

br_scalar BrFVector3Dot(const br_fvector3* v1, const br_fvector3* v2)
{
	FIXME("(%p, %p): stub", v1, v2);
	return 0;
}

void BrVector2Sub(br_vector2* v1, const br_vector2* v2, const br_vector2* v3)
{
	static br_vector2 t;

	t.v[0] = BR_SUB(v2->v[0], v3->v[0]);
	t.v[1] = BR_SUB(v2->v[1], v3->v[1]);

	BrVector2Copy(v1, &t);
}

void BrVector2Normalise(br_vector2* v1, const br_vector2* v2)
{
	static br_vector2 t;

	if (v2->v[0] == 0 && v2->v[1] == 0)
	{
        t.v[0] = 1;
        t.v[1] = 0;
	}
	else
	{
	    br_scalar len = BrVector2Length(v2);
        t.v[0] = BR_DIV(v2->v[0], len);
        t.v[1] = BR_DIV(v2->v[1], len);
	}

	BrVector2Copy(v1, &t);
}

void BrVector2Accumulate(br_vector2* v1, const br_vector2* v2)
{
	static br_vector2 t;

	BrVector2Add(&t, v1, v2);

	BrVector2Copy(v1, &t);
}

void BrVector2InvScale(br_vector2* v1, const br_vector2* v2, br_scalar s)
{
	static br_vector2 t;

	s = BR_RCP(s);
	t.v[0] = BR_MUL(v2->v[0], s);
	t.v[1] = BR_MUL(v2->v[1], s);

	BrVector2Copy(v1, &t);
}

void BrVector2Add(br_vector2* v1, const br_vector2* v2, const br_vector2* v3)
{
	static br_vector2 t;

	t.v[0] = BR_ADD(v2->v[0], v3->v[0]);
	t.v[1] = BR_ADD(v2->v[1], v3->v[1]);

	BrVector2Copy(v1, &t);
}

br_scalar BrVector2Length(const br_vector2* v1)
{
	return BR_SQRT(BrVector2LengthSquared(v1));
}

br_scalar BrVector2Dot(const br_vector2* v1, const br_vector2* v2)
{
	return BR_MAC2(v1->v[0], v2->v[0], v1->v[1], v2->v[1]);
}

br_scalar BrVector2LengthSquared(const br_vector2* v1)
{
	return BR_MAC2(v1->v[0], v1->v[0], v1->v[1], v1->v[1]);
}

void BrVector2SetFloat(br_vector2* v1, float f1, float f2)
{
	v1->v[0] = BrFloatToScalar(f1);
	v1->v[1] = BrFloatToScalar(f2);
}

void BrVector2Scale(br_vector2* v1, const br_vector2* v2, br_scalar s)
{
	static br_vector2 t;

	t.v[0] = BR_MUL(v2->v[0], s);
	t.v[1] = BR_MUL(v2->v[1], s);

	BrVector2Copy(v1, &t);
}

void BrVector2Set(br_vector2* v1, br_scalar s1, br_scalar s2)
{
    v1->v[0] = s1;
    v1->v[1] = s2;
}

void BrVector2SetInt(br_vector2* v1, int i1, int i2)
{
	BrVector2Set(v1, i1, i2);
}

void BrVector2Copy(br_vector2* v1, const br_vector2* v2)
{
	BrBlockCopy(v1, v2, sizeof(br_vector2));
}

void BrVector3Negate(br_vector3* v1, const br_vector3* v2)
{
	static br_vector3 t;

	BrVector3Scale(&t, v2, -1.0);

	BrVector3Copy(v1, &t);
}

void BrVector3Add(br_vector3* v1, const br_vector3* v2, const br_vector3* v3)
{
	static br_vector3 t;

	t.v[0] = BR_ADD(v2->v[0], v3->v[0]);
	t.v[1] = BR_ADD(v2->v[1], v3->v[1]);
	t.v[2] = BR_ADD(v2->v[2], v3->v[2]);

	BrVector3Copy(v1, &t);
}

void BrVector3Sub(br_vector3* v1, const br_vector3* v2, const br_vector3* v3)
{
	static br_vector3 t;

	t.v[0] = BR_SUB(v2->v[0], v3->v[0]);
	t.v[1] = BR_SUB(v2->v[1], v3->v[1]);
	t.v[2] = BR_SUB(v2->v[2], v3->v[2]);

	BrVector3Copy(v1, &t);
}

void BrVector3Set(br_vector3* v1, br_scalar s1, br_scalar s2, br_scalar s3)
{
    v1->v[0] = s1;
    v1->v[1] = s2;
    v1->v[2] = s3;
}

void BrVector3SetFloat(br_vector3* v1, float f1, float f2, float f3)
{
	v1->v[0] = BrFloatToScalar(f1);
	v1->v[1] = BrFloatToScalar(f2);
	v1->v[2] = BrFloatToScalar(f3);
}

void BrVector3SetInt(br_vector3* v1, int i1, int i2, int i3)
{
	v1->v[0] = i1;
	v1->v[1] = i2;
	v1->v[2] = i3;
}

void BrVector3Copy(br_vector3* v1, const br_vector3* v2)
{
	BrBlockCopy(v1, v2, sizeof(br_vector3));
}

void BrVector3Cross(br_vector3* v1, const br_vector3* v2, const br_vector3* v3)
{
	static br_vector3 t;

	t.v[0] = (v2->v[1] * v3->v[2]) - (v2->v[2] * v3->v[1]);
	t.v[1] = (v2->v[2] * v3->v[0]) - (v2->v[0] * v3->v[2]);
	t.v[2] = (v2->v[0] * v3->v[1]) - (v2->v[1] * v3->v[0]);

	BrVector3Copy(v1, &t);
}

void BrVector3Scale(br_vector3* v1, const br_vector3* v2, br_scalar s)
{
	static br_vector3 t;

    t.v[0] = BR_MUL(v2->v[0], s);
	t.v[1] = BR_MUL(v2->v[1], s);
	t.v[2] = BR_MUL(v2->v[2], s);

	BrVector3Copy(v1, &t);
}

void BrVector3InvScale(br_vector3* v1, const br_vector3* v2, br_scalar s)
{
	static br_vector3 t;

	s = BR_RCP(s);
	t.v[0] = BR_MUL(v2->v[0], s);
	t.v[1] = BR_MUL(v2->v[1], s);
	t.v[2] = BR_MUL(v2->v[2], s);

	BrVector3Copy(v1, &t);
}

void BrVector3Normalise(br_vector3* v1, const br_vector3* v2)
{
	static br_vector3 t;

	if (v2->v[0] == 0 && v2->v[1] == 0 && v2->v[2] == 0)
	{
        t.v[0] = 1;
        t.v[1] = 0;
        t.v[2] = 0;
	}
	else
	{
	    br_scalar len = BrVector3Length(v2);
        t.v[0] = BR_DIV(v2->v[0], len);
        t.v[1] = BR_DIV(v2->v[1], len);
        t.v[2] = BR_DIV(v2->v[2], len);
	}

	BrVector3Copy(v1, &t);
}

void BrVector3NormaliseQuick(br_vector3* v1, const br_vector3* v2)
{
	static br_vector3 t;

    br_scalar len = BrVector3Length(v2);
	t.v[0] = BR_DIV(v2->v[0], len);
	t.v[1] = BR_DIV(v2->v[1], len);
	t.v[2] = BR_DIV(v2->v[2], len);

	BrVector3Copy(v1, &t);
}

void BrVector3Accumulate(br_vector3* v1, const br_vector3* v2)
{
	static br_vector3 t;

	BrVector3Add(&t, v1, v2);

	BrVector3Copy(v1, &t);
}

br_scalar BrVector3Dot(const br_vector3* v1, const br_vector3* v2)
{
	return BR_MAC3(v1->v[0], v2->v[0], v1->v[1], v2->v[1], v1->v[2], v2->v[2]);
}

br_scalar BrVector3Length(const br_vector3* v1)
{
	return BR_SQRT(BrVector3LengthSquared(v1));
}

br_scalar BrVector3LengthSquared(const br_vector3* v1)
{
	return BR_MAC3(v1->v[0], v1->v[0], v1->v[1], v1->v[1], v1->v[2], v1->v[2]);
}

char* BrVector3ToString(const br_vector3* v)
{
	char *buffer = BrMemAllocate(50, BR_MEMORY_STRING);
	sprintf(buffer, "{X:%g Y:%g Z:%g}", v->v[0], v->v[1], v->v[2]);
	return buffer;
}

void BrVector4Copy(br_vector4* v1, const br_vector4* v2)
{
	BrBlockCopy(v1, v2, sizeof(br_vector4));
}
