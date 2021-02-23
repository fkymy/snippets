#ifndef DICT_H
# define DICT_H

# define INITIAL_SIZE (1024)
# define GROWTH_FACTOR (2)
# define MAX_LOAD_FACTOR (1)

typedef struct dict *Dict;

struct elt {
	char *key;
	char *value;
	struct elt *next;
};

struct dict {
	int size;
	int n;
	struct elt **table;
};

Dict dict_create(void);

void dict_destroy(Dict d);

void dict_insert(Dict d, const char *key, const char *value);

const char *dict_search(Dict d, const char *key);

void dict_delete(Dict d, const char *key);

#endif
