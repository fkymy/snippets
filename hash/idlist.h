#ifndef IDLIST_H
#define IDLIST_H

#define MIN_ID (0)
#define MAX_ID (999999999)

typedef struct id_list
{
	int size;
	int ids[1];
} id_list;

/* overhead parameter that determines both space and search costs */
/* must be strictly greater than 1 */
#define OVERHEAD (1.1)
#define NULL_ID (-1)

id_list *id_list_create(int n, int unsorted_id_list[]);
int id_list_contains(id_list *list);
void id_list_destroy(id_list *list);

#endif
