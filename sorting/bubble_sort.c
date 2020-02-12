#include <stdio.h>

void	swap_elements(int *lhs, int *rhs)
{
	int tmp;

	tmp = *lhs;
	*lhs = *rhs;
	*rhs = tmp;
}

void	bubble_sort(int *array, int size)
{
	int i;
	int j;

	i = 0;
	while (i < size - 1)
	{
		j = 0;
		while (j < size - 1 - i)
		{
			if (array[j] > array[j + 1])
				swap_elements(&array[j], &array[j + 1]);
			j++;
		}
		i++;
	}
}

void	bubble_sort_until_no_swap(int *array, int size)
{
	// until no swap, to improve best case
	int i;
	int j;
	int did_swap;

	i = 0;
	while (i < size - 1)
	{
		j = 0;
		did_swap = 0;
		while (j < size - 1 - i)
		{
			if (array[j] > array[j + 1])
			{
				swap_elements(&array[j], &array[j + 1]);
				did_swap = 1;
			}
			j++;
		}
		if (!did_swap)
			return ;
		i++;
	}
}
