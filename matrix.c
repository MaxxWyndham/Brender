#include "brender.h"
#include "matrix.h"

static br_matrix34				br_tmp_m34_1;
static br_matrix34				br_tmp_m34_2;

br_matrix34* BrEulerToMatrix34(br_matrix34* mat, const br_euler* euler)
{
	FIXME("(%p, %p): stub", mat, euler);
	return NULL;
}

br_matrix34* BrQuatToMatrix34(br_matrix34* mat, const br_quat* q)
{
	FIXME("(%p, %p): stub", mat, q);
	return NULL;
}

/************************* Matrix23 ************************/

void BrMatrix23Identity(br_matrix23* mat)
{
	mat->m[0][0] = 1; mat->m[0][1] = 0;
	mat->m[1][0] = 0; mat->m[1][1] = 1;
	mat->m[2][0] = 0; mat->m[2][1] = 0;
}

void BrMatrix23Mul(br_matrix23* A, const br_matrix23* B, const br_matrix23* C)
{
	FIXME("(): stub");
}

void BrMatrix23Copy(br_matrix23* A, const br_matrix23* B)
{
	FIXME("(): stub");
}

void BrMatrix23PostScale(br_matrix23* mat, br_scalar sx, br_scalar sy)
{
	FIXME("(): stub");
}

/************************* Matrix34 ************************/

void BrMatrix34Copy(br_matrix34* A, const br_matrix34* B)
{
	BrBlockCopy(A, B, sizeof(br_matrix34));
}

void BrMatrix34Mul(br_matrix34* A, const br_matrix34* B, const br_matrix34* C)
{
	static br_matrix34 t;

	t.m[0][0] = BR_MAC3(B->m[0][0], C->m[0][0], B->m[0][1], C->m[1][0], B->m[0][2], C->m[2][0]);
	t.m[0][1] = BR_MAC3(B->m[0][0], C->m[0][1], B->m[0][1], C->m[1][1], B->m[0][2], C->m[2][1]);
	t.m[0][2] = BR_MAC3(B->m[0][0], C->m[0][2], B->m[0][1], C->m[1][2], B->m[0][2], C->m[2][2]);

	t.m[1][0] = BR_MAC3(B->m[1][0], C->m[0][0], B->m[1][1], C->m[1][0], B->m[1][2], C->m[2][0]);
	t.m[1][1] = BR_MAC3(B->m[1][0], C->m[0][1], B->m[1][1], C->m[1][1], B->m[1][2], C->m[2][1]);
	t.m[1][2] = BR_MAC3(B->m[1][0], C->m[0][2], B->m[1][1], C->m[1][2], B->m[1][2], C->m[2][2]);

	t.m[2][0] = BR_MAC3(B->m[2][0], C->m[0][0], B->m[2][1], C->m[1][0], B->m[2][2], C->m[2][0]);
	t.m[2][1] = BR_MAC3(B->m[2][0], C->m[0][1], B->m[2][1], C->m[1][1], B->m[2][2], C->m[2][1]);
	t.m[2][2] = BR_MAC3(B->m[2][0], C->m[0][2], B->m[2][1], C->m[1][2], B->m[2][2], C->m[2][2]);

	t.m[3][0] = BR_ADD(BR_MAC3(B->m[3][0], C->m[0][0], B->m[3][1], C->m[1][0], B->m[3][2], C->m[2][0]), C->m[3][0]);
	t.m[3][1] = BR_ADD(BR_MAC3(B->m[3][0], C->m[0][1], B->m[3][1], C->m[1][1], B->m[3][2], C->m[2][1]), C->m[3][1]);
	t.m[3][2] = BR_ADD(BR_MAC3(B->m[3][0], C->m[0][2], B->m[3][1], C->m[1][2], B->m[3][2], C->m[2][2]), C->m[3][2]);

	BrMatrix34Copy(A, &t);
}

void BrMatrix34Identity(br_matrix34* mat)
{
	static br_matrix34 identity = BR_MATRIX34_IDENTITY();
	BrMatrix34Copy(mat, &identity);
}

void BrMatrix34Translate(br_matrix34* mat, br_scalar dx, br_scalar dy, br_scalar dz)
{
	BrMatrix34Identity(mat);
	mat->m[3][0] = dx;
	mat->m[3][1] = dy;
	mat->m[3][2] = dz;
}

void BrMatrix34RotateX(br_matrix34* mat, br_angle rx)
{
	BrMatrix34Identity(mat);
	mat->m[1][1] =  BR_COS(rx);
	mat->m[1][2] =  BR_SIN(rx);
	mat->m[2][1] = -BR_SIN(rx);
	mat->m[2][2] =  BR_COS(rx);
}

void BrMatrix34RotateY(br_matrix34* mat, br_angle ry)
{
	BrMatrix34Identity(mat);
	mat->m[0][0] =  BR_COS(ry);
	mat->m[0][2] = -BR_SIN(ry);
	mat->m[2][0] =  BR_SIN(ry);
	mat->m[2][2] =  BR_COS(ry);
}

void BrMatrix34RotateZ(br_matrix34* mat, br_angle rz)
{
	BrMatrix34Identity(mat);
	mat->m[0][0] =  BR_COS(rz);
	mat->m[0][1] =  BR_SIN(rz);
	mat->m[1][0] = -BR_SIN(rz);
	mat->m[1][1] =  BR_COS(rz);
}

void BrMatrix34PreRotateX(br_matrix34* mat, br_angle rx)
{
	BrMatrix34RotateX(&br_tmp_m34_1, rx);
	BrMatrix34Pre(mat, &br_tmp_m34_1);
}

void BrMatrix34PostRotateX(br_matrix34* mat, br_angle rx)
{
	BrMatrix34RotateX(&br_tmp_m34_1, rx);
	BrMatrix34Post(mat, &br_tmp_m34_1);
}

void BrMatrix34PreRotateY(br_matrix34* mat, br_angle ry)
{
	BrMatrix34RotateY(&br_tmp_m34_1, ry);
	BrMatrix34Pre(mat, &br_tmp_m34_1);
}

void BrMatrix34PostRotateY(br_matrix34* mat, br_angle ry)
{
	BrMatrix34RotateY(&br_tmp_m34_1, ry);
	BrMatrix34Post(mat, &br_tmp_m34_1);
}

void BrMatrix34PreRotateZ(br_matrix34* mat, br_angle rz)
{
	BrMatrix34RotateZ(&br_tmp_m34_1, rz);
	BrMatrix34Pre(mat, &br_tmp_m34_1);
}

void BrMatrix34PostRotateZ(br_matrix34* mat, br_angle rz)
{
	BrMatrix34RotateZ(&br_tmp_m34_1, rz);
	BrMatrix34Post(mat, &br_tmp_m34_1);
}

void BrMatrix34PostShearX(br_matrix34* mat, br_scalar sy, br_scalar sz)
{
	BrMatrix34ShearX(&br_tmp_m34_1, sy, sz);
	BrMatrix34Post(mat, &br_tmp_m34_1);
}

void BrMatrix34PostShearY(br_matrix34* mat, br_scalar sx, br_scalar sz)
{
	FIXME("(): stub");
}

void BrMatrix34TApplyV(br_vector3* A, const br_vector3* B, const br_matrix34* C)
{
	static br_vector3 t;

//	t.v[0] = BR_MAC3(B->v[0], C->m[0][0], B->v[1], C->m[0][1], B->v[2], C->m[0][2]);
//	t.v[1] = BR_MAC3(B->v[0], C->m[1][0], B->v[1], C->m[1][1], B->v[2], C->m[1][2]);
//	t.v[2] = BR_MAC3(B->v[0], C->m[2][0], B->v[1], C->m[2][1], B->v[2], C->m[2][2]);
	t.v[0] = BR_MAC3(B->v[0], C->m[0][0], B->v[0], C->m[1][0], B->v[0], C->m[2][0]);
	t.v[1] = BR_MAC3(B->v[1], C->m[0][1], B->v[1], C->m[1][1], B->v[1], C->m[2][1]);
	t.v[2] = BR_MAC3(B->v[2], C->m[0][2], B->v[2], C->m[1][2], B->v[2], C->m[2][2]);

	BrVector3Copy(A, &t);
}

void BrMatrix34TApplyP(br_vector3* A, const br_vector3* B, const br_matrix34* C)
{
	static br_vector3 t;

//	t.v[0] = BR_ADD(BR_MAC3(B->v[0], C->m[0][0], B->v[1], C->m[0][1], B->v[2], C->m[0][2]), C->m[3][0]);
//	t.v[1] = BR_ADD(BR_MAC3(B->v[0], C->m[1][0], B->v[1], C->m[1][1], B->v[2], C->m[1][2]), C->m[3][1]);
//	t.v[2] = BR_ADD(BR_MAC3(B->v[0], C->m[2][0], B->v[1], C->m[2][1], B->v[2], C->m[2][2]), C->m[3][2]);
	t.v[0] = BR_ADD(BR_MAC3(B->v[0], C->m[0][0], B->v[0], C->m[1][0], B->v[0], C->m[2][0]), C->m[3][0]);
	t.v[1] = BR_ADD(BR_MAC3(B->v[1], C->m[0][1], B->v[1], C->m[1][1], B->v[1], C->m[2][1]), C->m[3][1]);
	t.v[2] = BR_ADD(BR_MAC3(B->v[2], C->m[0][2], B->v[2], C->m[1][2], B->v[2], C->m[2][2]), C->m[3][2]);

	BrVector3Copy(A, &t);
}

void BrMatrix34ApplyV(br_vector3* A, const br_vector3* B, const br_matrix34* C)
{
	static br_vector3 t;

	t.v[0] = BR_MAC3(B->v[0], C->m[0][0], B->v[1], C->m[1][0], B->v[2], C->m[2][0]);
	t.v[1] = BR_MAC3(B->v[0], C->m[0][1], B->v[1], C->m[1][1], B->v[2], C->m[2][1]);
	t.v[2] = BR_MAC3(B->v[0], C->m[0][2], B->v[1], C->m[1][2], B->v[2], C->m[2][2]);

	BrVector3Copy(A, &t);
}

void BrMatrix34ApplyP(br_vector3* A, const br_vector3* B, const br_matrix34* C)
{
	static br_vector3 t;

	t.v[0] = BR_ADD(BR_MAC3(B->v[0], C->m[0][0], B->v[1], C->m[1][0], B->v[2], C->m[2][0]), C->m[3][0]);
	t.v[1] = BR_ADD(BR_MAC3(B->v[0], C->m[0][1], B->v[1], C->m[1][1], B->v[2], C->m[2][1]), C->m[3][1]);
	t.v[2] = BR_ADD(BR_MAC3(B->v[0], C->m[0][2], B->v[1], C->m[1][2], B->v[2], C->m[2][2]), C->m[3][2]);

	BrVector3Copy(A, &t);
}

void BrMatrix34LPNormalise(br_matrix34* A, const br_matrix34* B)
{
	static br_matrix34 t;

	BrVector3Normalise((br_vector3 *)(t.m[0]), (br_vector3 *)(B->m[0]));
	BrVector3Normalise((br_vector3 *)(t.m[1]), (br_vector3 *)(B->m[1]));
	BrVector3Normalise((br_vector3 *)(t.m[2]), (br_vector3 *)(B->m[2]));
	t.m[3][0] = B->m[3][0];
	t.m[3][1] = B->m[3][1];
	t.m[3][2] = B->m[3][2];

	BrMatrix34Copy(A, &t);
}

void BrMatrix34ShearX(br_matrix34* mat, br_scalar sy, br_scalar sz)
{
	BrMatrix34Identity(mat);
	mat->m[0][1] = sy;
	mat->m[0][2] = sz;
}

void BrMatrix34Rotate(br_matrix34* mat, br_angle r, const br_vector3* a)
{
	BrMatrix34Identity(mat);

	if (a == 0) { return; }

	br_scalar c = BR_COS(r);
	br_scalar s = BR_SIN(r);
	br_scalar t = BR_SUB(1.0f, c);

	mat->m[0][0] = t * a->v[0] * a->v[0] + c;
	mat->m[0][1] = t * a->v[0] * a->v[1] - a->v[2] * s;
	mat->m[0][2] = t * a->v[0] * a->v[2] + a->v[1] * s;
	mat->m[1][0] = t * a->v[0] * a->v[1] + a->v[2] * s;
	mat->m[1][1] = t * a->v[1] * a->v[1] + c;
	mat->m[1][2] = t * a->v[1] * a->v[2] - a->v[0] * s;
	mat->m[2][0] = t * a->v[0] * a->v[2] - a->v[1] * s;
	mat->m[2][1] = t * a->v[1] * a->v[2] + a->v[0] * s;
	mat->m[2][2] = t * a->v[2] * a->v[2] + c;
}

void BrMatrix34Pre(br_matrix34* A, const br_matrix34* B)
{
	BrMatrix34Mul(&br_tmp_m34_2, B, A);
	BrMatrix34Copy(A, &br_tmp_m34_2);
}

void BrMatrix34PreRotate(br_matrix34* mat, br_angle r, const br_vector3* axis)
{
	FIXME("(): stub");
}

void BrMatrix34PreScale(br_matrix34* mat, br_scalar sx, br_scalar sy, br_scalar sz)
{
	FIXME("(): stub");
}

void BrMatrix34PreShearX(br_matrix34* mat, br_scalar sy, br_scalar sz)
{
	BrMatrix34ShearX(&br_tmp_m34_1, sy, sz);
	BrMatrix34Pre(mat, &br_tmp_m34_1);
}

void BrMatrix34PreShearY(br_matrix34* mat, br_scalar sx, br_scalar sz)
{
	FIXME("(): stub");
}

void BrMatrix34PreTranslate(br_matrix34* mat, br_scalar dx, br_scalar dy, br_scalar dz)
{
	BrMatrix34Translate(&br_tmp_m34_1, dx, dy, dz);
	BrMatrix34Pre(mat, &br_tmp_m34_1);
}

void BrMatrix34Post(br_matrix34* A, const br_matrix34* B)
{
	BrMatrix34Mul(&br_tmp_m34_2, A, B);
	BrMatrix34Copy(A, &br_tmp_m34_2);
}

void BrMatrix34PostTranslate(br_matrix34* mat, br_scalar dx, br_scalar dy, br_scalar dz)
{
	BrMatrix34Translate(&br_tmp_m34_1, dx, dy, dz);
	BrMatrix34Post(mat, &br_tmp_m34_1);
}

void BrMatrix34LPInverse(br_matrix34* A, const br_matrix34* B)
{
	static br_matrix34 t;

	br_scalar d  = B->m[0][0] * (B->m[1][1]*B->m[2][2] - B->m[1][2]*B->m[2][1]);
			  d -= B->m[0][1] * (B->m[1][0]*B->m[2][2] - B->m[1][2]*B->m[2][0]);
			  d += B->m[0][2] * (B->m[1][0]*B->m[2][1] - B->m[1][1]*B->m[2][0]);

	if (BR_ABS(d) < 0.99995)
	{
		INFO("%s", BrMatrix34ToString(B));
		WARN("This shit ain't LP : %f", d);
		return;
	}

	BrMatrix34Inverse(&t, B);
	BrMatrix34Copy(A, &t);
//	BrMatrix34Identity(A);
//	A->m[0][0] = B->m[0][0];
//	A->m[0][1] = B->m[1][0];
//	A->m[0][2] = B->m[2][0];
//	A->m[1][0] = B->m[0][1];
//	A->m[1][1] = B->m[1][1];
//	A->m[1][2] = B->m[2][1];
//	A->m[2][0] = B->m[0][2];
//	A->m[2][1] = B->m[1][2];
//	A->m[2][2] = B->m[2][2];
////	BrMatrix34ApplyV(pos, pos, A);
////	A->m[3][0] = pos->v[0];
////	A->m[3][1] = pos->v[1];
////	A->m[3][2] = pos->v[2];
}

br_scalar BrMatrix34Inverse(br_matrix34* A, const br_matrix34* B)
{
	br_scalar d  = B->m[0][0] * (B->m[1][1]*B->m[2][2] - B->m[1][2]*B->m[2][1]);
			  d -= B->m[0][1] * (B->m[1][0]*B->m[2][2] - B->m[1][2]*B->m[2][0]);
			  d += B->m[0][2] * (B->m[1][0]*B->m[2][1] - B->m[1][1]*B->m[2][0]);

	/* no inverse */
	if (d == 0) { return 0; }

	d = BR_RCP(d);

	static br_matrix34 t;

	t.m[0][0] =  BR_MUL(d, BR_SUB(BR_MUL(B->m[1][1], B->m[2][2]), BR_MUL(B->m[1][2], B->m[2][1])));
	t.m[1][0] = -BR_MUL(d, BR_SUB(BR_MUL(B->m[1][0], B->m[2][2]), BR_MUL(B->m[1][2], B->m[2][0])));
	t.m[2][0] =  BR_MUL(d, BR_SUB(BR_MUL(B->m[1][0], B->m[2][1]), BR_MUL(B->m[1][1], B->m[2][0])));

	t.m[0][1] = -BR_MUL(d, BR_SUB(BR_MUL(B->m[0][1], B->m[2][2]), BR_MUL(B->m[0][2], B->m[2][1])));
	t.m[1][1] =  BR_MUL(d, BR_SUB(BR_MUL(B->m[0][0], B->m[2][2]), BR_MUL(B->m[0][2], B->m[2][0])));
	t.m[2][1] = -BR_MUL(d, BR_SUB(BR_MUL(B->m[0][0], B->m[2][1]), BR_MUL(B->m[0][1], B->m[2][0])));

	t.m[0][2] =  BR_MUL(d, BR_SUB(BR_MUL(B->m[0][1], B->m[1][2]), BR_MUL(B->m[0][2], B->m[1][1])));
	t.m[1][2] = -BR_MUL(d, BR_SUB(BR_MUL(B->m[0][0], B->m[1][2]), BR_MUL(B->m[0][2], B->m[1][0])));
	t.m[2][2] =  BR_MUL(d, BR_SUB(BR_MUL(B->m[0][0], B->m[1][1]), BR_MUL(B->m[0][1], B->m[1][0])));

	t.m[3][0] = -BR_MAC3(B->m[3][0], t.m[0][0], B->m[3][1], t.m[1][0], B->m[3][2], t.m[2][0]);
	t.m[3][1] = -BR_MAC3(B->m[3][0], t.m[0][1], B->m[3][1], t.m[1][1], B->m[3][2], t.m[2][1]);
	t.m[3][2] = -BR_MAC3(B->m[3][0], t.m[0][2], B->m[3][1], t.m[1][2], B->m[3][2], t.m[2][2]);

	BrMatrix34Copy(A, &t);

	return d;
}

void BrMatrix34Scale(br_matrix34* mat, br_scalar sx, br_scalar sy, br_scalar sz)
{
	BrMatrix34Identity(mat);
	mat->m[0][0] = sx;
	mat->m[1][1] = sy;
	mat->m[2][2] = sz;
}

void BrMatrix34PostScale(br_matrix34* mat, br_scalar sx, br_scalar sy, br_scalar sz)
{
	BrMatrix34Scale(&br_tmp_m34_1, sx, sy, sz);
	BrMatrix34Post(mat, &br_tmp_m34_1);
}

void BrMatrix34PostShearZ(br_matrix34* mat, br_scalar sx, br_scalar sy)
{
	FIXME("(): stub");
}

void BrMatrix34RollingBall(br_matrix34* mat, int dx, int dy, int radius)
{
	FIXME("(): stub");
}

void BrMatrix34PreTransform(br_matrix34* mat, const br_transform* xform)
{
	BrTransformToMatrix34(&br_tmp_m34_1, xform);
	BrMatrix34Pre(mat, &br_tmp_m34_1);
}

void BrMatrix34PostTransform(br_matrix34* mat, const br_transform* xform)
{
	FIXME("(): stub");
}

void BrMatrix34ToTransform(br_transform* xform, const br_matrix34* mat)
{
	xform->type = BR_TRANSFORM_MATRIX34;
	BrMatrix34Copy(&xform->t.mat, mat);
}

char* BrMatrix34ToString(const br_matrix34* mat)
{
	char *buffer = BrMemAllocate(255, BR_MEMORY_STRING);
	sprintf(buffer, "{ {M00:%g M01:%g M02:%g} {M10:%g M11:%g M12:%g} {M20:%g M21:%g M22:%g} {M30:%g M31:%g M32:%g} }", mat->m[0][0], mat->m[0][1], mat->m[0][2]
																													 , mat->m[1][0], mat->m[1][1], mat->m[1][2]
																													 , mat->m[2][0], mat->m[2][1], mat->m[2][2]
																													 , mat->m[3][0], mat->m[3][1], mat->m[3][2]);
	return buffer;
}

/************************* Matrix4 ************************/

void BrMatrix4Copy(br_matrix4* A, const br_matrix4* B)
{
	BrBlockCopy(A, B, sizeof(br_matrix4));
}

void BrMatrix4Copy34(br_matrix4* A, const br_matrix34* B)
{
	BrMatrix4Identity(A);
	A->m[0][0] = B->m[0][0]; A->m[0][1] = B->m[0][1]; A->m[0][2] = B->m[0][2];
	A->m[1][0] = B->m[1][0]; A->m[1][1] = B->m[1][1]; A->m[1][2] = B->m[1][2];
	A->m[2][0] = B->m[2][0]; A->m[2][1] = B->m[2][1]; A->m[2][2] = B->m[2][2];
	A->m[3][0] = B->m[3][0]; A->m[3][1] = B->m[3][1]; A->m[3][2] = B->m[3][2];
}

void BrMatrix4Identity(br_matrix4* mat)
{
	static br_matrix4 identity = BR_MATRIX4_IDENTITY();
	BrMatrix4Copy(mat, &identity);
}

void BrMatrix4Perspective(br_matrix4* mat, br_angle field_of_view, br_scalar aspect, br_scalar hither, br_scalar yon)
{
	static br_matrix4 empty;
	BrMatrix4Copy(mat, &empty);

	if (1)
	{
		br_scalar f = BR_COTAN(BR_DIV(field_of_view, 2));
		br_scalar dp = BR_SUB(yon, hither);
		mat->m[0][0] =  BR_DIV(f, aspect);
		mat->m[1][1] =  f;
		mat->m[2][2] =  BR_DIV(BR_ADD(yon, hither), dp);
		mat->m[2][3] = -1.0;
		mat->m[3][2] = -BR_DIV(BR_MUL(2, BR_MUL(yon, hither)), dp);
	}
	else
	{
		br_scalar range  = BR_MUL(BR_TAN(field_of_view), hither);
		br_scalar left	 = BR_MUL(-range, aspect);
		br_scalar right	 = BR_MUL(range, aspect);
		br_scalar bottom = -range;
		br_scalar top	 = range;

		mat->m[0][0] =  BR_DIV(BR_MUL(2, hither), BR_SUB(right, left));
		mat->m[1][1] =  BR_DIV(BR_MUL(2, hither), BR_SUB(top, bottom));
		mat->m[2][2] = -BR_DIV(BR_ADD(yon, hither), BR_SUB(yon, hither));
		mat->m[2][3] = -1.0;
		mat->m[3][2] = -BR_DIV(BR_MUL(2, BR_MUL(yon, hither)), BR_SUB(yon, hither));
	}
}

void BrMatrix4TApply(br_vector4* A, const br_vector4* B, const br_matrix4* C)
{
	static br_vector4 t;

	t.v[0] = (B->v[0] * C->m[0][0]) + (B->v[1] * C->m[0][1]) + (B->v[2] * C->m[0][2]) + (B->v[3] * C->m[0][3]);
	t.v[1] = (B->v[0] * C->m[1][0]) + (B->v[1] * C->m[1][1]) + (B->v[2] * C->m[1][2]) + (B->v[3] * C->m[1][3]);
	t.v[2] = (B->v[0] * C->m[2][0]) + (B->v[1] * C->m[2][1]) + (B->v[2] * C->m[2][2]) + (B->v[3] * C->m[2][3]);
	t.v[3] = (B->v[0] * C->m[3][0]) + (B->v[1] * C->m[3][1]) + (B->v[2] * C->m[3][2]) + (B->v[3] * C->m[3][3]);

	BrVector4Copy(A, &t);
	//BR_MATRIX4_TOSTRING(C);
}

br_scalar BrMatrix4Inverse(br_matrix4* A, const br_matrix4* B)
{
	br_scalar d = BrMatrix4Determinant(B);

	if (d == 0)
	{
		return 0;
	}

	br_scalar invd = BR_RCP(d);
	static br_matrix4 t;

	br_scalar b0 = BR_SUB(BR_MUL(B->m[2][0], B->m[3][1]), BR_MUL(B->m[2][1], B->m[3][0]));
	br_scalar b1 = BR_SUB(BR_MUL(B->m[2][0], B->m[3][2]), BR_MUL(B->m[2][2], B->m[3][0]));
	br_scalar b2 = BR_SUB(BR_MUL(B->m[2][3], B->m[3][0]), BR_MUL(B->m[2][0], B->m[3][3]));
	br_scalar b3 = BR_SUB(BR_MUL(B->m[2][1], B->m[3][2]), BR_MUL(B->m[2][2], B->m[3][1]));
	br_scalar b4 = BR_SUB(BR_MUL(B->m[2][3], B->m[3][1]), BR_MUL(B->m[2][1], B->m[3][3]));
	br_scalar b5 = BR_SUB(BR_MUL(B->m[2][2], B->m[3][3]), BR_MUL(B->m[2][3], B->m[3][2]));

	br_scalar a0 = BR_SUB(BR_MUL(B->m[0][0], B->m[1][1]), BR_MUL(B->m[0][1], B->m[1][0]));
	br_scalar a1 = BR_SUB(BR_MUL(B->m[0][0], B->m[1][2]), BR_MUL(B->m[0][2], B->m[1][0]));
	br_scalar a2 = BR_SUB(BR_MUL(B->m[0][3], B->m[1][0]), BR_MUL(B->m[0][0], B->m[1][3]));
	br_scalar a3 = BR_SUB(BR_MUL(B->m[0][1], B->m[1][2]), BR_MUL(B->m[0][2], B->m[1][1]));
	br_scalar a4 = BR_SUB(BR_MUL(B->m[0][3], B->m[1][1]), BR_MUL(B->m[0][1], B->m[1][3]));
	br_scalar a5 = BR_SUB(BR_MUL(B->m[0][2], B->m[1][3]), BR_MUL(B->m[0][3], B->m[1][2]));

	br_scalar d11 = BR_MAC3(B->m[1][1],  b5, B->m[1][2],  b4, B->m[1][3], b3);
	br_scalar d12 = BR_MAC3(B->m[1][0],  b5, B->m[1][2],  b2, B->m[1][3], b1);
	br_scalar d13 = BR_MAC3(B->m[1][0], -b4, B->m[1][1],  b2, B->m[1][3], b0);
	br_scalar d14 = BR_MAC3(B->m[1][0],  b3, B->m[1][1], -b1, B->m[1][2], b0);

	br_scalar d21 = BR_MAC3(B->m[0][1],  b5, B->m[0][2],  b4, B->m[0][3], b3);
	br_scalar d22 = BR_MAC3(B->m[0][0],  b5, B->m[0][2],  b2, B->m[0][3], b1);
	br_scalar d23 = BR_MAC3(B->m[0][0], -b4, B->m[0][1],  b2, B->m[0][3], b0);
	br_scalar d24 = BR_MAC3(B->m[0][0],  b3, B->m[0][1], -b1, B->m[0][2], b0);

	br_scalar d31 = BR_MAC3(B->m[3][1],  a5, B->m[3][2],  a4, B->m[3][3], a3);
	br_scalar d32 = BR_MAC3(B->m[3][0],  a5, B->m[3][2],  a2, B->m[3][3], a1);
	br_scalar d33 = BR_MAC3(B->m[3][0], -a4, B->m[3][1],  a2, B->m[3][3], a0);
	br_scalar d34 = BR_MAC3(B->m[3][0],  a3, B->m[3][1], -a1, B->m[3][2], a0);

	br_scalar d41 = BR_MAC3(B->m[2][1],  a5, B->m[2][2],  a4, B->m[2][3], a3);
	br_scalar d42 = BR_MAC3(B->m[2][0],  a5, B->m[2][2],  a2, B->m[2][3], a1);
	br_scalar d43 = BR_MAC3(B->m[2][0], -a4, B->m[2][1],  a2, B->m[2][3], a0);
	br_scalar d44 = BR_MAC3(B->m[2][0],  a3, B->m[2][1], -a1, B->m[2][2], a0);

	t.m[0][0] =  BR_MUL(d11, invd);
	t.m[0][1] = -BR_MUL(d21, invd);
	t.m[0][2] =  BR_MUL(d31, invd);
	t.m[0][3] = -BR_MUL(d41, invd);

	t.m[1][0] = -BR_MUL(d12, invd);
	t.m[1][1] =  BR_MUL(d22, invd);
	t.m[1][2] = -BR_MUL(d32, invd);
	t.m[1][3] =  BR_MUL(d42, invd);

	t.m[2][0] =  BR_MUL(d13, invd);
	t.m[2][1] = -BR_MUL(d23, invd);
	t.m[2][2] =  BR_MUL(d33, invd);
	t.m[2][3] = -BR_MUL(d43, invd);

	t.m[3][0] = -BR_MUL(d14, invd);
	t.m[3][1] =  BR_MUL(d24, invd);
	t.m[3][2] = -BR_MUL(d34, invd);
	t.m[3][3] =  BR_MUL(d44, invd);

	BrMatrix4Copy(A, &t);

	return d;
}

void BrMatrix4Scale(br_matrix4* mat, br_scalar sx, br_scalar sy, br_scalar sz)
{
	BrMatrix4Identity(mat);
	mat->m[0][0] = sx;
	mat->m[1][1] = sy;
	mat->m[2][2] = sz;
}

void BrMatrix4Mul(br_matrix4* A, const br_matrix4* B, const br_matrix4* C)
{
	static br_matrix4 t;

	//t.m[0][0] = BR_MAC4(B->m[0][0], C->m[0][0], B->m[0][1], C->m[1][0], B->m[0][2], C->m[2][0], B->m[0][3], C->m[3][0]);
	t.m[0][0] = B->m[0][0] * C->m[0][0] + B->m[0][1] * C->m[1][0] + B->m[0][2] * C->m[2][0] + B->m[0][3] * C->m[3][0];
	t.m[0][1] = B->m[0][0] * C->m[0][1] + B->m[0][1] * C->m[1][1] + B->m[0][2] * C->m[2][1] + B->m[0][3] * C->m[3][1];
	t.m[0][2] = B->m[0][0] * C->m[0][2] + B->m[0][1] * C->m[1][2] + B->m[0][2] * C->m[2][2] + B->m[0][3] * C->m[3][2];
	t.m[0][3] = B->m[0][0] * C->m[0][3] + B->m[0][1] * C->m[1][3] + B->m[0][2] * C->m[2][3] + B->m[0][3] * C->m[3][3];

	t.m[1][0] = B->m[1][0] * C->m[0][0] + B->m[1][1] * C->m[1][0] + B->m[1][2] * C->m[2][0] + B->m[1][3] * C->m[3][0];
	t.m[1][1] = B->m[1][0] * C->m[0][1] + B->m[1][1] * C->m[1][1] + B->m[1][2] * C->m[2][1] + B->m[1][3] * C->m[3][1];
	t.m[1][2] = B->m[1][0] * C->m[0][2] + B->m[1][1] * C->m[1][2] + B->m[1][2] * C->m[2][2] + B->m[1][3] * C->m[3][2];
	t.m[1][3] = B->m[1][0] * C->m[0][3] + B->m[1][1] * C->m[1][3] + B->m[1][2] * C->m[2][3] + B->m[1][3] * C->m[3][3];

	t.m[2][0] = B->m[2][0] * C->m[0][0] + B->m[2][1] * C->m[1][0] + B->m[2][2] * C->m[2][0] + B->m[2][3] * C->m[3][0];
	t.m[2][1] = B->m[2][0] * C->m[0][1] + B->m[2][1] * C->m[1][1] + B->m[2][2] * C->m[2][1] + B->m[2][3] * C->m[3][1];
	t.m[2][2] = B->m[2][0] * C->m[0][2] + B->m[2][1] * C->m[1][2] + B->m[2][2] * C->m[2][2] + B->m[2][3] * C->m[3][2];
	t.m[2][3] = B->m[2][0] * C->m[0][3] + B->m[2][1] * C->m[1][3] + B->m[2][2] * C->m[2][3] + B->m[2][3] * C->m[3][3];

	t.m[3][0] = B->m[3][0] * C->m[0][0] + B->m[3][1] * C->m[1][0] + B->m[3][2] * C->m[2][0] + B->m[3][3] * C->m[3][0];
	t.m[3][1] = B->m[3][0] * C->m[0][1] + B->m[3][1] * C->m[1][1] + B->m[3][2] * C->m[2][1] + B->m[3][3] * C->m[3][1];
	t.m[3][2] = B->m[3][0] * C->m[0][2] + B->m[3][1] * C->m[1][2] + B->m[3][2] * C->m[2][2] + B->m[3][3] * C->m[3][2];
	t.m[3][3] = B->m[3][0] * C->m[0][3] + B->m[3][1] * C->m[1][3] + B->m[3][2] * C->m[2][3] + B->m[3][3] * C->m[3][3];

	BrMatrix4Copy(A, &t);
}

void BrMatrix4ApplyP(br_vector4* A, const br_vector3* B, const br_matrix4* C)
{
	static br_vector4 t;

	t.v[0] = BR_ADD(BR_MAC3(B->v[0], C->m[0][0], B->v[1], C->m[1][0], B->v[2], C->m[2][0]), C->m[3][0]);
	t.v[1] = BR_ADD(BR_MAC3(B->v[0], C->m[0][1], B->v[1], C->m[1][1], B->v[2], C->m[2][1]), C->m[3][1]);
	t.v[2] = BR_ADD(BR_MAC3(B->v[0], C->m[0][2], B->v[1], C->m[1][2], B->v[2], C->m[2][2]), C->m[3][2]);
	t.v[3] = 1;
	//t.v[3] = BR_ADD(BR_MAC3(B->v[0], C->m[0][3], B->v[1], C->m[1][3], B->v[2], C->m[2][2]), C->m[3][2]);

	BrVector4Copy(A, &t);
}

br_scalar BrMatrix4Determinant(const br_matrix4* mat)
{
	br_scalar b0 = BR_SUB(BR_MUL(mat->m[2][0], mat->m[3][1]), BR_MUL(mat->m[2][1], mat->m[3][0]));
	br_scalar b1 = BR_SUB(BR_MUL(mat->m[2][0], mat->m[3][2]), BR_MUL(mat->m[2][2], mat->m[3][0]));
	br_scalar b2 = BR_SUB(BR_MUL(mat->m[2][3], mat->m[3][0]), BR_MUL(mat->m[2][0], mat->m[3][3]));
	br_scalar b3 = BR_SUB(BR_MUL(mat->m[2][1], mat->m[3][2]), BR_MUL(mat->m[2][2], mat->m[3][1]));
	br_scalar b4 = BR_SUB(BR_MUL(mat->m[2][3], mat->m[3][1]), BR_MUL(mat->m[2][1], mat->m[3][3]));
	br_scalar b5 = BR_SUB(BR_MUL(mat->m[2][2], mat->m[3][3]), BR_MUL(mat->m[2][3], mat->m[3][2]));

	br_scalar d11 = BR_MAC3(mat->m[1][1],  b5, mat->m[1][2],  b4, mat->m[1][3], b3);
	br_scalar d12 = BR_MAC3(mat->m[1][0],  b5, mat->m[1][2],  b2, mat->m[1][3], b1);
	br_scalar d13 = BR_MAC3(mat->m[1][0], -b4, mat->m[1][1],  b2, mat->m[1][3], b0);
	br_scalar d14 = BR_MAC3(mat->m[1][0],  b3, mat->m[1][1], -b1, mat->m[1][2], b0);

	return BR_ADD(BR_SUB(BR_MUL(mat->m[0][0], d11), BR_MUL(mat->m[0][1], d12)), BR_SUB(BR_MUL(mat->m[0][2], d13), BR_MUL(mat->m[0][3], d14)));
}
