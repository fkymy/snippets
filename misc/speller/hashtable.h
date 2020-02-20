#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdbool.h>

#define LENGTH 45
#define MAXELEMENTS 1000

bool check(const char *word);
unsigned int hash(const char *word);
bool load(const char *dictionary);
unsigned int size(void);
bool unload(void);

#endif
