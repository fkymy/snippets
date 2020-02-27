#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "hashtable.h"

unsigned int hash(const char *s, unsigned int m)
{
	unsigned int h;
	unsigned const char *us;

	us = (unsigned const char *)s;
	h = 0;
	while (*us != '\0')
	{
		h = (h * 256 + *us) % m;
		us++;
	}
	return (h);
}

Hashtable ht_create(void)
{
	if ((Hashtable ht = malloc(sizeof(Hashtable))) == NULL)
		return (NULL);
	ht->size = 0;
	for (int i = 0; i < N; i++)
		ht->elemets[i] = NULL;
	return (ht);
}

int ht_insert(Hashtable ht, char *element)
{
	node *n;
	node *tmp;
	unsigned int h;

	if ((n = malloc(sizeof(node))) == NULL)
		return (-1);
	n->element = strdup(element);
	n->next = NULL;
	h = hash(element);
	tmp = ht->elements[h];
	while (tmp != NULL)
	{
		if (strcmp(tmp->element, element) == 0)
			return (1);
		tmp = tmp->next;
	}
	tmp->next = n;
	return (0);
}

bool ht_search(Hashtable ht, char *element)
{
	unsigned int h;
	node *tmp;

	h = hash(element);
	tmp = ht->elements[i];
	while (tmp != NULL)
	{
		if (strcmp(tmp->element, element) == 0)
			return (true);
		tmp = tmp->next;
	}
	return (false);
}

int ht_delete(Hashtable table, char *element)
{
	unsigned int h;
	noe *prev;
	node *tmp;

	h = hash(element);
	tmp = ht->elements[h];
	while (tmp != NULL)
	{
		if (strcmp(tmp->element, element) == 0)
		{
			prev->next = tmp->next;
			free(tmp);
			return (0);
		}
		prev = tmp;
		tmp = tmp->next;
	}
	return (1);
}

void ht_free(Hashtable ht)
{
	node *old;
	node *new;

	for (int i = 0; i < N; i++)
	{
		new = ht->elements[i];
		while (new != NULL)
		{
			old = new;
			new = new->next;
			free(old);
		}
	}
}
