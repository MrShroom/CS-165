#include <stdlib.h>
void modded_quick_sort(int * , int , int, int);
void quick_sort(int *, int, int);
int partition(int *, int, int, int);
void swap(int *, int *);

int doalg(int n, int k, int *Best)
{
	printf("Quick\n");
	if (k > n)//check that we aren't asking for more than we get. 
	{
		return 0;
	}

	int *indexes = (int *) malloc(sizeof(int) *n);//creat heapspace

	//initialize index to trival assignment  
	int i = 0;
	for (; i < n; i++)
	{
		indexes[i] = i + 1;
	}

	modded_quick_sort(indexes, 0, n - 1, k);
	quick_sort(indexes, 0, k - 1);
	for (i = 0; i < k; i++)
	{
		Best[i] = indexes[i];
	}
	free(indexes);
	return 1;
}

void quick_sort(int * index_array, int left, int right)
{
	if (left >= right)
		return;
	int current_piviot_value = partition(index_array, left, right, (right +left)/2);
	quick_sort(index_array, left, current_piviot_value - 1);
	quick_sort(index_array, current_piviot_value + 1, right);
}

void modded_quick_sort(int * index_array, int left, int right, int k)
{
	if (left >= right)
		return ;
	int current_piviot_value = partition(index_array, left, right, k);
	if (current_piviot_value + 1 == k)
		return;
	if (current_piviot_value + 1 > k)
		modded_quick_sort(index_array, left, current_piviot_value - 1, k);
	else
		modded_quick_sort(index_array, current_piviot_value + 1, right, k);
}

int partition(int *index_array, int left, int right, int k)
{
	int pivot_index = index_array[k >= right ? left : k];
	while ( left < right)
	{
		while (COMPARE(index_array[left], pivot_index ) == 1)
		{			
			left++;
		}
		if (left < right)
			while (COMPARE( pivot_index,index_array[right]) == 1)
			{
				right--;
			}
		
		if(left < right)
			swap(&index_array[left], &index_array[right]);
	}
	
	return right;
}

void swap(int *a, int *b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}