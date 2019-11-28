#include "brender.h"
#include "brres.h"

void* BrResAllocate(void* vparent, br_size_t size, int res_class)
{
    if (size == 0) { return NULL; }
	/* Should probably do something with vparent. */
	/* Maybe keep track of resources allocated to each item for freeing purposes */
	/* http://stackoverflow.com/questions/3483684/freeing-memory-allocated-for-a-tree-c */
	return BrMemCalloc(1, size, res_class);
}

char* BrResStrDup(void* vparent, const char* str)
{
	return BrMemStrDup((strlen(str) == 0 ? "NO_IDENTIFIER\0" : str));
}

br_resource_class* BrResClassAdd(br_resource_class* rclass)
{
#if 0
	FIXME("(%p): stub", rclass);
#endif
	// the game does not care about the return value, except that it succeeds
	return (br_resource_class *)BR_TRUE;
}

void* BrResRemove(void* vres)
{
	FIXME("(%p): stub", vres);
	return NULL;
}

void BrResFree(void* vres)
{
	//INFO("%p", vres);
	BrMemFree(vres);
}
