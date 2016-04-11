#include <stdlib.h>
void heapify(int *, int, int); // make the ith element the root of a heap
void buildHeap(int *, int);// makes array a heap represntation 
int popFromHeap(int *, int *);// pops the max value from the heap

int doalg(int n, int k, int *Best)
{
	static int seen = 0;//Delete me later
	if (seen == 0)
	{
		printf("Min heap of Maxes\n");
		seen = 1;
	}

	if (k > n)//check that we aren't asking for more than we get. 
	{
		return 0;
	}

	int *myHeap = (int *)malloc(sizeof(int) * k);
	int heapsize = k;//track heap size

	//initialize heap to first k elements  
	int i = 1;
	for (; i <= k; i++)
	{
		myHeap[i-1] = i ;
	}

	buildHeap(myHeap, heapsize);//make array have "heap structure"

	for (; i <= n; i++)
	{
		if (COMPARE(myHeap[0], i) == 2)
		{
			myHeap[0] = i;
			heapify(myHeap, k, 0);
		}
	}

	for (i= k-1; i >=0 ; i--)//pop-off the top K-1 values 
	{
		Best[i] = popFromHeap(myHeap, &heapsize);
	}
	free(myHeap);
	return 1;
}

void heapify(int * heap, int n, int i)
{
	
	int left = 2 * i + 1;
	int right = 2 * i + 2;
	int smallest = i;

	if (left < n  && COMPARE(heap[left], heap[i]) != 1)
		smallest = left;
	if (right < n  && COMPARE(heap[right], heap[smallest]) != 1)
		smallest = right;

	if (i != smallest)
	{
		int temp = heap[i];
		heap[i] = heap[smallest];
		heap[smallest] = temp;
		heapify(heap, n, smallest);
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