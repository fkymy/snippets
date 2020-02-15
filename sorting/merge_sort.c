#include <stdio.h>

void	merge(int arr[], int l, int m, int r)
{
	int i;
	int j;
	int k;
	int n1 = m - l + 1;
	int n2 = r - m;
	int L[n1];
	int R[n2];

	i = 0;
	while (i < n1)
	{
		L[i] = arr[l + i];
		i++;
	}
	j = 0;
	while (j < n2)
	{
		R[j] = arr[m + 1 + j];
		j++;
	}

	i = 0;
	j = 0;
	k = l;
	while (i < n1 && j < n2)
	{
		if (L[i] <= R[j])
			arr[k] = L[i++];
		else
			arr[k] = R[j++];
		k++;
	}
	while (i < n1)
		arr[k++] = L[i++];
	while (j < n2)
		arr[k++] = R[j++];
}

void	merge_sort(int arr[], int l, int r)
{
	int m;

	if (r > l)
	{
		m = l + (r - l) / 2;
		merge_sort(arr, l, m);
		merge_sort(arr, m + 1, r);
		merge(arr, l, m, r);
	}
}

void	print_array(int arr[], int size)
{
	int i;

	for (i = 0; i < size; i++)
		printf("%d ", arr[i]);
	printf("\n");
}

int	main(void)
{
	int arr[10] = { 4, 0, 9, 3, 2, 7, 6, 5, 8, 1 };
	int size = sizeof(arr) / sizeof(arr[0]);
	printf("size: %d\n", size);
	printf("Given array is: \n");
	print_array(arr, size);
	merge_sort(arr, 0, size - 1);
	printf("Sorted array is: \n");
	print_array(arr, size);
	return (0);
}
