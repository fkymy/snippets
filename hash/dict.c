/* a string to string dict using chaining */

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "dict.h"

Dict internal_dict_create(int size)
{
	Dict d;
	int i;

	d = malloc(sizeof(*d));
	assert(d != 0);

	d->size = size;
	d->n = 0;
	d->table = malloc(sizeof(struct elt *) * d->size);
	assert(d->table != 0);

	for (i = 0; i < d->size; i++)
		d->table[i] = 0;
	return (d);
}

Dict dict_create(void)
{
	return internal_dict_create(INITIAL_SIZE);
}

void dict_destroy(Dict d)
{
	int i;
	struct elt *e;
	struct elt *next;

	for (i = 0; i < d->size; i++)
	{
		for (e = d->table[i]; e != 0; e = next)
		{
			next = e->next;
			free(e->key);
			free(e->value);
			free(e);
		}
	}
	free(d->table);
	free(d);
}

#define MULTIPLIER (97)

static unsigned long hash(const char *s)
{
	unsigned const char *us;
	unsigned long h;

	h = 0;
	for (us = (unsigned const char *)s; *us; us++)
		h = h * MULTIPLIER + *us;
	return (h);
}

static void grow(Dict d)
{
	Dict d2;
	struct dict swap;
	int i;
	struct elt *e;

	d2 = internal_dict_create(d->size * GROWTH_FACTOR);

	for (i = 0; i < d->size; i++)
		for (e = d->table[i]; e != 0; e = e->next)
			dict_insert(d2, e->key, e->value);

	swap = *d;
	*d = *d2;
	*d2 = swap;
	dict_destroy(d2);
}

void dict_insert(Dict d, const char *key, const char *value)
{
	static struct elt *e;
	unsigned long h;

	assert(key);
	assert(value);
	e = malloc(sizeof(*e));
	assert(e);
	e->key = strdup(key);
	e->value = strdup(value);
	h = hash(key) % d->size;
	e->next = d->table[h];
	d->table[h] = e;
	d->n++;
	if (d->n >= d->size * MAX_LOAD_FACTOR)
		grow(d);
}

const char *dict_search(Dict d, const char *key)
{
	struct elt *e;

	for (e = d->table[hash(key) % d->size]; e != 0; e = e->next)
		if (!strcmp(e->key, key))
			return (e->value);
	return (0);
}

void dict_delete(Dict d, const char *key)
{
	struct elt **prev;
	struct elt *e;

	for (prev = &(d->table[hash(key) % d->size]); *prev != 0; prev = &((*prev)->next))
	{
		if (!strcmp((*prev)->key, key))
		{
			e = *prev;
			*prev = e->next;
			free(e->key);
			free(e->value);
			free(e);
			return ;
		}
	}
}
