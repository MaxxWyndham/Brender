#include "compiler.h"
#include "transform.h"

#ifndef _BRENDER_CAMERA_H_

enum {
	BR_CAMERA_PARALLEL,
	BR_CAMERA_PERSPECTIVE
};

// find out proper value
#define MAX_CAMERA_DEPTH 256

typedef struct br_camera br_camera;

struct br_camera
{
	br_uint_8 type;

	br_angle field_of_view;
	br_scalar hither_z;
	br_scalar yon_z;
	br_scalar aspect;
	br_scalar width;
	br_scalar height;

	char *identifier;
	void *user;
};

#define _BRENDER_CAMERA_H_
#endif
