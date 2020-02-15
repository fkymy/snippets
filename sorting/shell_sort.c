#include <stdio.h>

void	shell_sort(int arr[], int n)
{
	int h;
	int tmp;

	h = 1;
	while (h < n / 3)
		h = 3 * h + 1; // knuth's increment sequence
	while (h > 0)
	{
		// h-sort the array
		for (int i = h; i < n; i++)
		{
			for (int j = i - h; j >= 0 && arr[j] > arr[j + h]; j -= h)
			{
				tmp = arr[j];
				arr[j] = arr[j + h];
				arr[j + h] = tmp;
			}
		}
		h = h / 3;
	}
}
