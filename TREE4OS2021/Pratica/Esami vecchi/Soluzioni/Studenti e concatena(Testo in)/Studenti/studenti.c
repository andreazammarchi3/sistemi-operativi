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

#define PROFESSORESISTASPOSTANDO 0
#define PROFESSOREFERMOALBANCO 1

/* dati da proteggere */
int StatoProfessore=PROFESSORESISTASPOSTANDO;
int IndiceBancoVisitato=0;

/* variabili per la sincronizzazione */
pthread_mutex_t  mutex;
pthread_cond_t   condProfessoreIniziaAMuoversi,
		 condProfessoreSiFermaAlBanco;

void *Studente (void *arg) 
{ 
	char Plabel[128];
	intptr_t indice;

	indice=(intptr_t)arg;
	sprintf(Plabel,"Studente%" PRIiPTR "",indice);
	/* l'indice dello studente e' anche l'indice del banco dello studente */

	DBGpthread_mutex_lock(&mutex,Plabel); 
	while ( 1 ) {
		/* lo studente lavora sia mentre il proff si muove i
		   e sia mentre il prof e' fermo al suo (dello studente) banco 
		*/
		printf("studente %s lavora normalmente\n", Plabel);
	        fflush(stdout);
		while ( StatoProfessore == PROFESSORESISTASPOSTANDO
			||
			( StatoProfessore == PROFESSOREFERMOALBANCO
			  &&
			  IndiceBancoVisitato == indice
			)
		       ) 
			DBGpthread_cond_wait(&condProfessoreSiFermaAlBanco,&mutex,Plabel);

		/* lo studente inizia a copiare ... */
		printf("studente %s inizia a copiare dal foglietto\n", Plabel);
	        fflush(stdout);
		DBGpthread_cond_wait(&condProfessoreIniziaAMuoversi,&mutex,Plabel);
		/* lo studente smette di copiare quando il prof ricomincia a muoversi */
		printf("studente %s smette di copiare dal foglietto\n", Plabel);
	        fflush(stdout);
	}
	DBGpthread_mutex_unlock(&mutex,Plabel); 
		
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
	while ( 1 ) {
		int tempindice;
		DBGpthread_mutex_lock(&mutex,Plabel); 
		StatoProfessore=PROFESSORESISTASPOSTANDO;
		DBGpthread_cond_broadcast(&condProfessoreIniziaAMuoversi,Plabel); 
		printf("professore si sposta\n");
	        fflush(stdout);
		DBGpthread_mutex_unlock(&mutex,Plabel); 
		sleep(1);  /* prof si sposta */
		/* raggiunge banco selezionato casualmente */
		DBGpthread_mutex_lock(&mutex,Plabel); 
		while ( (tempindice=random()%NUMSTUDENTI) == IndiceBancoVisitato ) ;
		IndiceBancoVisitato=tempindice;
		StatoProfessore=PROFESSOREFERMOALBANCO;
		printf("professore raggiunge banco %i\n", IndiceBancoVisitato);
	        fflush(stdout);
		DBGpthread_cond_broadcast(&condProfessoreSiFermaAlBanco,Plabel); 
		DBGpthread_mutex_unlock(&mutex,Plabel); 
		sleep(3);  /* prof si ferma al banco */
	}
	pthread_exit(NULL); 
}


int main ( int argc, char* argv[] ) 
{ 
	pthread_t    th; 
	int  rc;
	uintptr_t i=0;
	int seme;

	seme=time(NULL);
        srandom(seme);

	rc = pthread_cond_init(&condProfessoreIniziaAMuoversi, NULL);
	if( rc ) PrintERROR_andExit(rc,"pthread_cond_init failed");
	rc = pthread_cond_init(&condProfessoreSiFermaAlBanco, NULL);
	if( rc ) PrintERROR_andExit(rc,"pthread_cond_init failed");

	rc = pthread_mutex_init(&mutex, NULL); 
	if( rc ) PrintERROR_andExit(rc,"pthread_mutex_init failed");

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
  
  
  
