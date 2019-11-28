#include "arraylist.h"
#include <assert.h>

ArrayList *ArrayList_create(br_uint_32 element_size, br_uint_32 capacity)
{
	ArrayList *al = BrMemAllocate(sizeof(ArrayList), BR_MEMORY_SCRATCH);

	al->capacity		= capacity;
	al->element_size	= element_size;
	al->count			= 0;
	al->data			= BrMemAllocate(element_size * capacity, BR_MEMORY_SCRATCH);

	return al;
}

ArrayList *ArrayList_createInt32()
{
	return ArrayList_create(sizeof(br_int_32), 10);
}

br_uint_32 ArrayList_size(ArrayList *al)
{
	return al->count;
}

void ArrayList_ensureCapacity(ArrayList *al, br_uint_32 reqCapacity)
{
	if (reqCapacity > al->capacity) {

		// double capacity until we have enough room, this is quite aggressive but fast
		while (al->capacity < reqCapacity)
			al->capacity *= 2;

		void *tmp = BrMemAllocate(al->element_size * al->capacity, BR_MEMORY_SCRATCH);
		BrBlockCopy(tmp, al->data, al->element_size * al->count);
		BrMemFree(al->data);
		al->data = tmp;
	}
}

br_uint_32 ArrayList_addInt32(ArrayList *al, br_int_32 val)
{
	ArrayList_ensureCapacity(al, al->count + 1);
	((br_int_32 *)al->data)[al->count++] = val;
	return al->count;
}

void ArrayList_setInt32(ArrayList *al, br_uint_32 index, br_int_32 val)
{
    ((br_int_32 *)al->data)[index] = val;
}

br_int_32 ArrayList_getInt32(ArrayList *al, br_uint_32 index)
{
	assert(index < al->count);
	return ((br_int_32 *)al->data)[index];
}

br_int_32 ArrayList_find(ArrayList *al, br_int_32 val)
{
    for (br_int_32 i = 0; i < al->count; i++) {
        if (((br_int_32 *)al->data)[i] == val) { return i; }
    }

    return -1;
}

br_int_32 *ArrayList_getInt32Ptr(ArrayList *al)
{
	return (br_int_32 *)al->data;
}

void ArrayList_clear(ArrayList *al)
{
	al->count = 0;
}

void ArrayList_free(ArrayList *al)
{
	BrMemFree(al->data);
	BrMemFree(al);
}
