#include <float.h>

#ifndef _BRENDER_SCALAR_H_

typedef float br_scalar;
typedef float br_fraction;
typedef float br_ufraction;

br_scalar BrIntToScalar(int i);
int BrScalarToInt(br_scalar s);
br_scalar BrFloatToScalar(float f);
float BrScalarToFloat(br_scalar s);
br_scalar BrFractionToScalar(br_fraction f);
br_ufraction BrScalarToUFraction(br_scalar s);

#define BR_MAC2(a,b,c,d)	BR_ADD(BR_MUL(a, b), BR_MUL(c, d))
#define BR_MAC3(a,b,c,d,e,f) BR_ADD(BR_MUL(a, b), BR_ADD(BR_MUL(c, d), BR_MUL(e, f)))
#define BR_MAC4(a,b,c,d,e,f,g,h) BR_ADD(BR_MAC2(a,b,c,d), BR_MAC2(e,f,g,h))
#define BR_LENGTH2(a,b)		BR_SQRT(BR_SQR2(a, b))

#define BR_SCALAR_EPSILON	FLT_MIN
#define BR_SCALAR_MAX		FLT_MAX
#define BR_SCALAR_MIN		-FLT_MAX

#define BR_SCALAR(x)		((br_scalar)(x))
#define BR_FRACTION(x)		(br_fraction)(x)
#define BR_UFRACTION(x)		(br_ufraction)(x)

#define BR_ADD(a,b)			(br_scalar)((br_scalar)(a) + (br_scalar)(b))
#define BR_SUB(a,b)			(br_scalar)((br_scalar)(a) - (br_scalar)(b))
#define BR_CONST_MUL(a,x)	(br_scalar)((br_scalar)(a) * (x))
#define BR_MUL(a,b)			(br_scalar)((br_scalar)(a) * (br_scalar)(b))
#define BR_MULDIV(a,b,c)	BR_DIV(BR_MUL(a, b), c)
#define BR_SQR(a)			BR_MUL(a, a)
#define BR_SQR2(a,b)		BR_ADD(BR_SQR(a), BR_SQR(b))
#define BR_SQR3(a,b,c)		BR_ADD(BR_SQR(a), BR_ADD(BR_SQR(b), BR_SQR(c)))
#define BR_RCP(a)			BR_DIV(1.0f, a)
#define BR_CONST_DIV(a,x)	(br_scalar)((br_scalar)(a) / (x))
#define BR_DIV(a,b)			(br_scalar)((br_scalar)(a) / (br_scalar)(b))
#define BR_DIVR(a,b)		((a) / (b))
#define BR_ABS(a)			(br_scalar)fabs((float)a)
#define BR_POW(a,b)			(br_scalar)pow((float)a,(float)b)
#define BR_SQRT(a)			(br_scalar)sqrt((float)a)

#define BR_MIN(a,b)			( ( (a) < (b) ) ? (a) : (b) )
#define BR_MAX(a,b)			( ( (a) > (b) ) ? (a) : (b) )

#define _BRENDER_SCALAR_H_
#endif
