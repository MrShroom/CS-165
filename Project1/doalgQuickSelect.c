#include <stdlib.h>
void quick_select(int * , int , int, int);
void quick_sort(int *, int, int);
int partition(int *, int, int, int, int);
void swap(int *, int *);
void heapify(int * heap, int n, int i);
void buildHeap(int *heap, int size);
int popFromHeap(int *heap, int *heapsize);

int doalg(int n, int k, int *Best)
{
	static int seen = 0;//Delete me later
	if (seen == 0)
	{
		printf("quick select\n");
		seen = 1;
	}

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

	quick_select(indexes, 0, n - 1, k);
	Best[k-1] = indexes[k-1];
	buildHeap(indexes, k - 1);
	int kmin1 = k - 1;
	for (i = 0; i < k - 2; i++)
	{
		Best[i] = popFromHeap(indexes, &kmin1);
	}
	Best[k-2] = indexes[0];
	free(indexes);
	return 1;
}

void quick_sort(int * index_array, int left, int right)
{
	if (left >= right)
		return;
	int pivot = left + (int)(dshrandom(0)*(right - left));
	int current_piviot_value = partition(index_array, left, right, (right +left)/2, pivot);
	quick_sort(index_array, left, current_piviot_value - 1);
	quick_sort(index_array, current_piviot_value + 1, right);
}

void quick_select(int * index_array, int left, int right, int k)
{
	if (left >= right)
		return ;
	int pivot = left + (int)(dshrandom(0)*(right - left));
	int current_piviot_value = partition(index_array, left, right, k, pivot);
	if (current_piviot_value + 1 == k)
		return;
	if (current_piviot_value + 1 > k)
		quick_select(index_array, left, current_piviot_value - 1, k);
	else
		quick_select(index_array, current_piviot_value + 1, right, k);
}

int partition(int *index_array, int left, int right, int k, int pivot)
{
	/*
	while ( left < right)
	{
		while (COMPARE(index_array[left], pivot ) == 1)
		{			
			left++;
		}
		if (left < right)
			while (COMPARE( pivot,index_array[right]) == 1)
			{
				right--;
			}
		
		if(left < right)
			swap(&index_array[left], &index_array[right]);
	}
	
	return right;
	*/
	int pivotValue = index_array[pivot];
	swap(&index_array[pivot], &index_array[right]);
	int storeIndex = left;
	int i = left;
	for (; i < right; i++) {
		if (COMPARE(index_array[i], pivotValue) == 1) {
			swap(&index_array[storeIndex], &index_array[i]);
			storeIndex++;
		}
	}
	swap(&index_array[right], &index_array[storeIndex]);
	return storeIndex;
}

void swap(int *a, int *b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}


// ******** TESTING **********
// adding heap sort to see if we reduce our amount of compares


void heapify(int * heap, int n, int i)
{
	
	int left = 2 * i + 1;
	int right = 2 * i + 2;
	int largest = i;

	if (left < n  && COMPARE(heap[left], heap[i]) == 1)
		largest = left;
	if (right < n  && COMPARE(heap[right], heap[largest]) == 1)
		largest = right;

	int temp;
	if (i != largest)
	{
		temp = heap[i];
		heap[i] = heap[largest];
		heap[largest] = temp;
		heapify(heap, n, largest);
	}
}

void buildHeap(int *heap, int size)
{
	int i = size / 2;
	while (i-->0)
		heapify(heap, size, i);
}

int popFromHeap(int *heap, int *heapsize)
{
	int out = heap[0];
	heap[0] = heap[--(*heapsize)];
	heapify(heap, *heapsize, 0);
	return out;
}