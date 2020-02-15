void	insertion_sort(int *array, int n)
{
	int i;
	int j;
	int key;

	for (i = 1; i < n; i++)
	{
		key = array[i];
		j = i - 1;
		while (j >= 0 && array[j] > key)
		{
			array[j + 1] = array[j];
			j--;
		}
		array[j + 1] = key;
	}
}
