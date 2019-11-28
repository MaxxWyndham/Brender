#include "brender.h"
#include "transform.h"

void BrMatrix34Transform(br_matrix34* A, const br_transform* xform)
{
	BrMatrix34Copy(A, &xform->t.mat);
}

void BrTransformToMatrix34(br_matrix34* mat, const br_transform* xform)
{
	switch (xform->type)
	{
        case BR_TRANSFORM_IDENTITY:
            //BrMatrix34Identity(mat);
            BrMatrix34Copy(mat, &xform->t.mat);
            break;

        case BR_TRANSFORM_TRANSLATION:
            BrMatrix34Translate(mat, xform->t.translate.t.v[0], xform->t.translate.t.v[1], xform->t.translate.t.v[2]);
            break;

        case BR_TRANSFORM_EULER:
            BrEulerToMatrix34(mat, &xform->t.euler.e);
            mat->m[3][0] = xform->t.euler.t.v[0];
            mat->m[3][1] = xform->t.euler.t.v[1];
            mat->m[3][2] = xform->t.euler.t.v[2];
            break;

        case BR_TRANSFORM_LOOK_UP:
        {
			br_vector3 xaxis, yaxis, zaxis;

			BrVector3Sub(&zaxis, &xform->t.look_up.t, &xform->t.look_up.look);
			BrVector3Normalise(&zaxis, &zaxis);

			BrVector3Cross(&xaxis, &xform->t.look_up.up, &zaxis);
			BrVector3Normalise(&xaxis, &xaxis);

			BrVector3Cross(&yaxis, &zaxis, &xaxis);

			mat->m[0][0] = xaxis.v[0];	mat->m[0][1] = yaxis.v[0];	mat->m[0][2] = zaxis.v[0];
			mat->m[1][0] = xaxis.v[1];	mat->m[1][1] = yaxis.v[1];	mat->m[1][2] = zaxis.v[1];
			mat->m[2][0] = xaxis.v[2];	mat->m[2][1] = yaxis.v[2];	mat->m[2][2] = zaxis.v[2];
			mat->m[3][0] = -BrVector3Dot(&xaxis, &xform->t.look_up.t);
			mat->m[3][1] = -BrVector3Dot(&yaxis, &xform->t.look_up.t);
			mat->m[3][2] = -BrVector3Dot(&zaxis, &xform->t.look_up.t);
            break;
        }

        case BR_TRANSFORM_QUAT:
            BrQuatToMatrix34(mat, &xform->t.quat.q);
            break;

        case BR_TRANSFORM_MATRIX34:
        case BR_TRANSFORM_MATRIX34_LP:
            BrMatrix34Copy(mat, &xform->t.mat);
            break;

		default:
			WARN("unknown transform type : %u", xform->type);
            break;
	}
}

void BrTransformToTransform(br_transform* dest, const br_transform* src)
{
	if (src->type == dest->type)
	{
		BrBlockCopy(dest, src, sizeof(br_transform));
	}
	else
	{
		br_matrix34 t;
		BrTransformToMatrix34(&t, src);
		BrMatrix34ToTransform(dest, &t);
	}
}
