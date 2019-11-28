#include "brender.h"
#include "scalar.h"

br_scalar BrIntToScalar(int i)
{
	//FIXME("(%d): check implementation", i);
	return (br_scalar)i;
}

int BrScalarToInt(br_scalar s)
{
	//FIXME("(%.4f): check implementation", s);
	int r = (int)s;
	return r;
}

br_scalar BrFloatToScalar(float f)
{
	return (br_scalar)f;
}

float BrScalarToFloat(br_scalar s)
{
	//FIXME("(%.4f): check implementation", s);
	return (float)s;
}

br_scalar BrFractionToScalar(br_fraction f)
{
	FIXME("(%.4f): check implementation", f);
	return (br_scalar)f;
}

br_ufraction BrScalarToUFraction(br_scalar s)
{
	FIXME("(%d): check implementation", s);
	return (br_ufraction)s;
}
