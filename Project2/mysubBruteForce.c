#include <stdlib.h>

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
int mysub (int n) {
#ifdef DEBUG
	QCOUNT(-1);
#endif
	list_t *list;
	list = (list_t*)malloc(sizeof(list_t));
	list->list = (node*)malloc(sizeof(node) * n);

	int *current_majorities = (int*)malloc(sizeof(int) * n);

	// build the lists up.
	int i = 0;
	for (i = 1; i < n; i+=4) {
		node *datum = (node*)malloc(sizeof(node));
		datum->a = i;
		datum->b = i + 1;
		datum->c = i + 2;
		datum->d = i + 3;
		getstatus(datum);
		lognode(datum);
		current_majorities[i] = datum->status;
	}
	return 0;
}

void getstatus(node *elem) {
	int *indexs = (int*)malloc(sizeof(int) * 4);
	indexs[0] = elem->a; indexs[1] = elem->b; indexs[2] = elem->c;
	indexs[3] = elem->d;
	elem->status = QCOUNT(1, indexs);
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