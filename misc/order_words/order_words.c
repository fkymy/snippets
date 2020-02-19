// Order Words
// Create a program that lists words that appear in a string given by its length, alphabetically (unless aA, then order or appearance)
// e.g. ./order_alpha "stay awhile you are so beautiful You aaa" | cat -e
// so$
// aaa are you You$ stay$
// awhile$
// beautiful$

#include "order_words.h"

void	swap_dicts(t_dict *d1, t_dict *d2)
{
	t_dict temp;

	temp = *d1;
	*d1 = *d2;
	*d2 = temp;
}

void	order_words(t_dict *dicts, int(*cmp)(t_dict, t_dict))
{
	int i;
	int j;
	int size;

	size = 0;
	while (dicts[size].str)
		size++;

	i = 0;
	while (i < size - 1)
	{
		j = 0;
		while (j < size - 1 - i)
		{
			if ((*cmp)(dicts[j], dicts[j + 1]) > 0)
				swap_dicts(&dicts[j], &dicts[j + 1]);
			j++;
		}
		i++;
	}
}

void	print_dicts(t_dict *dicts)
{
	int i;
	int len;

	i = 0;
	len = 0;
	while (dicts[i].str)
	{
		if (dicts[i].len > len)
		{
			len = dicts[i].len;
			if (i != 0)
				write(1, "\n", 1);
		}
		write(1, dicts[i].str, dicts[i].len);
		if (dicts[i + 1].str && dicts[i + 1].len == dicts[i].len)
			write(1, " ", 1);
		i++;
	}
}

void	free_dicts(t_dict *dicts)
{
	int i;

	i = 0;
	while (dicts[i].str)
		free(dicts[i++].str);
	free(dicts);
}

int		main(int argc, char *argv[])
{
	t_dict	*dicts;

	if (argc == 2)
	{
		dicts = str_to_dict(argv[1]);
		order_words(dicts, compare_dicts);
		print_dicts(dicts);
		free_dicts(dicts);
	}
	write(1, "\n", 1);
	return (0);
}
