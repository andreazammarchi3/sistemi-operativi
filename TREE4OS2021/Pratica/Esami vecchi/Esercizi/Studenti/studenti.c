/* file:  studenti.c */

#ifndef _THREAD_SAFE
	#define _THREAD_SAFE
#endif
#ifndef _POSIX_C_SOURCE
	#define _POSIX_C_SOURCE 200112L
#endif

/* la   #define _POSIX_C_SOURCE 200112L   e' dentro printerror.h */
#ifndef _BSD_SOURCE
#define _BSD_SOURCE     /* per random e srandom */
#endif


/* messo prima perche' contiene define _POSIX_C_SOURCE */
#include "printerror.h"

#include <unistd.h>   /* exit() etc */
#include <stdlib.h>     /* random  srandom */
#include <stdio.h>
#include <string.h>     /* per strerror_r  and  memset */
#include <sys/types.h>
#include <signal.h>
#include <sys/time.h> /*gettimeofday() struct timeval timeval{} for select()*/
#include <time.h> /* timespec{} for pselect() */
#include <limits.h> /* for OPEN_MAX */
#include <errno.h>
#include <assert.h>
#include <stdint.h>     /* uint64_t intptr_t */
#include <inttypes.h>   /* per PRIiPTR */
#include <fcntl.h>
#include <sys/stat.h>
#include <pthread.h>

#include "DBGpthread.h"

#define NUMSTUDENTI 3


void *Studente (void *arg) 
{ 
	char Plabel[128];
	intptr_t indice;

	indice=(intptr_t)arg;
	sprintf(Plabel,"Studente%" PRIiPTR "",indice);
	/* l'indice dello studente e' anche l'indice del banco dello studente */

	/* implementare lo studente  ...... */
	



	
	pthread_exit(NULL); 
}


void *Professore (void *arg) 
{ 
	char Plabel[128];
	intptr_t indice;

	indice=(intptr_t)arg;
	sprintf(Plabel,"Professore%" PRIiPTR "",indice);

	printf("professore controlla dalla cattedra\n");
        fflush(stdout);
	sleep(3);

	/* implementare professore gira tra i banchi  ...... */

	while ( 1 ) {
		/* ripete indefinitamente ......... */



	}


	pthread_exit(NULL); 
}


int main ( int argc, char* argv[] ) 
{ 
	pthread_t    th; 
	int  rc;
	uintptr_t i=0;
	int seme;

	/* inizializzazione generatore numeri casuali */
	seme=time(NULL);
        srandom(seme);

	/* inizializza strutture dati per mutua esclusione e sincronizzazione */

	for(i=0;i<NUMSTUDENTI;i++) {
		rc=pthread_create(&th,NULL,Studente,(void*)i); 
		if(rc) PrintERROR_andExit(rc,"pthread_create failed");
	}
	i=0;
	rc=pthread_create(&th,NULL,Professore,(void*)i); 
	if(rc) PrintERROR_andExit(rc,"pthread_create failed");
	
	pthread_exit(NULL);
	return(0); 
} 
  
  
  
