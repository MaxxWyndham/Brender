#include "compiler.h"
#include "brmem.h"

#ifndef _BRENDER_ARRAYLIST_H_

typedef struct ArrayList
{
	br_uint_32 capacity;
	br_uint_32 count;
	br_uint_32 element_size;
	br_intptr_t *data;
} ArrayList;

ArrayList *ArrayList_createInt32();
br_uint_32 ArrayList_addInt32(ArrayList *, br_int_32);
void ArrayList_setInt32(ArrayList *, br_uint_32 index, br_int_32);
br_int_32 ArrayList_getInt32(ArrayList *, br_uint_32 index);
br_int_32 ArrayList_find(ArrayList *,br_int_32);
// XXX: this pointer is valid until next add* call
br_int_32 *ArrayList_getInt32Ptr(ArrayList *);

br_uint_32 ArrayList_size(ArrayList *);
void ArrayList_ensureCapacity(ArrayList *, br_uint_32);
void ArrayList_clear(ArrayList *);
void ArrayList_free(ArrayList *);

#define _BRENDER_ARRAYLIST_H_
#endif
