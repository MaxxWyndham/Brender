#include "compiler.h"

#ifndef _BRENDER_BRRES_H_

#define BR_MAX_RESOURCE_CLASSES 256

typedef void br_resourcefree_cbfn;

typedef struct
{
	br_uint_8				res_class;
	br_resourcefree_cbfn	*free_cb;
} br_resource_class;

void* BrResAllocate(void* vparent, br_size_t size, int res_class);
char* BrResStrDup(void* vparent, const char* str);
br_resource_class* BrResClassAdd(br_resource_class* rclass);
void* BrResRemove(void* vres);
void BrResFree(void* vres);

#define _BRENDER_BRRES_H_
#endif
