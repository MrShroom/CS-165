#include <stdlib.h>

typedef struct list_s
{
	int key;
	int value;
}list_t;

typedef struct myCacheContainer_s
{
	int size;
	int capacity;
	list_t *cache;

}myCacheContainer_t;

//heap functions
void heapify(int *, int, int, myCacheContainer_t*, int); // make the ith element the root of a heap
void buildHeap(int *, int, myCacheContainer_t*, int);// makes array a heap represntation 
int popFromHeap(int *, int *, myCacheContainer_t*, int);// pops the max value from the heap

//caching functions
void intilizeCache(myCacheContainer_t*, int);// get memory for cache
int compare_using_cache(myCacheContainer_t*, int , int, int);//does look up to see if we has cached the compare befe calling the COMPARE
int hash(int , int);//simple hashing function
int get_from_cache(myCacheContainer_t *, int);//retrive element from cache
void resize_cache(myCacheContainer_t *);//doubles the capacity of the cache
void set_in_cache(myCacheContainer_t *, int , int );//caches a key value pair
void clear_cache_mem(myCacheContainer_t*);//frees the memory cache was using

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
	
	//create a cache
	myCacheContainer_t *myCache = (myCacheContainer_t *)malloc(sizeof(myCacheContainer_t));
	intilizeCache(myCache, n);

	int *myHeap = (int *)malloc(sizeof(int) * k);//create the heap 
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
		if (compare_using_cache(myCache,n ,myHeap[0], i) == 2)
		{
			myHeap[0] = i;
			heapify(myHeap, k, 0,myCache, n);
		}
	}

	for (i= k-1; i >=0 ; i--)//pop-off the top K values 
	{
		Best[i] = popFromHeap(myHeap, &heapsize, myCache, n);
	}
	free(myHeap);
	clear_cache_mem(myCache);
	return 1;
}

void heapify(int * heap, int n, int i, myCacheContainer_t* cache, int bigN)
{
	int left = 2 * i + 1;//index of left child
	int right = 2 * i + 2;//index of right child
	int smallest = i;//the smallest element between i, left and right

	//find the smallest
	if (left < n  && compare_using_cache(cache, bigN, heap[i], heap[left]) == 1)
		smallest = left;
	if (right < n  && compare_using_cache(cache, bigN, heap[smallest], heap[right]) == 1)
		smallest = right;

	if (i != smallest)//the root needs to be the smallest
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
	cache->cache = (list_t *)malloc(sizeof(list_t) * n);
	int i;
	for (i = 0; i < n; i++)
	{
		cache->cache[i].key = -1;
		cache->cache[i].value = 0;
	}
	cache->capacity = n;
	cache->size = 0;
}

int hash(int num, int size)
{
	//Knuth's multiplicative method:
	//idea taken from http://stackoverflow.com/questions/664014/what-integer-hash-function-are-good-that-accepts-an-integer-hash-key
	return (num * 2654435761u) % size;
}

int get_from_cache(myCacheContainer_t *cache, int key)
{
	int hash_result = hash(key, cache->capacity);
	
	while (cache->cache[hash_result].key != -1)
	{
		if (cache->cache[hash_result].key == key)
			return cache->cache[hash_result].value;
		hash_result = (hash_result + 1) % cache->capacity;
	}
	return 0;
}

void resize_cache(myCacheContainer_t *cache)
{
	list_t  *old_cache = cache->cache;
	int new_cap = (2 * cache->capacity);
	cache->cache = (list_t *)malloc(sizeof(list_t) * new_cap );
	int i;
	for (i = 0; i < new_cap; i++)
	{
		cache->cache[i].key = -1;
		cache->cache[i].value = 0;
	}

	int current_hash;
	int current_key;
	int current_value;
	for (i = 0; i < cache->capacity; i++)
	{
		current_key = old_cache[i].key;
		if (current_key != -1)
		{
			current_hash = hash(current_key, new_cap);
			current_value = old_cache[i].value;
			while (cache->cache[current_hash].key != -1)
			{
				current_hash = (current_hash + 1) % new_cap;
			}
			cache->cache[current_hash].key = current_key;
			cache->cache[current_hash].value = current_value;
		}
		
	}
	cache->capacity = new_cap;
	free(old_cache);
}

void set_in_cache(myCacheContainer_t *cache, int key, int value)
{
	int hash_result = hash(key, cache->capacity);
	
	while (cache->cache[hash_result].key != -1)
	{
		if (cache->cache[hash_result].key != key)
		{
			cache->size--;
			break;
		}
		hash_result = (hash_result + 1) % cache->capacity;
	}

	cache->cache[hash_result].key = key;
	cache->cache[hash_result].value = value;
	cache->size++; 

	if (cache->size > cache->capacity * 0.5)
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
	free(cache->cache);
	free(cache);
}