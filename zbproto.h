#include "compiler.h"
#include "actor.h"
#include "model.h"
#include "material.h"
#include "matrix.h"

#ifndef _BRENDER_ZBPROTO_H_
#define _BRENDER_ZBPROTO_H_

typedef void br_renderbounds_cbfn(br_actor*, const br_model*, const br_material*, void*, br_uint_8, const br_matrix4*, const br_int_32 [4]);

#endif
