#include <stdio.h>

void	swap_elements(int *a, int *b)
{
	int tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

void	selection_sort(int *array, int size)
{
	int current;
	int min;
	int i;
	int j;

	i = 0;
	while (i < size - 1)
	{
		current = i;
		min = current;
		j = i;
		while (j < size)
		{
			if (array[j] < array[min])
				min = j;
			j++;
		}
		if (array[min] < array[current])
			swap_elements(&array[current], &array[min]);
		i++;
	}
}

int	main(void)
{
	int array[10] = { 4, 0, 9, 3, 2, 7, 6, 5, 8, 1 };
	selection_sort(array, 10);
	for (int i = 0; i < 10; i++)
		printf("%d ", array[i]);
	return (0);
}
