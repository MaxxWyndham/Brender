#include "compiler.h"
#include "brmem.h"
#include "brres.h"
#include "brhton.h"
#include "camera.h"
#include "brfile.h"
#include "pixelmap.h"
#include "transform.h"
#include "actor.h"
#include "light.h"
#include "brfont.h"
#include "register.h"
#include "list.h"
#include "zbproto.h"

#define vBRENDER 130

/* for ERROR */
#include <stdlib.h>

/* I have no idea what this is, threw in some value */
#define BR_MAX_CLIP_PLANES 256

#ifdef _DEBUG
	#include <stdio.h>
	#define FIXME(...) debug_printf("[FIXME] %s ", __FUNCTION__, __VA_ARGS__)
	#define WARN(...) debug_printf("[WARN] %s ", __FUNCTION__, __VA_ARGS__)
	#define ERR(...) fatal_printf("[ERROR] %s ", __FUNCTION__, __VA_ARGS__)
	#define INFO(...) debug_printf("[INFO] %s ", __FUNCTION__, __VA_ARGS__)

	#define BR_ERROR(...) fatal_printf("[BR_ERROR] %*s", "", __VA_ARGS__)
	#define BR_FATAL1(...) fatal_printf("[BR_FATAL1] %*s", "", __VA_ARGS__)

	void debug_printf(const char *fmt, const char *fn, const char *fmt2, ...);
	void fatal_printf(const char *fmt, const char *fn, const char *fmt2, ...);
#else
	#define FIXME(...)
	#define WARN(...)
	#define ERR(...) exit(1)
	#define INFO(...)

	#define BR_ERROR(...) exit(1)
	#define BR_FATAL1(...) exit(1)
#endif

#define br_errorhandler void*

#define BR_FOR_SIMPLELIST(l, i) \
	for (i = *l; i != NULL; i = i->next)

// I'll keep these here until we figure out wtf
#define BrIntToFixed(i)	i
#define BRT_BLEND_B		-1
#define BRT_OPACITY_X	-1

extern br_list *reg_models;
extern br_list *reg_materials;
extern br_list *reg_textures;
extern br_list *reg_tables;

extern br_material	*DEFAULT_MATERIAL;
extern br_model		*DEFAULT_MODEL;

void BrBegin(void);
void BrEnd(void);
br_uint_8 BrOnScreenCheck(const br_bounds* bounds);

void BrZbBegin(br_uint_8 colour_type, br_uint_8 depth_type);
void BrZbSceneRenderBegin(br_actor* world, br_actor* camera, br_pixelmap* colour_buffer, br_pixelmap* depth_buffer);
void BrZbSceneRender(br_actor* world, br_actor* camera, br_pixelmap* colour_buffer, br_pixelmap* depth_buffer);
void BrZbSceneRenderAdd(br_actor* tree);
void BrZbSceneRenderEnd(void);
void BrZbModelRender(br_actor* actor, br_model* model, const br_material* material, br_uint_8 style, int on_screen, int use_custom);
void BrZbEnd(void);

void BrExtGLLoadMatrix(br_matrix4* mat);
void BrExtGLMultMatrix(br_matrix4* mat);
