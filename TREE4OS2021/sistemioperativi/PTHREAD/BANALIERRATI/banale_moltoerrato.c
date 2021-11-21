/* banale_moltoerrato.c  */

/* simboli gia' definiti nella riga di comando per compilazione dentro il Makefile 
#define _THREAD_SAFE
#define _REENTRANT
#define _POSIX_C_SOURCE 200112L
*/

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#define NUM_THREADS 5

int Glob=10;

void *PrintHello(void *p_index)
{
	Glob+=3;
	sleep(4);
	printf("\n index %d: Hello World! Glob=%d\n", *((int*)p_index), Glob);
	pthread_exit (NULL);
}

int fai_qualcosa_nello_stack( void ) {
	int i=9999999, j=7777777, k=21212121;
	/*
	int i1=9999999, j1=7777777, k1=21212121;
	int i2=9999999, j2=7777777, k2=21212121;
	*/
	if( (i+j+k)%i==3) i++;
	/*
	if( (i1+j1+k1)%i1==3) i1++;
	if( (i2+j2+k2)%i2==3) i2++;
	*/
	return(0);
}

int crea_threads( int num ) {
	int rc, t;
	pthread_t tid;
	for(t=0;t < NUM_THREADS;t++){

		printf("Creating thread %d\n", t);
		rc = pthread_create (&tid, NULL, PrintHello, (void*)&t );  /* NOTARE l'ultimo parametro */
		if (rc){
			printf("ERROR; return code from pthread_create() is %d\n",rc);
			exit(-1);
		}
	}
	return(1);
}

int main()
{
	crea_threads( NUM_THREADS );

	/* fai qualcosa nello stack */
	while(1)
		fai_qualcosa_nello_stack();


	printf("fine main\n"); fflush(stdout);

	pthread_exit (NULL);
	return(0);
}
