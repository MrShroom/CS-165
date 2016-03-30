#include "COMPARE.c"
#define MAXN 10000

int heapify(int *, int);

int doalg(int n, int k int *Best)
{
	int myHeap[MAXN];
	//initialize Best 
	int i = 0;
	for (; i < n; i++)
	{
		myHeap[i] = i + 1;
	}

}

int heapify(int * heap, int n, int i)
{
	int left = 2 * i;
	int right = 2 * i + 1;
	int largest = i;
	
	if (left < n  && COMPARE(heap[left], heap[i]) == 1)
		largest = left;
}