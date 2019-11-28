#include "compiler.h"
#include "scalar.h"
#include "colour.h"
#include "pixelmap.h"
#include "matrix.h"
#include "token.h"

#ifndef _BRENDER_MATERIAL_H_

#define BR_MATF_LIGHT			(1 << 0)	/* correct */
#define BR_MATF_PRELIT			(1 << 1)	/* correct */
#define BR_MATF_SMOOTH			(1 << 2)	/* correct */
#define BR_MATF_ENVIRONMENT_I	(1 << 3)
#define BR_MATF_ENVIRONMENT_L	(1 << 4)
#define BR_MATF_PERSPECTIVE		(1 << 5)
#define BR_MATF_DECAL			(1 << 6)
#define BR_MATF_BLEND			(1 << 7)
#define BR_MATF_ALWAYS_VISIBLE	(1 << 8)
#define BR_MATF_TWO_SIDED		(1 << 9)	/* correct */
#define BR_MATF_FORCE_FRONT		(1 << 10)
#define BR_MATF_DITHER			(1 << 11)

#define BR_MATU_MAP_TRANSFORM	(1 << 0)
#define BR_MATU_RENDERING		(1 << 1)	/* correct */
#define BR_MATU_LIGHTING		(1 << 2)
#define BR_MATU_COLOURMAP		(1 << 3)
#define BR_MATU_ALL				(0x7fff)

typedef struct br_material
{
    char			*identifier;

	br_uint_32		flags;
	br_ufraction	ka;
	br_ufraction	kd;
	br_ufraction	ks;
	br_scalar		power;

	br_colour		colour;
	br_uint_8		index_base;
	br_uint_8		index_range;
	br_pixelmap		*index_shade;
	br_pixelmap		*index_blend;

	br_pixelmap		*colour_map;
	br_matrix23		map_transform;

	br_token_value	*extra_prim; // used by the game, not documented

	void			*user;
} br_material;

typedef br_uint_32 BR_CALLBACK br_material_enum_cbfn(br_material* mat, void* arg);

br_material* BrMaterialAllocate(const char* name);
br_material* BrMaterialAdd(br_material* material);
void BrMaterialUpdate(br_material* material, br_uint_16 flags);
br_uint_32 BrMaterialEnum(const char* pattern, br_material_enum_cbfn* callback, void* arg);
br_uint_32 BrMaterialLoadMany(const char* filename, br_material** materials, br_uint_16 num);
br_uint_32 BrMaterialAddMany(br_material* const* materials, int n);
br_material* BrMaterialFind(const char* pattern);
br_material* BrMaterialLoad(const char* filename);
br_material* BrMaterialRemove(br_material* material);
void BrMaterialFree(br_material* m);
br_uint_32 BrMaterialFileCount(const char* filename, br_uint_16* num);

br_material* BrDefaultMaterial();

#define _BRENDER_MATERIAL_H_
#endif
