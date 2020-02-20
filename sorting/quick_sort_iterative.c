#include <stdio.h>

void	swap(int *a, int *b)
{
	int tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

int		partition(int *arr, int start, int end)
{
	int pivot = arr[end];
	int i = start;
	for (int j = start; j < end; j++)
	{
		if (arr[j] <= pivot)
		{
			swap(&arr[i], &arr[j]);
			i++;
		}
	}
	swap(&arr[i], &arr[end]);
	return (i);
}

void	quick_sort_iterative(int arr[], int start, int end)
{
	int stack[end - start + 1];
	int top;

	top = -1;
	stack[++top] = start;
	stack[++top] = end;
	while (top >= 0)
	{
		end = stack[top--];
		start = stack[top--];
		int p = partition(arr, start, end);
		if (p - 1 > start)
		{
			stack[++top] = start;
			stack[++top] = p - 1;
		}
		if (p + 1 < end)
		{

			stack[++top] = p + 1;
			stack[++top] = end;
		}
	}
}

void	print_array(int arr[], int n)
{
	for (int i = 0; i < n; i++)
		printf("%d ", arr[i]);
	printf("\n");
}

int	main(void)
{
	int arr[10] = { 4, 0, 9, 3, 2, 7, 6, 5, 8, 1 };
	int n = sizeof(arr) / sizeof(arr[0]);
	printf("Given array is: \n");
	print_array(arr, n);
	quick_sort_iterative(arr, 0, n - 1);
	printf("Sorted array is: \n");
	print_array(arr, n);
	return (0);
}
