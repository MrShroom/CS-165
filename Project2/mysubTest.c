#include <stdlib.h>
#include <string.h>
int mysub (int n) {
	// test stuff.
	int stuff[4];
	stuff[0] = 1;
	stuff[1] = 2;
	stuff[2] = 3;
	stuff[3] = 4;
	printf("1,2,3,4 gives %d\n", QCOUNT(1, stuff));

	stuff[0] = 1;
	stuff[1] = 4;
	stuff[2] = 8;
	stuff[3] = 12;
	printf("1,4,8,12 gives %d\n", QCOUNT(1, stuff));

	stuff[0] = 1;
	stuff[1] = 3;
	stuff[2] = 2;
	stuff[3] = 4;
	printf("1,3,2,4 gives %d\n", QCOUNT(1, stuff));
	return 0;
}