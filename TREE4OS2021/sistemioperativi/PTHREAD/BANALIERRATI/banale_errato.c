/* banale_errato.c  */

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

int main()
{
	pthread_t tid;
	int rc, t;  /* NOTARE che ora la variabile t sullo stack diventa un problema */
	for(t=0;t < NUM_THREADS;t++){

		printf("Creating thread %d\n", t);
		rc = pthread_create (&tid, NULL, PrintHello, (void*)&t ); /* NOTARE l'ultimo parametro */
		if (rc){
			printf("ERROR; return code from pthread_create() is %d\n",rc);
			exit(-1);
		}

	}

	pthread_exit (NULL);  /* tolto il commento */
	return(0);
}

