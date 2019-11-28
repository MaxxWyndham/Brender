#include "arraylist.h"
#include "compiler.h"
#include "material.h"
#include "scalar.h"
#include "vector.h"

#ifndef _BRENDER_MODEL_H_

typedef void *br_model_custom_cbfn; /* implement this only if required */

#define BR_MODF_DONT_WELD		(1 << 0)    // validated
#define BR_MODF_KEEP_ORIGINAL	(1 << 1)    // validated!
#define BR_MODF_GENERATE_TAGS	(1 << 2)    // validated
#define BR_MODF_QUICK_UPDATE	(1 << 3)    // not used by game
#define BR_MODF_CUSTOM			(1 << 5)    // validated
#define BR_MODF_UPDATEABLE		(1 << 7)    // validated!
#define BR_MODF_UNKNOWN			(1 << 8)    // used internally
#define BR_MODF_ALL             (0x7fff)
#define BR_MODF_ERROR			(1 << 15)	// used internally

#define BR_MODU_VERTEX_POSITIONS	(1 << 0) // validated
#define BR_MODU_VERTEX_COLOURS		(1 << 1) // validated
#define BR_MODU_FACES			    (1 << 2)
#define BR_MODU_MATERIAL			(1 << 3)
#define BR_MODU_RADIUS				(1 << 4)
#define BR_MODU_BOUNDING_BOX		(1 << 5)
#define BR_MODU_VERTICES			(1 << 6)
#define BR_MODU_EDGES				(1 << 7) // Both edges and groups are 0x80
#define BR_MODU_GROUPS				(1 << 7) //
#define BR_MODU_ALL					(0x7fff)

#define BR_FACEF_COPLANAR_0		(1 << 0)
#define BR_FACEF_COPLANAR_1		(1 << 1)
#define BR_FACEF_COPLANAR_2		(1 << 2)

typedef struct br_vertex_prep {
	br_vector3 p;
	br_vector2 map;
	br_vector3 n;
} br_vertex_prep;

typedef struct br_face_prep {
	br_uint_16 vertices[3];
	br_uint_16 edges[3];
    br_vector4 eqn;
} br_face_prep;

typedef struct br_group {
	void			*stored;

	br_face_prep	*faces;
    br_colour		*face_colours;
    br_uint_16		*face_user;

	br_vertex_prep	*vertices;
    br_colour		*vertex_colours;
    br_uint_16		*vertex_user;

	br_uint_16		nfaces;
	br_uint_16		nvertices;
	br_uint_16		nedges;
} br_group;

/* I have no idea what this structure should be called */
typedef struct br_model_prep {
	br_size_t	size;
	br_uint_32	flags;
	br_uint_16	ngroups;
	br_vector3	pivot;

	br_group	*groups;
} br_model_prep;

typedef struct br_face
{
	br_uint_16				vertices[3];	// 0
	br_uint_16				flags;			// 6
	br_material				*material;		// 8

	br_scalar				padding1;		// c

	br_uint_16				smoothing;		// 10
	
	br_uint_16				padding2;		// 12
	br_scalar				padding3;		// 14

	br_vector3				n;				// 18
	br_scalar				d;				// 24
} br_face;

typedef struct br_vertex
{
	br_vector3				p;
	br_vector2				map;
	br_uint_8				index;

	br_uint_8				red;
	br_uint_8				grn;
	br_uint_8				blu;

	br_scalar				padding[4];
} br_vertex;

typedef struct br_model
{
	br_scalar				padding;		// 0

    char					*identifier;    // 4
    br_vertex				*vertices;      // 8
    br_face					*faces;         // C

    br_uint_16				nvertices;      // 10
    br_uint_16				nfaces;         // 12

    br_vector3				pivot;          // 14

	br_uint_16				flags;          // 20

	br_model_custom_cbfn	*custom;        // 22 not sure about the order of these two
	void					*user;          // 26

	// 6 more padding?

	br_scalar				radius;         // 30
	br_bounds				bounds;         // 34

	br_model_prep			*prepared;      // 4C
} br_model;

br_model* BrModelAllocate(const char* name, int nvertices, int nfaces);
void BrModelUpdate(br_model* model, br_uint_16 flags);
br_uint_32 BrModelFileCount(const char* filename, br_uint_16* num);
br_model* BrModelLoad(const char* filename);
br_uint_32 BrModelLoadMany(const char* filename, br_model** models, br_uint_16 num);
br_uint_32 BrModelSaveMany(const char* filename, const br_model* const* models, br_uint_16 num);
br_uint_32 BrModelAddMany(br_model* const* models, int n);
br_model* BrModelAdd(br_model* model);
br_model* BrModelFind(const char* pattern);
br_model* BrModelRemove(br_model* model);
void BrModelFree(br_model* m);

void BrIntCalculatePlaneEquation(br_vector4* plane, br_vector3* v1, br_vector3* v2, br_vector3* v3);
void BrIntSort(void *base, size_t nitems, size_t size, int(*compar)(const void *, const void*));
void BrIntSomethingSomething(br_model* model);

br_model* BrDefaultModel();

#define _BRENDER_MODEL_H_
#endif
