#include "compiler.h"
#include "colour.h"
#include "scalar.h"
#include "angles.h"

#ifndef _BRENDER_LIGHT_H_

enum {
	BR_LIGHT_DIRECT,
	BR_LIGHT_POINT,
	BR_LIGHT_SPOT
};

#define BR_MAX_LIGHTS 256

typedef struct
{
	br_uint_8	type;
	br_colour	colour;

	br_scalar	attenuation_c;
	br_scalar	attenuation_l;
	br_scalar	attenuation_q;

	br_angle	cone_inner;
	br_angle	cone_outer;

	char		*identifier;
	void		*user;
} br_light;

#define _BRENDER_LIGHT_H_
#endif
