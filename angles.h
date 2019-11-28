#include "compiler.h"
#include "fixed.h"

#include <math.h>

#ifndef _BRENDER_ANGLES_H_

typedef br_fixed_ls br_angle;

enum {
	BR_EULER_XYZ_S
};

typedef struct {
	br_angle	a;
	br_angle	b;
	br_angle	c;
	br_uint_8	order;
} br_euler;

#define BR_SIN(a) 		sin(a * BrAngleToRadians)
#define BR_COS(a) 		cos(a * BrAngleToRadians)
#define BR_TAN(a) 		tan(a * BrAngleToRadians)
#define BR_COTAN(a) 	(BR_COS(a) / BR_SIN(a))
#define BR_ATAN2(y,x) 	BR_ANGLE_RAD(atan2(y, x))
#define BR_ASIN(d) 		BR_ANGLE_RAD(asin(d))

#define BR_ANGLE_DEG(d) BrDegreeToAngle(BR_SCALAR(d))
#define BR_ANGLE_RAD(r) BrRadianToAngle(BR_SCALAR(r))

#define PI 3.14159265358979323846
#define BrAngleToRadians 0.0000958737992428525731200550374922
#define DegreeToBrAngle 182.0444444444445

br_scalar BrAngleToDegree(br_angle a);
br_scalar BrAngleToRadian(br_angle a);
br_scalar BrAngleToScalar(br_angle a);
br_angle BrDegreeToAngle(br_scalar s);
br_angle BrRadianToAngle(br_scalar s);

#define _BRENDER_ANGLES_H_
#endif
