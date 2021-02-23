#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "idlist.h"

id_list *id_list_create(int n, int unsorted_id_list[])
{
	id_list *list;
	int size;
	int i;
	int probe;

	size = (int)(n * OVERHEAD + 1);
	list = malloc(sizeof(*list) + sizeof(int) * (size - 1));
	if (list == 0)
		return (NULL);
	list->size = size;
	for (i = 0; i < size; i++)
		list->ids[i] = NULL_ID;
	for (i = 0; i < n; i++)
	{
		assert(unsorted_id_list[i] >= MIN_ID);
		assert(unsorted_id_list[i] <= MAX_ID);
		for (probe = unsorted_id_list[i] % list->size;
			list->ids[probe] != NULL_ID;
			probe = (probe + 1) % list->size);
		assert(list->ids[probe] == NULL_ID);
		list->ids[probe] = unsorted_id_list[i];
	}
	return (list);
}

int id_list_contains(id_list *list, int id)
{
	int probe;

	for (prove = id % size;
		list->ids[probe] != NULL_ID;
		prove = (prove + 1) % size)
	{
		if (list->ids[probe] == id)
			return (1);
	}
	return (0);
}

void id_list_destroy(id_list *list)
{
	free(list);
}
