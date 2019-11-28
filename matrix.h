#include "scalar.h"
#include "angles.h"
#include "quat.h"

#ifndef _BRENDER_MATRIX_H_
#define _BRENDER_MATRIX_H_

#include "transform.h"

typedef struct br_matrix
{
	br_scalar m[3][2];
} br_matrix;

typedef br_matrix br_matrix23; /* uh? */

typedef struct br_matrix34
{
	br_scalar m[4][3];
} br_matrix34;

typedef struct br_matrix4
{
	br_scalar m[4][4];
} br_matrix4;

// supposed to be in angles.h but damn circular reference, need to move stuff
br_matrix34* BrEulerToMatrix34(br_matrix34* mat, const br_euler* euler);

// supposed to be in quat.h but same reason as above -errol
br_matrix34* BrQuatToMatrix34(br_matrix34* mat, const br_quat* q);

void BrMatrix23Identity(br_matrix23* mat);
void BrMatrix23Mul(br_matrix23* A, const br_matrix23* B, const br_matrix23* C);
void BrMatrix23Copy(br_matrix23* A, const br_matrix23* B);
void BrMatrix23PostScale(br_matrix23* mat, br_scalar sx, br_scalar sy);

void BrMatrix34Copy(br_matrix34* A, const br_matrix34* B);
void BrMatrix34Mul(br_matrix34* A, const br_matrix34* B, const br_matrix34* C);
void BrMatrix34Pre(br_matrix34* A, const br_matrix34* B);
void BrMatrix34Post(br_matrix34* A, const br_matrix34* B);

void BrMatrix34ToTransform(br_transform* xform, const br_matrix34* mat);

void BrMatrix34Identity(br_matrix34* mat);
void BrMatrix34Translate(br_matrix34* mat, br_scalar dx, br_scalar dy, br_scalar dz);
void BrMatrix34Scale(br_matrix34* mat, br_scalar sx, br_scalar sy, br_scalar sz);
void BrMatrix34ShearX(br_matrix34* mat, br_scalar sy, br_scalar sz);
void BrMatrix34RotateX(br_matrix34* mat, br_angle rx);
void BrMatrix34RotateY(br_matrix34* mat, br_angle ry);
void BrMatrix34Rotate(br_matrix34* mat, br_angle r, const br_vector3* a);

void BrMatrix34TApplyP(br_vector3* A, const br_vector3* B, const br_matrix34* C);
void BrMatrix34TApplyV(br_vector3* A, const br_vector3* B, const br_matrix34* C);
void BrMatrix34ApplyP(br_vector3* A, const br_vector3* B, const br_matrix34* C);
void BrMatrix34ApplyV(br_vector3* A, const br_vector3* B, const br_matrix34* C);
void BrMatrix34RollingBall(br_matrix34* mat, int dx, int dy, int radius);
void BrMatrix34LPNormalise(br_matrix34* A, const br_matrix34* B);
void BrMatrix34LPInverse(br_matrix34* A, const br_matrix34* B);
br_scalar BrMatrix34Inverse(br_matrix34* A, const br_matrix34* B);

void BrMatrix34PreTranslate(br_matrix34* mat, br_scalar dx, br_scalar dy, br_scalar dz);
void BrMatrix34PostTranslate(br_matrix34* mat, br_scalar dx, br_scalar dy, br_scalar dz);
void BrMatrix34PreScale(br_matrix34* mat, br_scalar sx, br_scalar sy, br_scalar sz);
void BrMatrix34PostScale(br_matrix34* mat, br_scalar sx, br_scalar sy, br_scalar sz);
void BrMatrix34PreShearX(br_matrix34* mat, br_scalar sy, br_scalar sz);
void BrMatrix34PostShearX(br_matrix34* mat, br_scalar sy, br_scalar sz);
void BrMatrix34PreShearY(br_matrix34* mat, br_scalar sx, br_scalar sz);
void BrMatrix34PostShearY(br_matrix34* mat, br_scalar sx, br_scalar sz);

void BrMatrix34PostShearZ(br_matrix34* mat, br_scalar sx, br_scalar sy);
void BrMatrix34PreRotateX(br_matrix34* mat, br_angle rx);
void BrMatrix34PostRotateX(br_matrix34* mat, br_angle rx);
void BrMatrix34PreRotateY(br_matrix34* mat, br_angle ry);
void BrMatrix34PostRotateY(br_matrix34* mat, br_angle ry);
void BrMatrix34PreRotateZ(br_matrix34* mat, br_angle rz);
void BrMatrix34PostRotateZ(br_matrix34* mat, br_angle rz);
void BrMatrix34PreRotate(br_matrix34* mat, br_angle r, const br_vector3* axis);
void BrMatrix34PreTransform(br_matrix34* mat, const br_transform* xform);
void BrMatrix34PostTransform(br_matrix34* mat, const br_transform* xform);
char* BrMatrix34ToString(const br_matrix34* mat);

#define BR_MATRIX34(m00, m01, m02, m10, m11, m12, m20, m21, m22, m30, m31, m32) {{ { m00, m01, m02 }, { m10, m11, m12 }, { m20, m21, m22 }, { m30, m31, m32 } }}
#define BR_MATRIX34_IDENTITY() BR_MATRIX34(1,0,0,0,1,0,0,0,1,0,0,0)
#define BR_MATRIX34_RESET() BR_MATRIX34(0,0,0,0,0,0,0,0,0,0,0,0)

void BrMatrix4Copy(br_matrix4* A, const br_matrix4* B);
void BrMatrix4Copy34(br_matrix4* A, const br_matrix34* B);
void BrMatrix4Identity(br_matrix4* mat);
void BrMatrix4Perspective(br_matrix4* mat, br_angle field_of_view, br_scalar aspect, br_scalar hither, br_scalar yon);
br_scalar BrMatrix4Determinant(const br_matrix4* mat);
void BrMatrix4TApply(br_vector4* A, const br_vector4* B, const br_matrix4* C);
void BrMatrix4Copy(br_matrix4* A, const br_matrix4* B);
br_scalar BrMatrix4Inverse(br_matrix4* A, const br_matrix4* B);
void BrMatrix4Perspective(br_matrix4* mat, br_angle field_of_view, br_scalar aspect, br_scalar hither, br_scalar yon);
void BrMatrix4Scale(br_matrix4* mat, br_scalar sx, br_scalar sy, br_scalar sz);
void BrMatrix4Mul(br_matrix4* A, const br_matrix4* B, const br_matrix4* C);
void BrMatrix4ApplyP(br_vector4* A, const br_vector3* B, const br_matrix4* C);

#define BR_MATRIX4(m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33) {{ { m00, m01, m02, m03 }, { m10, m11, m12, m13 }, { m20, m21, m22, m23 }, { m30, m31, m32, m33 } }}
#define BR_MATRIX4_IDENTITY() BR_MATRIX4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1)
#define BR_MATRIX4_RESET() BR_MATRIX4(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0)
#define BR_MATRIX4_TOSTRING(mat) INFO("{ { M00: %.6f M01: %.6f M02: %.6f M03: %.6f } { M10: %.6f M11: %.6f M12: %.6f M13: %.6f } { M20: %.6f M21: %.6f M22: %.6f M23: %.6f } { M30: %.6f M31: %.6f M32: %.6f M33: %.6f } }", mat->m[0][0], mat->m[0][1], mat->m[0][2], mat->m[0][3], mat->m[1][0], mat->m[1][1], mat->m[1][2], mat->m[1][3], mat->m[2][0], mat->m[2][1], mat->m[2][2], mat->m[2][3], mat->m[3][0], mat->m[3][1], mat->m[3][2], mat->m[3][3])

#endif
