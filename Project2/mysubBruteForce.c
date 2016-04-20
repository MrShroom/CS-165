#include <stdlib.h>
#include <string.h>
struct node;
typedef struct node {
	int a;
	int b;
	int c;
	int d;
	int status;
} node;

typedef struct list_s {
	node *list;
	node *end;
} list_t;

void logmsg(char *msg);
void lognode(node *d);
void getstatus(node*);
node findgroupoffour(int);
int docompare(int,int,int,int);
int findoneoff(int,int);
int findoneoff_node(node,int);
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
	node spot;
	spot.a = -1;
#ifdef DEBUG
	QCOUNT(-1);
#endif
	int *current_majorities = (int*)malloc(sizeof(int) * n), value=-1;
	memcpy(current_majorities, &value, sizeof(int)*n);

	// build the lists up.
	int i = 0, container_size=0;
	for (i = 0; i < n; i+=4) {
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
			if (claimzero(&zero, i)) {
				sameaszero += 4;
				zero = i * 4 + 1;
			}
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
		spot = findgroupoffour(0);
		logmsg("The spot of four I found was");
		lognode(&spot);
		char msg[100];
		zero = spot.a;
		for (i = 0; i < container_size; i++) {
			if (current_majorities[i] == ONE_DIFFERENT) {
				int diff = findoneoff_node(spot, i);
				if (diff == -1) continue;
				sprintf(msg, "Group %d has one different element at %d based on this node", j, diff);
				logmsg(msg);
				lognode(&spot);
				int j = 0, count = 0, next = getnextelement(i, diff);
				while (j < 3) { // we need three compares to verify the the grouping
					if (next != spot.a && next != spot.b && next != spot.c && next != spot.d) {
						int status = docompare(spot.a-1, spot.b-1, spot.c-1, next);
						count += status == ALL_SAME ? 1 : -1;
						next = getnextelement(i, diff + ++j);
					} else j++;
				}
				if (count > 0) {
					sprintf(msg, "Group %d mostly the same", i);
					sameaszero += 3;
					notsame += 1;
				} else {
					sprintf(msg, "Group %d are mostly different", i);
					sameaszero += 1;
					notsame += 3;
				}
				logmsg(msg);
			}
		}
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
		return zero;
	} else {
		// search forward...then backward...O(N). Technically N - 3
		int group[4];
		if (spot.a == -1) {
			group[0] = zero * 4;
			group[1] = zero * 4 + 1;
			group[2] = zero * 4 + 2;
			i = zero*4 + 2;
		} else {
			group[0] = spot.a;
			group[1] = spot.b;
			group[2] = spot.c;
			i = spot.d;
		}
		for (; i < n; i++) {
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
	elem->status = docompare(elem->a, elem->b, elem->c, elem->d);
}

int docompare(int a, int b, int c, int d) {
	int group[4];
	group[0] = a+1;
	group[1] = b+1;
	group[2] = c+1;
	group[3] = d+1;
	int status = QCOUNT(1, group);
	return status;
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

int findoneoff_node(node majority, int oneoff) {
	int i, group[4];
	oneoff *= 4;
	group[0] = majority.a;
	group[1] = majority.b;
	group[2] = majority.c;
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

node findgroupoffour(int start) {
	// this whole process will take 7! compares
	int spots[4], offsets[4], max = 7, status = -1;
	spots[0] = offsets[0] = 0;
	spots[1] = offsets[1] = 1;
	spots[2] = offsets[2] = 2;
	spots[3] = offsets[3] = 3;
	while (status != ALL_SAME) {
		if (spots[0] == max - 3 && spots[3] == max) {
			// we need error.
			break;
		}
		if (spots[3] == max) {
			spots[3] = ++offsets[3];
			spots[2] = ++offsets[2];
		} else if (spots[2] == max - 1) {
			spots[2] = ++offsets[2];
			spots[1] = ++offsets[1];
		} else if (spots[1] == max - 2) {
			spots[1] = ++offsets[1];
			spots[0] = ++offsets[0];
		} else if (spots[0] == max - 3) {
			spots[0] = ++offsets[0];
		} else {
			spots[3]++;
		}

		status = docompare(spots[0], spots[1], spots[2], spots[3]);
	}

	node spot;
	spot.a = spots[0] + 1;
	spot.b = spots[1] + 1;
	spot.c = spots[2] + 1;
	spot.d = spots[3] + 1;
	return spot;
}
// }
// 	// three compares: first, second, first, second
// 	//				   third, fourth, third, fourth
// 	//				   second, third, second, third
// 	int first_group_result = docompare(zero * 4, zero * 4 + 1, i * 4, i * 4 + 1),
// 		second_group_result = docompare(zero * 4 + 2, zero * 4 + 3, i * 4 + 2, i * 4 + 3),
// 		third_group_result = docompare(zero * 4 + 1, zero * 4 + 2, i * 4 + 1, i * 4 + 2);

// 	if (first_group_result == ONE_DIFFERENT) {
// 		if (third_group_result == EVEN_DIVIDE || second_group_result == EVEN_DIVIDE) {
// 			// three go to the majority, 1 to the minority.
// 			return 3;
// 		} else if (third_group_result == ONE_DIFFERENT && second_group_result == ONE_DIFFERENT) {
// 			// three go to the majority, 1 to the minority.
// 			return 3;
// 		} else if (third_group_result == ALL_SAME && )
// 	} else if (first_group_result == ALL_SAME) {

// 	} else {

// 	}

void logmsg(char *msg) {
#ifdef DEBUG
	printf("%s\n", msg);
	fflush(stdout);
#endif
}

void lognode(node *d) {
#ifdef DEBUG
	printf("{%d,%d,%d,%d}=%d\n", d->a, d->b, d->c, d->d, d->status);
	fflush(stdout);
#endif
}