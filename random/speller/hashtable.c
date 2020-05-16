#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "hashtable.h"

typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

const unsigned int N = 27*27*27;
char *alphas = "abcdefghijklmnopqrstuvwxyz\'";

node *table[N];
unsigned int g_size;

bool check(const char *word)
{
    unsigned int hash_value;

    hash_value = hash(word);
    node *current = table[hash_value];
    while (current != NULL)
    {
        if (strcasecmp(current->word, word) == 0)
            return true;
        current = current->next;
    }
    return false;
}

/* unsigned int hash(const char *word) */
/* { */
/*     unsigned int first; */
/*     unsigned int second; */
/*     unsigned int third; */
/*  */
/*     first = 0; */
/*     while (alphas[first]) */
/*     { */
/*         if (strncasecmp(&alphas[first], &word[0], 1) == 0) */
/*         { */
/*             if (word[1] == '\0') */
/*                 return (first); */
/*             else */
/*                 break; */
/*         } */
/*         first++; */
/*     } */
/*     second = 0; */
/*     while (alphas[second]) */
/*     { */
/*         if (strncasecmp(&alphas[second], &word[1], 1) == 0) */
/*         { */
/*             if (word[2] == '\0') */
/*                 return (first * 27 + second); */
/*             else */
/*                 break; */
/*         } */
/*         second++; */
/*     } */
/*     third = 0; */
/*     while (alphas[third]) */
/*     { */
/*         if (strncasecmp(&alphas[third], &word[2], 1) == 0) */
/*             return ((first * 27*27) + (second * 27) + third); */
/*         third++; */
/*     } */
/*  */
/*     printf("proper hash value not found for word: %s\n", word); */
/*     return 0; */
/* } */

unsigned int hash(const char *str)
{
	unsigned int hash = 0;
	int i = 0;
	while (str[i] != '\0')
	{
		hash = str[i] + (hash << 6) + (hash << 16) - hash;
		i++;
	}
	return hash % N;
}

bool load(const char *dictionary)
{
    FILE *file;
    char word[LENGTH + 1];
    node *n;
    unsigned int hash_value;

    if ((file = fopen(dictionary, "r")) == NULL)
        return false;

    while (fscanf(file, "%s", word) != EOF)
    {
        if (!(n = malloc(sizeof(node))))
        {
            printf("Error malloc");
            return false;
        }
        strcpy(n->word, word);
        n->next = NULL;

        hash_value = hash(word);
        if (table[hash_value] == NULL)
            table[hash_value] = n;
        else
        {
            n->next = table[hash_value];
            table[hash_value] = n;
        }
        g_size++;
    }

    if (ferror(file))
    {
        fclose(file);
        printf("Error reading dictionary: %s\n", dictionary);
        return false;
    }

    fclose(file);
    return true;
}

unsigned int size(void)
{
    return g_size;
}

bool unload(void)
{
    node *current;
    node *tmp;
    unsigned int i;

    i = 0;
    while (i < N)
    {
        current = table[i];
        while (current != NULL)
        {
            tmp = current;
            current = current->next;
            free(tmp);
        }
        i++;
    }

    return true;
}
