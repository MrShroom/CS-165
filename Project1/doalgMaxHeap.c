#include <stdlib.h>
void heapify(int *, int, int); // make the ith element the root of a heap
void buildHeap(int *, int);// makes array a heap represntation 
int popFromHeap(int *, int *);// pops the max value from the heap

int doalg(int n, int k, int *Best)
{
	static int seen = 0;//Delete me later
	if (seen == 0)
	{
		printf("Max Heap\n");
		seen = 1;
	}

	if (k > n)//check that we aren't asking for more than we get. 
	{
		return 0;
	}

	int *myHeap = (int *) malloc(sizeof(int) *n);//creat heapspace
	int heapsize = n;//track heap size

	//initialize heap to trival assignment  
	int i = 0;
	for (; i < n; i++)
	{
		myHeap[i] = i + 1;
	}

	buildHeap(myHeap, heapsize);//make array have "heap structure"

	for (i=0; i < k-1; i++)//pop-off the top K-1 values 
	{
		Best[i] = popFromHeap(myHeap, &heapsize);
	}
	Best[k-1] = myHeap[0];//we do this seperate because we don't care if we re-heapify
	free(myHeap);
	return 1;
}

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