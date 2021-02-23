#include <stdio.h>
#include <assert.h>

#include "dict.h"

int main()
{
	Dict d;
	// char buf[512];
	// int i;

	d = dict_create();

	dict_insert(d, "hello", "there");
	printf("%s\n", dict_search(d, "hello"));
	dict_insert(d, "hello", "my friend");
	printf("%s\n", dict_search(d, "hello"));
	dict_delete(d, "hello");
	printf("%s\n", dict_search(d, "hello"));
	dict_delete(d, "hello");
	assert(dict_search(d, "hello") == 0);

	dict_destroy(d);
	return (0);
}
