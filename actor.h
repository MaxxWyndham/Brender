#include "compiler.h"
#include "transform.h"
#include "model.h"
#include "material.h"

#ifndef _BRENDER_ACTOR_H_

enum {
	BR_ACTOR_NONE,
	BR_ACTOR_MODEL,
	BR_ACTOR_LIGHT,
	BR_ACTOR_CAMERA,
	BR_ACTOR_BOUNDS,
	BR_ACTOR_BOUNDS_CORRECT,
	BR_ACTOR_CLIP_PLANE
};

enum {
	BR_RSTYLE_DEFAULT,
	BR_RSTYLE_NONE,				/* correct */
	BR_RSTYLE_POINTS,
	BR_RSTYLE_EDGES,
	BR_RSTYLE_FACES,			/* correct */
	BR_RSTYLE_BOUNDING_POINTS,
	BR_RSTYLE_BOUNDING_EDGES,
	BR_RSTYLE_BOUNDING_FACES
};

typedef struct br_actor br_actor;

struct br_actor
{
	br_actor *next;			/* sibling */
	br_actor **prev;		/* sibling */
	br_actor *children;
	br_actor *parent;

	br_uint_16 depth;
	br_uint_8 type;

	char *identifier;
	br_model *model;
	br_material *material;

	br_uint_8 render_style;

	br_transform t;

	void *type_data;
	void *user;
};

// using custom br_intptr_t instead of br_uint32_t for 64 bit compatibility
typedef br_intptr_t BR_CALLBACK br_actor_enum_cbfn(struct br_actor *, void *);

br_actor *BrActorAllocate(br_uint_8 actor_type, void* type_data);
br_actor* BrActorAdd(br_actor* parent, br_actor* a);
br_uint_32 BrActorEnum(br_actor* parent, br_actor_enum_cbfn* callback, void* arg);
br_bounds* BrActorToBounds(br_bounds* b, const br_actor* ap);
br_actor* BrActorRemove(br_actor* a);
br_uint_8 BrActorToActorMatrix34(br_matrix34* m, const br_actor* a, const br_actor* b);
void BrActorRelink(br_actor* parent, br_actor* a);
void BrActorFree(br_actor* a);
br_uint_32 BrActorSave(const char* filename, const br_actor* actor);
br_uint_32 BrActorFileCount(const char* filename, br_uint_16* num);
br_actor* BrActorLoad(const char* filename);
br_uint_32 BrActorLoadMany(const char* filename, br_actor** actors, br_uint_16 num);

void BrLightEnable(br_actor* l);
br_actor* BrEnvironmentSet(br_actor* a);
void BrClipPlaneEnable(br_actor* cp);
void BrClipPlaneDisable(br_actor* cp);

#define _BRENDER_ACTOR_H_
#endif
