#include "brender.h"
#include "list.h"

br_uint_32 BrListInsert(br_list *list, void *node)
{
	/* adds the entry at the beginning of the current list, not the end */

	br_node *n = BrMemCalloc(1, sizeof(br_node), 0);
	n->data = node;
	n->next = NULL;

	if (list->head != NULL)
	{
		n->next = list->head;
	}

	list->head = n;

	return 1;
}

br_node *BrListFind(br_list *list, const char *identifier)
{
	if (list->head == NULL)
		return NULL;

    br_node *i = list->head;

	while (i)
	{
		char *id = (char *)*(int *)i->data;

		if (id != NULL && strcmp(id, identifier) == 0)
			return i->data;

		i = i->next;
	}

    //WARN("Failed to find %s", identifier);

	return NULL;
}

br_uint_32 BrListRemove(br_list *list, void *node)
{
	if (list->head == NULL)
		return 0;

	br_node *i = list->head;
	br_node *l = NULL;

	while (i)
	{
		if (i->data != NULL && i->data == node)
		{
			if (l == NULL)
			{
				list->head = i->next;
			}
			else
			{
				l->next = i->next;
			}

			BrMemFree(i);
			return 1;
		}

		l = i;
		i = i->next;
	}

	WARN("Failed to remove %p from list %p", node, list);
	return 0;
}

void BrListFree(br_list *list)
{
	br_node *i = list->head;
	br_node *t = NULL;

	while (i)
	{
		t = i;
		i = i->next;
		BrMemFree(t);
	}

	BrMemFree(list);
}
