#include "brender.h"
#include "angles.h"

br_scalar BrAngleToDegree(br_angle a)
{
	return BR_MULDIV(a, 360, 65535);
}

br_scalar BrAngleToRadian(br_angle a)
{
    // Loses precision from double to single
	return a * BrAngleToRadians;
}

br_scalar BrAngleToScalar(br_angle a)
{
	if (a > 65535)
	{
		WARN("What to do, what to do");
		return 0;
	}
	return BR_DIV(a, 65535);
}

br_angle BrDegreeToAngle(br_scalar s)
{
	return (br_angle)(s * DegreeToBrAngle) & 0xFFFF;
}

br_angle BrRadianToAngle(br_scalar s)
{
	return (br_angle)(s / BrAngleToRadians) & 0xFFFF;
}
