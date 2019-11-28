#include "compiler.h"

#ifndef _BRENDER_LIST_H_

typedef struct br_node br_node;
typedef struct br_list br_list;

struct br_node
{
	void *data;
	br_node *next;
};

struct br_list
{
	br_node *head;
};

#define ID(obj) (char *)*(int *)obj

br_uint_32 BrListInsert(br_list *list, void *node);
br_node *BrListFind(br_list *list, const char *identifier);
br_uint_32 BrListRemove(br_list *list, void *node);
void BrListFree(br_list *list);
#define  _BRENDER_LIST_H_
#endif
