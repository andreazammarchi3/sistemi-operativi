/* file:  soccorsi_semplice.c */

#ifndef _THREAD_SAFE
	#define _THREAD_SAFE
#endif
#ifndef _POSIX_C_SOURCE
	#define _POSIX_C_SOURCE 200112L
#endif

#include "printerror.h"

#include <unistd.h> 
#include <stdlib.h> 
#include <stdio.h> 
#include <stdint.h>
#include <inttypes.h>
#include <pthread.h> 

#include "DBGpthread.h"

#define NUMPROF 1
#define NUMMEDICI 1
#define NUMESORCISTI 1

/* dati da proteggere */
pthread_mutex_t  mutex;
int		 richiestaAiuto=0;
int NumSoccorritoriArrivati=0;
pthread_cond_t   condRichiestaAiuto;
pthread_cond_t condInizioSoccorso;
pthread_cond_t condFineSoccorso;

#define MEDICO 1
#define ESORCISTA 0

void *Soccorritore (char *Label, int ruolo) 
{ 
	while(1) {
		DBGpthread_mutex_lock(&mutex,Label);
		/* il soccorritore attende la richiesta di aiuto del professore */ 
		while(	richiestaAiuto==0 ) {
			DBGpthread_cond_wait(&condRichiestaAiuto,&mutex,Label);
		}
		printf("soccorritore %s arriva da prof \n", Label );
		fflush(stdout);

		/* occorre aspettare di essere in due soccorritori */
		/* da completare */

		NumSoccorritoriArrivati++;
		if( NumSoccorritoriArrivati == 2 ) {
			/* c'e' gia' l'altro soccorritore,
			   e' fermo su condInizioSoccorso, lo sveglio */
			DBGpthread_cond_signal(&condInizioSoccorso,Label); 
			/* ora il prof ha entrambi i soccorritori */
			richiestaAiuto--;
		}
		else { /* non c'e' l'altro soccorritore, aspetto prima di soccorrere */
			DBGpthread_cond_wait(&condInizioSoccorso,&mutex,Label);
		}


		/* ora ci sono entrambi i soccorritori */
		printf("soccorritore %s inizia soccorso a prof \n", Label );
		fflush(stdout);
		DBGpthread_mutex_unlock(&mutex,Label); 

		/* tempo per soccorrere */
		sleep(2);
	
		DBGpthread_mutex_lock(&mutex,Label); 
		printf("soccorritore %s finisce soccorso a prof \n", Label );
		fflush(stdout);
		/* il soccorritore ha finito
		   ma prima di andare via deve aspettare i
		   che entrambi i soccorritori abbiano finito */


		/* da completare */
		NumSoccorritoriArrivati--; 
		if( NumSoccorritoriArrivati == 0 ) {
			/* entrambi i soccorritori hanno finito, 
			   avviso l'altro soccorritore che puo' andare via
			   e avviso anche il professore	*/
			DBGpthread_cond_broadcast(&condFineSoccorso,Label); 
		}
		else {
			/* l'altro soccorritore non ha finito
			   aspetto che abbia finito sulla stessa cond del professore
			*/
			DBGpthread_cond_wait(&condFineSoccorso,&mutex, Label);
		}
		
		printf("soccorritore %s va via \n", Label );
		fflush(stdout);
		DBGpthread_mutex_unlock(&mutex,Label); 

	}
	pthread_exit(NULL); 
} 


void *Medico (void *arg) 
{ 
	char Plabel[128];
	intptr_t indice;
	int ruolo=MEDICO;

	indice=(intptr_t)arg;
	sprintf(Plabel,"Med%" PRIiPTR "",indice);

	return (  Soccorritore ( Plabel, ruolo)  ); 
}


void *Esorcista (void *arg) 
{ 
	char Plabel[128];
	intptr_t indice;
	int ruolo=ESORCISTA;

	indice=(intptr_t)arg;
	sprintf(Plabel,"Eso%" PRIiPTR "",indice);

	return (  Soccorritore ( Plabel, ruolo)  ); 
}


void *Professore (void *arg) 
{ 
	char Plabel[128];
	intptr_t indice;

	indice=(intptr_t)arg;
	sprintf(Plabel,"Prof%" PRIiPTR "",indice);

	while(1) {
		DBGpthread_mutex_lock(&mutex,Plabel); 
		printf("prof %s cade e chiede aiuto \"Porc... @#! Vac...\n", Plabel );
		fflush(stdout);
		/* prof cade e chiede aiuto */
		richiestaAiuto++;
		DBGpthread_cond_broadcast(&condRichiestaAiuto,Plabel); 

		/* prof aspetta fine del soccorso */
		DBGpthread_cond_wait(&condFineSoccorso,&mutex,Plabel);
		printf("prof %s puo' rialzarsi:   \"GRAZIE\" \n", Plabel );
		fflush(stdout);
		DBGpthread_mutex_unlock(&mutex,Plabel); 

		/* prof ci mette un po' a rialzarsi */
		sleep(1);

		/* prof, se ne va a fare lezione */
		sleep(4);
	}
	pthread_exit(NULL); 
} 

int main ( int argc, char* argv[] ) 
{ 
	pthread_t    th; 
	int  rc;
	intptr_t i;

	richiestaAiuto=0;
	rc = pthread_mutex_init(&mutex, NULL); 
	if( rc ) PrintERROR_andExit(rc,"pthread_mutex_init failed");
	rc = pthread_cond_init(&condRichiestaAiuto, NULL);
	if( rc ) PrintERROR_andExit(rc,"pthread_cond_init failed");

	rc = pthread_cond_init(&condInizioSoccorso, NULL);
	if(rc) PrintERROR_andExit(rc,"pthread_create failed");
	rc = pthread_cond_init(&condFineSoccorso, NULL);
	if(rc) PrintERROR_andExit(rc,"pthread_create failed");

	for(i=0;i<NUMPROF;i++) {
		rc=pthread_create(&th,NULL,Professore,(void*)i); 
		if(rc) PrintERROR_andExit(rc,"pthread_create failed");
	}

	for(i=0;i<NUMMEDICI;i++) {
		rc=pthread_create(&th,NULL,Medico,(void*)i); 
		if(rc) PrintERROR_andExit(rc,"pthread_create failed");
	}

	for(i=0;i<NUMESORCISTI;i++) {
		rc=pthread_create(&th,NULL,Esorcista,(void*)i); 
		if(rc) PrintERROR_andExit(rc,"pthread_create failed");
	}

	pthread_exit(NULL);
	return(0); 
} 
  
