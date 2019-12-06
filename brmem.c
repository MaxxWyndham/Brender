#include "brender.h"
#include "brmem.h"

#include <SDL.h>
#include <stdlib.h>

br_uint_32 gBrIntScratchpadInUse = 0;
br_uint_32 gBrIntScratchpadRequestedSize = 0;
br_uint_32 gBrIntScratchpadSize = 0;
void* gBrIntScratchpad = 0;

void BrBlockFill(void* dest_ptr, int value, int dwords)
{
	memset(dest_ptr, value, dwords);
}

void BrBlockCopy(void* dest_ptr, const void* src_ptr, int dwords)
{
	memcpy(dest_ptr, src_ptr, dwords);
}

void *BrMemCalloc(int nelems, br_size_t size, br_uint_8 type)
{
	/* ignoring type for now */

	if ((nelems * size) == 0)
	{
		//WARN("Tried to calloc(%d, %d) of type %u", nelems, size, type);
		return NULL;
	}
	else
	{
		void *p = calloc(nelems, size);
		//INFO("%p\t+", p);
		return p;
	}
}

void *BrMemAllocate(br_size_t size, br_uint_8 type)
{
	/* ignoring type for now */

	if (size == 0)
	{
		//WARN("Tried to malloc(0) of type %u", type);
		return NULL;
	}
	else
	{
		void *p = calloc(1, size);
		//INFO("%p\t+", p);
		return p;
	}
}

char* BrMemStrDup(const char* str)
{
    char *dest = BrMemAllocate(strlen (str) + 1, BR_MEMORY_STRING);
    return (dest ? strcpy(dest, str) : NULL);
}

void BrMemFree(void* block)
{
	//INFO("%p\t-",block);
	free(block);
}

void BrAllocatorSet(br_allocator *a)
{
	FIXME("(%p): stub", a);
}

br_uint_32 BrMemReadBU32( char **pPtr )
{
	br_uint_32 raw_long;
	BrBlockCopy(&raw_long, *pPtr, 4);
	*pPtr += 4;
	return (BrBLtoHL( raw_long ));
}

br_uint_16 BrMemReadBU16( char **pPtr )
{
	br_uint_16	raw_short;
	BrBlockCopy(&raw_short, *pPtr, 2);
	*pPtr += 2;
	return (BrBStoHS( raw_short ));
}

br_uint_8 BrMemReadU8( char **pPtr )
{
	return (*((br_uint_8 *)(*pPtr)++));
}

br_int_32 BrMemReadBS32( char **pPtr )
{
	br_int_32	raw_long;
	BrBlockCopy(&raw_long, *pPtr, 4);
	*pPtr += 4;
	return (BrBLtoHL( raw_long ));
}

br_int_16 BrMemReadBS16( char **pPtr )
{
	br_int_16	raw_short;
	BrBlockCopy(&raw_short, *pPtr, 2);
	*pPtr += 2;
	return (BrBStoHS( raw_short ));
}

br_int_8 BrMemReadS8( char **pPtr )
{
	return (*((br_int_8 *)(*pPtr)++));
}

br_float BrMemReadBFloat( char **pPtr )
{
	br_float	raw_float;
	BrBlockCopy(&raw_float, *pPtr, 4);
	*pPtr += 4;
	return (BrBFtoHF( raw_float ));
}

char* BrMemReadString( char **pPtr )
{
    br_uint_32 l = strlen(*pPtr) + 1;

    /* modify source buffer so string is in uppercase */
    char *str = *pPtr;

    for (br_uint_8 i = 0; i+1 < l; i++)
    {
    	if (**pPtr >= 90 && **pPtr <= 122)
    	{
    		**pPtr -= 32;
    	}

    	*pPtr += 1;
    }

    //INFO("%s", str);

	*pPtr += 1;
    //*pPtr += l;
    return str;
}

void BrMemSkipBytes( char **pPtr, int pBytes_to_skip )
{
	*pPtr += pBytes_to_skip;
}

void* BrIntAllocateScratchpad(br_uint_32 pRequestedSize)
{
	if (gBrIntScratchpadInUse) {
		BR_ERROR("Scratchpad not available");
	}

	gBrIntScratchpadRequestedSize = pRequestedSize;

	if (gBrIntScratchpadSize < pRequestedSize) {
		if (gBrIntScratchpad) { BrResFree(gBrIntScratchpad); }

		gBrIntScratchpad = BrResAllocate(NULL, pRequestedSize, 1); // M00552a48
		gBrIntScratchpadSize = pRequestedSize;
	}

	gBrIntScratchpadInUse = 1;

	return gBrIntScratchpad;
}

void BrIntMemSet(void* dest_ptr, int value, int dwords) 
{
	/* Errol note...
	   The assembly first zeros using dword then ANDs dwords with 3 and zero using byte
	*/
	memset(dest_ptr, value, dwords);
}