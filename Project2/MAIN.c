#include <stdio.h>
#define MAXN 10000
#include "QCOUNT.c"
#include "mysub.c"

main(){
	int N[5] = {10,20,200,2000,10000};
	int n,ct,loop,round,answer,total,biggest;

	for (round = 0; round<5; round++) {
		total = 0;
		biggest = -1;
		for (loop=1; loop<=10000; loop++) {
			n = N[round];
			QCOUNT(0,n);
			answer = mysub( n );
			if (answer<0) {
				printf(" *** flagged error %d at round=%d, loop=%d\n",answer,round,loop);
				return(0);
			}
			ct = QCOUNT(2,answer);
			if (ct<=0) {
				printf(" *** ERROR at round=%d, loop=%d\n",round,loop);
				return(0);
			}
			if (ct>biggest) biggest=ct;
			total += ct;
		}
		printf("n=%5d,   max=%6d,   avg=%8.2f\n",
			N[round],biggest,total/10000.);
		fflush(stdout);
	}
    system("pause");
}
