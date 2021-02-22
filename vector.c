#include <stdlib.h>
#include <string.h>

struct vector
{
	char *data;
	size_t size;
	size_t capacity;
};

typedef struct vector t_vector;

t_vector *vector_initialize(void)
{
	t_vector *v;

	if (!(v = malloc(sizeof(v))))
		return NULL;
	v->data = NULL;
	v->size = 0;
	v->capacity = 0;
	return v;
}

void	vector_append(t_vector *v, int c)
{
	size_t	new_capacity;
	char	*new_data;

	if (v->size == v->capacity)
	{
		new_capacity = v->capacity ? v->capacity * 2 : 8;
		new_data = malloc(new_capacity);
		memcpy(new_data, v->data, v->size);
		free(v->data);
		v->data = new_data;
		v->capacity = new_capacity;
	}
	v->data[v->size] = c;
	++v->size;
}

void	vector_appends(t_vector *v, char *s)
{
	if (!s)
		return ;
	while (*s)
		vector_append(v, *s++);
}

char	*vector_gets(t_vector *v)
{
	if (v->size == 0)
		vector_append(v, '\0');
	else if (v->data[v->size - 1] != '\0')
		vector_append(v, '\0');
	return (v->data);
}

void	vector_free(t_vector *v)
{
	free(v->data);
	v->data = NULL;
}
