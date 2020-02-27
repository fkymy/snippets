#ifndef HASHTABLE_H
#define HASHTABLE_H

#define N (1000)

#include <stdbool.h>

typedef struct node
{
	char *element;
	struct node *next;
} node;

typedef struct hashtable
{
	node *elements[MAXELEMENTS];
	unsigned int size;
} hashtable;

typedef struct hashtable *Hashtable;

Hashtable ht_create(void);

int ht_insert(Hashtable ht, char *element);

int ht_delete(Hashtable ht, char *element);

bool ht_search(Hashtable ht, char *element);

int ht_free(Hashtable ht);

#endif
