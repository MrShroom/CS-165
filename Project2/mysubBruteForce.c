#include <stdlib.h>
#include <string.h>
struct node;
typedef struct node {
	int a;
	int b;
	int c;
	int d;
	int status;
	struct node *next;
} node;

typedef struct list_s {
	node *list;
	node *end;
} list_t;

void logmsg(char *msg);
void lognode(node *d);
void getstatus(node*);
int docompare(int,int,int,int);
int findoneoff(int,int);
int getnextelement(int,int);
int claimzero(int*,int);
int claimnotzero(int*,int);
const int ALL_SAME      = 4;
const int ONE_DIFFERENT = 2;
const int EVEN_DIVIDE   = 0;
int mysub (int n) {
	int sameaszero=0, zero=-1;
	int notsame=0, notzero=-1;
	int anyallsame=0;
	char msg[100];
#ifdef DEBUG
	QCOUNT(-1);
#endif
	list_t *list;
	list = (list_t*)malloc(sizeof(list_t));
	list->list = (node*)malloc(sizeof(node) * n);

	int *current_majorities = (int*)malloc(sizeof(int) * n), value=-1;
	memcpy(current_majorities, &value, sizeof(int)*n);

	// build the lists up.
	int i = 0, container_size=0;
	for (i = 1; i < n; i+=4) {
		node *datum = (node*)malloc(sizeof(node));
		datum->a = i;
		datum->b = i + 1;
		datum->c = i + 2;
		datum->d = i + 3;
		getstatus(datum);
		lognode(datum);
		current_majorities[i/4] = datum->status;
		container_size++;
	}
	if (current_majorities[n - 3] == -1) {
		logmsg("n is not evenly disible by 4");
	}

	// find all the groups of the same element, see if they are all the same elements.
	// O(n^2)
	int j = 0;
	for (i = 0; i < container_size; i++) {
		if (current_majorities[i] == ALL_SAME) {
			anyallsame = 1;
			if (claimzero(&zero, i)) sameaszero += 4;
			for (j = i + 1; j < container_size; j++) {
				if (current_majorities[j] == ALL_SAME) {
					// create a new group of four, see if they are the same.
					int status = docompare(i, i + 1, j, j + 1);
					if (status == EVEN_DIVIDE) {
						char msg[50];
						sprintf(msg, "Group %d and %d are NOT the same!", i, j);
						logmsg(msg);
						sameaszero += 4;
					} else {
						char msg[50];
						sprintf(msg, "Group %d and %d are the same!", i, j);
						logmsg(msg);
						notsame += 4;
					}
				}
			}
		}
	}

	sprintf(msg, "Current counts are %d same as zero and %d not the same", sameaszero, notsame);
	logmsg(msg);

	if (anyallsame) {
		// handle ALL_SAME with ONE_DIFFERENTS
		// find all groups of not the same element, see if we can match to elements of 4.
		// O(N^2)
		for (i = 0; i < container_size; i++) {
			if (current_majorities[i] == ALL_SAME) {
				for (j = i + 1; j < container_size; j++) {
					if (current_majorities[j] == ONE_DIFFERENT) {
						// find one missing
						int diff = findoneoff(i, j);
						if (diff == -1) continue;
						char msg[100];
						sprintf(msg, "Group %d has one different element at %d based on %d", j, diff, i);
						logmsg(msg);
						// Gets to the correct indicies in the array, then adds 1 and returns to 
						// the right index
						int next = getnextelement(j, diff);
						if (docompare(i, i + 1, i + 2, next) == ALL_SAME) {
							sprintf(msg, "Group %d and %d are mostly the same", i, j);
							sameaszero += 3;
							notsame += 1;
						} else {
							sprintf(msg, "Group %d and %d are mostly different", i, j);
							sameaszero += 1;
							notsame += 3;
						}
						logmsg(msg);
					}
				}
			}
		}
	} else {
		// handle all one different elements.
	}

	sprintf(msg, "Current counts are %d same as zero and %d not the same", sameaszero, notsame);
	logmsg(msg);
	for (i = 0; i < container_size; i++) {
		if (current_majorities[i] == EVEN_DIVIDE) {
			sameaszero+=2;
			notsame+=2;
		}
	}

	sprintf(msg, "Finals counts are %d same as zero and %d not the same", sameaszero, notsame);
	logmsg(msg);

	if (sameaszero > notsame) {
		return zero * 4 + 1;
	} else {
		// search forward...then backward...O(N). Technically N - 3
		int group[4];
		group[0] = zero * 4;
		group[1] = zero * 4 + 1;
		group[2] = zero * 4 + 2;
		for (i = zero*4 + 2; i < n; i++) {
			group[3] = i;
			if (QCOUNT(1, group) == ONE_DIFFERENT) return i;
		}

		for (i = 0; i < zero * 4; i++) {
			group[3] = i;
			if (QCOUNT(1, group) == ONE_DIFFERENT) return i;
		}
	}
	return 0; // I want it to error.
}

int getnextelement(int j, int diff) {
	return (j  * 4) + (((diff - j) + 1) % 4) + 1;
}

void getstatus(node *elem) {
	int *indexs = (int*)malloc(sizeof(int) * 4);
	indexs[0] = elem->a; indexs[1] = elem->b; indexs[2] = elem->c;
	indexs[3] = elem->d;
	elem->status = QCOUNT(1, indexs);
}

int docompare(int a, int b, int c, int d) {
	int group[4];
	group[0] = a+1;
	group[1] = b+1;
	group[2] = c+1;
	group[3] = d+1;
	return QCOUNT(1, group);
}

// will return the index of the different element
int findoneoff(int majority, int oneoff) {
	int i, group[4];
	majority *= 4;
	oneoff *= 4;
	group[0] = majority+1;
	group[1] = majority+2;
	group[2] = majority+3;
	for (i= 0; i < 4; i++) {
		group[3] = oneoff + i + 1;
		int result = QCOUNT(1, group);
		if (result == ONE_DIFFERENT) return oneoff + i + 1;
	}

	return -1;
}

int claimnotzero(int *notzero, int position) {
	if (*notzero == -1) {
		*notzero = position;
		char buf[50];
		sprintf(buf, "%d position claims zero", position*4+1);
		logmsg(buf);
		return 1;
	} else {
		return 0;
	}
}

int claimzero(int *zero, int position) {
	if (*zero == -1) {
		*zero = position;
		char buf[50];
		sprintf(buf, "%d position claims nonzero", position*4+1);
		logmsg(buf);
		return 1;
	} else {
		return 0;
	}
}

void logmsg(char *msg) {
#ifdef DEBUG
	printf("%s\n", msg);
#endif
}

void lognode(node *d) {
#ifdef DEBUG
	printf("{%d,%d,%d,%d}=%d\n", d->a, d->b, d->c, d->d, d->status);
#endif
}