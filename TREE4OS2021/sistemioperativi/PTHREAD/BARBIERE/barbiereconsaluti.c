/* file:  barbiere+saluto.c 
	  il barbiere, dopo avere finito il lavoro su un cliente,
	  lo saluta e lo manda via 
	  e aspetta che se ne sia andato.
	  Il cliente non se ne va fino a che non e' stato salutato
	  e dopo che si e' alzato dalla poltroba avvisa di essersi alzato.
*/ 

#ifndef _THREAD_SAFE
	#define _THREAD_SAFE
#endif
#ifndef _POSIX_C_SOURCE
	#define _POSIX_C_SOURCE 200112L
#endif


#include <unistd.h> 
#include <stdlib.h> 
#include <stdio.h> 
#include <stdint.h>	/* uint64_t */
#include <sys/time.h>	/* gettimeofday()    struct timeval */
#include <pthread.h> 
#include "printerror.h"
#include "DBGpthread.h"

#define NUMSEDIE 5
#define NUMCLIENTI 10

/* nessuna variabili da proteggere */
/* variabili per la sincronizzazione */
int numclientiincoda=0;
pthread_mutex_t  mutex;
pthread_cond_t   condBarbiereLibero; 
pthread_cond_t   condArrivoClienti; 
pthread_cond_t   condSaluto; 	/* aggiunta per salutare */

void *barbierechesaluta (void *arg) 
{ 
	char Blabel[128];
	char Blabelsignal[128];
	int indice;

	indice=*((int*)arg);
	free(arg);
	sprintf(Blabel,"B%d",indice);
	sprintf(Blabelsignal,"B%d->C",indice);

	
	while(1) {

		/* barbiere controlla se c'e' qualcun altro in coda */
		/* prendo la mutua esclusione */
		DBGpthread_mutex_lock(&mutex,Blabel); 

		if ( numclientiincoda <= 0 ) 
			/* il barbiere DORME */
			DBGpthread_cond_wait( &condArrivoClienti, &mutex, Blabel );

		/* c'e' qualche cliente in coda, ne risveglio 1 */
		DBGpthread_cond_signal( &condBarbiereLibero, Blabel );
		/* un cliente esce dalla coda e va nella poltrona del barbiere */

		/* aspetto che il cliente sia pronto a ricevere il saluto del barbiere */
		DBGpthread_cond_wait ( &condSaluto, &mutex, Blabel );
		
		/* rilascio mutua esclusione */
		DBGpthread_mutex_unlock(&mutex,Blabel); 

		/* barbiere serve il cliente */
		printf("barbiere %s serve cliente \n", Blabel ); 
		fflush(stdout);
		/* il barbiere lavora sul cliente per 1/2 di sec piu o meno */
		DBGnanosleep( 500000000, Blabel );

		/* barbiere finisce di servire il cliente */
		printf("barbiere %s finisce il cliente \n", Blabel ); 
		fflush(stdout);

		/* prendo la mutua esclusione */
		DBGpthread_mutex_lock(&mutex,Blabel);

		/* barbiere saluta per far alzare il cliente dalla poltrona e buttarlo fuori */
		DBGpthread_cond_signal( &condSaluto, Blabel );

		printf("barbiere %s ha salutato \n", Blabel ); 
		fflush(stdout);
		
		/* barbiere aspetto che il cliente si sia alzato prima di chiamare altri clienti */
		DBGpthread_cond_wait ( &condSaluto, &mutex, Blabel );
		
		printf("barbiere %s sa che cliente si e' alzato \n", Blabel ); 
		fflush(stdout);
		
		/* rilascio mutua esclusione */
		DBGpthread_mutex_unlock(&mutex,Blabel);
	}
	pthread_exit(NULL); 
} 

void *clientechevuoleesseresalutato (void *arg) 
{ 
	char Clabel[128];
	char Clabelsignal[128];
	int indice;

	indice=*((int*)arg);
	free(arg);
	sprintf(Clabel,"C%d",indice);
	sprintf(Clabelsignal,"C%d->B",indice);

	
	while(1) {

		/* il cliente aspetta qualche giorno = 1/2 sec e poi va dal barbiere */
		DBGnanosleep( 500000000, Clabel );

		/* cliente controlla se c'e' qualcun altro in coda */
		/* prendo la mutua esclusione */
		DBGpthread_mutex_lock(&mutex,Clabel); 

		/* se non ci sono sedie libere me ne vado incazzato  %@!$&^ */
		if ( numclientiincoda >= NUMSEDIE )  {
			DBGpthread_mutex_unlock(&mutex,Clabel); 

			/* il cliente aspetta piu' di qualche giorno = 1 sec 
			   prima di tornare dal barbiere
			*/
			DBGnanosleep( 1000000000-1, Clabel );
		}
		else {

			/* ci sono sedie libere, mi siedo */
			numclientiincoda++;
		
			if ( numclientiincoda <= 1 ) 
				/* ci sono solo io e il barbiere DORME, devo svegliarlo */
				DBGpthread_cond_signal( &condArrivoClienti, Clabel );

			/* aspetto un assenso dal barbiere per andare alla poltrona */
			DBGpthread_cond_wait( &condBarbiereLibero, &mutex, Clabel );

			/* io cliente esco dalla coda e vado nella poltrona del barbiere */
			numclientiincoda--;

			/* NB: ho spostato la unlock piu' in basso */
			DBGpthread_cond_signal ( &condSaluto, Clabel ); /*ok, pronto al saluto*/

			/* il cliente aspetta mentre il barbiere lavora */
			printf("il cliente %s aspetta mentre viene servito\n", Clabel ); 
			fflush(stdout);

			/* aspetto il saluto del barbiere per andare via dalla poltrona */
			DBGpthread_cond_wait( &condSaluto, &mutex, Clabel );

			/* cliente dice al barbiere di essersi alzato dalla poltrona */
			DBGpthread_cond_signal ( &condSaluto, Clabel ); /*ok, vado via */

			printf("il cliente %s si alza dalla poltrona \n", Clabel ); 
			fflush(stdout);

			/* rilascio mutua esclusione */
			DBGpthread_mutex_unlock(&mutex,Clabel); 

			/* e il cliente se ne va */
			printf("il cliente %s se ne va \n", Clabel ); 
			fflush(stdout);
		}
	}
	pthread_exit(NULL); 
} 

int main (int argc, char* argv[] ) 
{ 
	pthread_t    th; 
	int  rc, i, *intptr;

	rc = pthread_cond_init( &condBarbiereLibero , NULL);
	if( rc ) PrintERROR_andExit(rc,"pthread_cond_init failed");
	rc = pthread_cond_init( &condArrivoClienti , NULL);
	if( rc ) PrintERROR_andExit(rc,"pthread_cond_init failed");
	rc = pthread_cond_init( &condSaluto , NULL);
	if( rc ) PrintERROR_andExit(rc,"pthread_cond_init failed");
	rc = pthread_mutex_init( &mutex, NULL); 
	if( rc ) PrintERROR_andExit(rc,"pthread_mutex_init failed");

	numclientiincoda=0;
	
	/* lancio il barbiere */
	intptr=malloc(sizeof(int));
	if( !intptr ) { printf("malloc failed\n");exit(1); }
	*intptr=0; /* un solo barbiere */
	rc=pthread_create( &th,NULL,barbierechesaluta,(void*)intptr); 
	if(rc) PrintERROR_andExit(rc,"pthread_create failed");

	/* lancio i clienti */
	for(i=0;i<NUMCLIENTI;i++) {
		intptr=malloc(sizeof(int));
		if( !intptr ) { printf("malloc failed\n");exit(1); }
		*intptr=i;
		rc=pthread_create( &th,NULL,clientechevuoleesseresalutato,(void*)intptr); 
		if(rc) PrintERROR_andExit(rc,"pthread_create failed");
	}

	pthread_exit(NULL); 
	return(0); 
} 
  
  
  
