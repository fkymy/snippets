#include <stdio.h>

void	swap(int *a, int *b)
{
	int tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

int		partition(int arr[], int start, int end)
{
	int pivot = arr[end];
	int pindex = start;
	for (int i = start; i < end; i++)
	{
		if (arr[i] <= pivot)
		{
			swap(&arr[i], &arr[pindex]);
			pindex++;
		}
	}
	swap(&arr[pindex], &arr[end]);
	return (pindex);
}

void	quick_sort(int arr[], int start, int end)
{
	if (start < end)
	{
		int pindex = partition(arr, start, end);
		quick_sort(arr, start, pindex - 1);
		quick_sort(arr, pindex + 1, end);
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
	quick_sort(arr, 0, n - 1);
	printf("Sorted array is: \n");
	print_array(arr, n);
	return (0);
}
