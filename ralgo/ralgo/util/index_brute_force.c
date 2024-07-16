#include <ralgo/util/index_brute_force.h>

void index_brute_force_init(int * arr, int arrsize)
{
	for (int i = 0; i < arrsize; ++i)
		arr[i] = i;
}


int index_brute_force_next(int * arr, int arrsize, int elements)
{
	int roof = elements - 1;

	for (int moved_index = arrsize - 1; moved_index >= 0; --moved_index)
	{
		if (arr[moved_index] == roof)
		{
			roof = roof - 1;
			continue;
		}

		else
		{
			arr[moved_index]++;

			for (int i = moved_index+1; i < arrsize; ++i)
				arr[i] = arr[moved_index] + (i-moved_index);

			return 0;
		}
	}

	return -1;
}
