#include "compiler.h"

#ifndef _BRENDER_BRMEM_H_

br_uint_32 gBrIntScratchpadInUse = 0;
br_uint_32 gBrIntScratchpadRequestedSize = 0;
br_uint_32 gBrIntScratchpadSize = 0;
void* gBrIntScratchpad = 0;

enum {
	BR_MEMORY_SCRATCH,
	BR_MEMORY_PIXELMAP,
	BR_MEMORY_PIXELS,
	BR_MEMORY_VERTICES,
	BR_MEMORY_FACES,
	BR_MEMORY_GROUPS,
	BR_MEMORY_MODEL,
	BR_MEMORY_MATERIAL,
	BR_MEMORY_MATERIAL_INDEX,
	BR_MEMORY_ACTOR,
	BR_MEMORY_PREPARED_VERTICES,
	BR_MEMORY_PREPARED_FACES,
	BR_MEMORY_LIGHT,
	BR_MEMORY_CAMERA,
	BR_MEMORY_BOUNDS,
	BR_MEMORY_CLIP_PLANE,
	BR_MEMORY_STRING,
	BR_MEMORY_REGISTRY,
	BR_MEMORY_TRANSFORM,
	BR_MEMORY_RESOURCE_CLASS,
	BR_MEMORY_FILE,
	BR_MEMORY_ANCHOR,
	BR_MEMORY_POOL,
	BR_MEMORY_RENDER_MATERIAL,
	BR_MEMORY_DATAFILE,
	BR_MEMORY_PREPARED_MODEL
};

typedef void * BR_CALLBACK brmem_allocate_cbfn(br_size_t, br_uint_8);
typedef void BR_CALLBACK brmem_free_cbfn(void *);
typedef br_size_t BR_CALLBACK brmem_inquire_cbfn(br_uint_8);

typedef struct
{
	char				*identifier;
	brmem_allocate_cbfn	*allocate;
	brmem_free_cbfn		*free;
	brmem_inquire_cbfn	*inquire;
	br_intptr_t			padding;
} br_allocator;

void BrBlockFill(void* dest_ptr, int value, int dwords);
void BrBlockCopy(void* dest_ptr, const void* src_ptr, int dwords);
void *BrMemCalloc(int nelems, br_size_t size, br_uint_8 type);
void* BrMemAllocate(br_size_t size, br_uint_8 type);
char* BrMemStrDup(const char* str);
void BrMemFree(void* block);

void BrAllocatorSet(br_allocator *);

br_uint_32 BrMemReadBU32( char **pPtr );
br_uint_16 BrMemReadBU16( char **pPtr );
br_uint_8  BrMemReadU8( char **pPtr );
br_int_32  BrMemReadBS32( char **pPtr );
br_int_16  BrMemReadBS16( char **pPtr );
br_int_8   BrMemReadS8( char **pPtr );
br_float   BrMemReadBFloat( char **pPtr );
char*      BrMemReadString( char **pPtr );
void BrMemSkipBytes( char **pPtr, int pBytes_to_skip );

#define _BRENDER_BRMEM_H_
#endif
