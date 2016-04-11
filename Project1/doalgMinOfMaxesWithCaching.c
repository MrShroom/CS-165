#include <stdlib.h>

typedef struct list_s
{
	int key;
	int value;
	struct list_s * next;
}list_t;

typedef struct myCacheContainer_s
{
	int size;
	int capacity;
	list_t **cache;

}myCacheContainer_t;

void heapify(int *, int, int, myCacheContainer_t*, int); // make the ith element the root of a heap
void buildHeap(int *, int, myCacheContainer_t*, int);// makes array a heap represntation 
int popFromHeap(int *, int *, myCacheContainer_t*, int);// pops the max value from the heap
void intilizeCache(myCacheContainer_t*, int);
int compare_using_cache(myCacheContainer_t*, int , int, int);
void clear_cache_mem(myCacheContainer_t*);

int doalg(int n, int k, int *Best)
{
	static int seen = 0;//Delete me later
	if (seen == 0)
	{
		printf("Min heaps of maxes with Caching\n");
		seen = 1;
	}

	if (k > n)//check that we aren't asking for more than we get. 
	{
		return 0;
	}
	
	myCacheContainer_t *myCache = (myCacheContainer_t *)malloc(sizeof(myCacheContainer_t));
	intilizeCache(myCache, n);

	int *myHeap = (int *)malloc(sizeof(int) * k);
	int heapsize = k;//track heap size

	//initialize heap to first k elements  
	int i = 1;
	for (; i <= k; i++)
	{
		myHeap[i-1] = i ;
	}

	buildHeap(myHeap, heapsize, myCache, n);//make array have "heap structure"

	for (; i <= n; i++)
	{
		if (COMPARE(myHeap[0], i) == 2)
		{
			myHeap[0] = i;
			heapify(myHeap, k, 0,myCache, n);
		}
	}

	for (i= k-1; i >=0 ; i--)//pop-off the top K-1 values 
	{
		Best[i] = popFromHeap(myHeap, &heapsize, myCache, n);
	}
	free(myHeap);
	clear_cache_mem(myCache);
	return 1;
}

void heapify(int * heap, int n, int i, myCacheContainer_t* cache, int bigN)
{
	
	int left = 2 * i + 1;
	int right = 2 * i + 2;
	int smallest = i;

	if (left < n  && compare_using_cache(cache, bigN, heap[left], heap[i]) != 1)
		smallest = left;
	if (right < n  && compare_using_cache(cache, bigN, heap[right], heap[smallest]) != 1)
		smallest = right;

	if (i != smallest)
	{
		int temp = heap[i];
		heap[i] = heap[smallest];
		heap[smallest] = temp;
		heapify(heap, n, smallest, cache, bigN);
	}
}

void buildHeap(int *heap, int size, myCacheContainer_t* cache, int bigN)
{
	int i = size / 2;
	while (i-->0)
		heapify(heap, size, i, cache, bigN);
}

int popFromHeap(int *heap, int *heapsize, myCacheContainer_t* cache, int bigN)
{
	int out = heap[0];
	heap[0] = heap[--(*heapsize)];
	heapify(heap, *heapsize, 0,cache, bigN);
	return out;
}

void intilizeCache(myCacheContainer_t *cache, int n)
{
	cache->cache = (list_t **)malloc(sizeof(list_t*) * n);
	int i;
	for (i = 0; i < n; i++)
	{
		cache->cache[i] = NULL;
	}
	cache->capacity = n;
	cache->size = 0;
}

int hash(int num, int size)
{
	return (num) % size;
}

int get_from_cache(myCacheContainer_t *cache, int key)
{
	int hash_result = hash(key, cache->capacity);
	list_t * current_bin = cache->cache[hash_result];
	while (current_bin)
	{
		if (current_bin->key == key)
			return current_bin->value;
		current_bin = current_bin->next;
	}
	return 0;
}

void resize_cache(myCacheContainer_t *cache)
{
	list_t  **old_cache = cache->cache;
	int new_cap = (2 * cache->capacity);
	cache->cache = (list_t **)malloc(sizeof(list_t*) * new_cap );
	int i;
	for (i = 0; i < new_cap; i++)
	{
		cache->cache[i] = NULL;
	}

	list_t  *next_bin;
	list_t  *current_bin;
	list_t *new_bin;
	int current_hash;
	for (i = 0; i < cache->capacity; i++)
	{
		next_bin = NULL;
		current_bin = old_cache[i];
		while (current_bin)
		{
			next_bin = current_bin->next;
			current_hash = hash(current_bin->value, new_cap);
			if (!cache->cache[current_hash])
			{
				cache->cache[current_hash] = current_bin;
				current_bin->next = NULL;
			}
			else
			{ 
				new_bin = cache->cache[current_hash];
				while (new_bin->next != NULL)
					new_bin = new_bin->next;
				new_bin->next = current_bin;
				current_bin->next = NULL;
			}
			current_bin = next_bin;
		}
		old_cache[i] = NULL;
	}
	cache->capacity = new_cap;
	free(old_cache);

}

void set_in_cache(myCacheContainer_t *cache, int key, int value)
{
	int hash_result = hash(key, cache->capacity);
	list_t * current_bin = cache->cache[hash_result];
	while (current_bin)
	{
		if (current_bin->key == key)
			break;
		current_bin = current_bin->next;
	}

	if (current_bin == NULL)
	{
		current_bin = (list_t *)malloc(sizeof(list_t));
		current_bin->next = cache->cache[hash_result];
		cache->cache[hash_result] = current_bin;
		cache->size++;
	}
	current_bin->key = key;
	current_bin->value = value;

	if (cache->size > cache->capacity * 0.7)
	{
		resize_cache(cache);
	}
}



int compare_using_cache(myCacheContainer_t *cache,int n, int arg1, int arg2)
{
	int cache_pos = (arg1-1) * n + (arg2-1);
	int returned_from_cache = get_from_cache(cache, cache_pos);

	if (returned_from_cache != 0)
		return returned_from_cache;

	int invered_par_pos = (arg2 - 1) * n + (arg1 - 1);
	returned_from_cache = get_from_cache(cache, invered_par_pos);

	if (returned_from_cache == 1)
		return 2;
	if (returned_from_cache == 2)
		return 1;

	int realCompareCall = COMPARE(arg1, arg2);

	set_in_cache(cache, cache_pos, realCompareCall);
	return realCompareCall;
}

void clear_cache_mem(myCacheContainer_t * cache)
{
	int i;
	list_t * curent_bin;
	list_t * next_bin;
	for ( i = 0; i < cache->capacity; i++)
	{
		curent_bin = cache->cache[i];
		while (curent_bin)
		{
			next_bin = curent_bin->next;
			free(curent_bin);
			curent_bin = next_bin;
		}
	}
	free(cache->cache);
	free(cache);
}