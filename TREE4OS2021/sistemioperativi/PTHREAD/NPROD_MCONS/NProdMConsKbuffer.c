/* file:  NProdNConsKBuffer.c 
   NProduttori e NConsumatori che si scambiano prodotti
   mediante piu' Buffer implementati in una coda finita.
   E' la versione efficente perche' 
   - utilizza due pthread_cond_var (una per Prod e una per Cons)
   - sveglia i pthread solo quando hanno qualcosa da fare.

   Nota Bene: la coda finita sfrutta la struttura dati buffer
   dipendente dalle necessita' del programma applicativo
   e definita nel file ../../COMMON/FINITE_QUEUE/userdefinedBuffer.h
   la struttura dati FTQ definita nel file ../../COMMON/FINITE_QUEUE/finiteTailQueue.h
   e le implementazioni delle funzioni init_FTQ() add_to_tail_FTQ() e remove_from_head_FTQ()
   contenute nel file ../../COMMON/FINITE_QUEUE/finiteTailQueue.c
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
#include <stdint.h>
#include <pthread.h> 
#include "printerror.h"
#include "DBGpthread.h"

/* dovremmo includere un percorso relativo,
   ma preferisco definire un flag -I
   per la directory con gli include
#include "../../COMMON/FINITE_QUEUE/userdefinedBuffer.h"
#include "../../COMMON/FINITE_QUEUE/finiteTailQueue.h"
*/

#include "userdefinedBuffer.h"
#include "finiteTailQueue.h"

/* settati per vedere output di debug
#define DEBUGSTAMPAAZIONI
*/

#define NUMPROD 15
#define NUMCONS 13 
#define NUMBUFFER 5 

/* dati da proteggere */
FTQ global_ftq;   /* coda finita dei buffer */

/* variabili per la sincronizzazione */
pthread_mutex_t  mutex; 
pthread_cond_t   condProd, condCons; 
int numBufferPieni=0; /* 0 o 1 */
int numProdWaiting=0;
int numProdWaitingAndSignalled=0; /* deve essere <= #buffer vuoti */
int numConsWaiting=0;
int numConsWaitingAndSignalled=0; /* deve essere <= #buffer pieni */

void *Produttore (void *arg) 
{ 
	char Plabel[128];
	char Plabelsignal[128];
	int indice;
	uint64_t valProdotto=0;
	STRUCTUREDBUFFER buffer;  /* il singolo buffer */

	indice=*((int*)arg);
	free(arg);
	sprintf(Plabel,"P%d",indice);
	sprintf(Plabelsignal,"P%d->C",indice);

	while(1) {
		/* QUI IL PRODUTTORE PRODUCE IL DATO, un intero.
		   supponiamo che il produttore impieghi circa
		   un secondo a produrre il dato
		DBGsleep(1,Plabel);
		printf("Prod %s PRODUCE dato %lu\n", Plabel, valProdotto+1 ); 
		fflush(stdout);
		*/
		valProdotto++;
		buffer.val=valProdotto;
		
		DBGpthread_mutex_lock(&mutex,Plabel); 

		/* se tutti i buffer sono pieni allora il
		   produttore deve aspettare che un consumatore
		   lo risvegli dopo avere svuotato un posto.

		In generale il problema e' che se ho NUMBUFFER buffer
		ed ho numBufferPieni buffer pieni
		ed ho (NUMBUFFER-numBufferPieni) buffer vuoti
		ed ho numProdWaiting Produttori in coda
		ed ho numProdWaitingAndSignalled Produttori 
		in coda e gia' abilitati a proseguire (cioe' signalled)
		SE i Prod in Coda e signalled SONO >= dei buffer vuoti
		cioe'
		 SE numProdWaitingAndSignalled >= (NUMBUFFER-numBufferPieni)
		allora
		NON POSSO FAR PROSEGUIRE dei Prod SENZA METTERLI NELLA CODA
		*/
		if( numProdWaitingAndSignalled >= (NUMBUFFER-numBufferPieni)){

#ifdef DEBUGSTAMPAAZIONI
			printf("Prod %s ENTRA CODA\n",Plabel);fflush(stdout);
#endif

			numProdWaiting++;
			DBGpthread_cond_wait(&condProd,&mutex,Plabel);
			numProdWaiting--;
			numProdWaitingAndSignalled--;
		}
#ifdef DEBUGSTAMPAAZIONI
		else {
			printf("Prod %s SALTA CODA\n",Plabel);fflush(stdout);
		}
#endif
		/* riempie il buffer col dato prodotto prima
		   in realta' per semplicita' incremento la var 
		*/
		add_to_tail_FTQ( &global_ftq, buffer );

		printf("Prod %s inserisce %lu \n", Plabel, buffer.val ); 
		fflush(stdout);

		numBufferPieni++;

		/*
		SE ho qualche Cons in coda non segnalato
		(cioe' SE numConsWaitingAndSignalled < numConsWaiting )
		E SE i Cons in Coda e signalled SONO < dei buffer pieni
		(cioe' SE numConsWaitingAndSignalled < numBufferPieni) allora
		POSSO ABILITARE altri Cons a proseguire e uscire dalla wait.
		*/
		if( 
			( numConsWaitingAndSignalled < numConsWaiting ) 
			&&
			( numConsWaitingAndSignalled < numBufferPieni ) 
		  ) {
			/* risveglio un Consumatore per svuotare un buffer */
			DBGpthread_cond_signal(&condCons,Plabelsignal); 
			numConsWaitingAndSignalled++;
		}

		/* rilascio mutua esclusione */
		DBGpthread_mutex_unlock(&mutex,Plabel); 
	}
	pthread_exit(NULL); 
} 

void *Consumatore (void *arg) 
{ 
	char Clabel[128];
	char Clabelsignal[128];
	int indice;
	uint64_t valPrelevato=0;
	STRUCTUREDBUFFER buffer;  /* il singolo buffer */

	indice=*((int*)arg);
	free(arg);
	sprintf(Clabel,"C%d",indice);
	sprintf(Clabelsignal,"C%d->P",indice);

	
	while(1) {
		DBGpthread_mutex_lock(&mutex,Clabel); 

		/* se tutti i buffer sono vuoti  allora
		   aspettiamo che un Prod ci risvegli 

		In generale il problema e' che se ho NUMBUFFER buffer
		ed ho numBufferPieni buffer pieni
		ed ho numConsWaiting Cousumatori in coda
		ed ho numConsWaitingAndSignalled Consumatori 
		   in coda e gia' abilitati a proseguire (cioe' signalled)
		SE i Cons in Coda e Signalled SONO >= dei buffer pieni
		cioe'
		 SE numConsWaitingAndSignalled >= numBufferPieni
		allora
		NON POSSO FAR PROSEGUIRE dei Cons SENZA METTERLI NELLA CODA
		*/
		if( numConsWaitingAndSignalled >= numBufferPieni ) {

#ifdef DEBUGSTAMPAAZIONI
			printf("Cons %s ENTRA CODA\n",Clabel);fflush(stdout);
#endif

			numConsWaiting++;
			DBGpthread_cond_wait(&condCons,&mutex,Clabel);
			numConsWaiting--;
			numConsWaitingAndSignalled--;
		}
#ifdef DEBUGSTAMPAAZIONI
		else {
			printf("Cons %s SALTA CODA\n",Clabel);fflush(stdout);
		}
#endif

		/* prendo cio' che c'e' nel buffer */
		remove_from_head_FTQ(&global_ftq, &buffer);
		valPrelevato=buffer.val;

		printf("Cons %s legge %lu \n", Clabel, valPrelevato ); 
		fflush(stdout);

		numBufferPieni--;

		/*
		SE i Prod in Coda e signalled SONO >= dei buffer vuoti
		NON POSSO ABILITARE altri Prod in coda a proseguire

		invece	
		SE ho qualche Prod in coda non segnalato
		(cioe' SE numProdWaitingAndSignalled < numProdWaiting )
		E SE i Prod in Coda e signalled SONO < dei buffer pieni
		cioe' 
		 SE numProdWaitingAndSignalled < (NUMBUFFER-numBufferPieni)
		allora
		POSSO ABILITARE altri Prod a proseguire e uscire dalla wait.
		*/
		if( 
		    (numProdWaitingAndSignalled < numProdWaiting ) 
			&&
		    (numProdWaitingAndSignalled < (NUMBUFFER-numBufferPieni)) 
		  ) 
		{ 
			/* risveglio 1 Prod per riempire il buffer svuotato */
			DBGpthread_cond_signal(&condProd,Clabelsignal); 
			numProdWaitingAndSignalled++;
		}

		/* rilascio mutua esclusione */
		DBGpthread_mutex_unlock(&mutex,Clabel); 

		/* fuori da mutua esclusione, uso il dato 
		   supponiamo che ci voglia 1 secondo
		printf("Cons %s USA dato %lu \n", Clabel, val ); 
		fflush(stdout);
		DBGsleep(1,Clabel);
		*/
	}
	pthread_exit(NULL); 
} 

int main (int argc, char* argv[] ) 
{ 
	pthread_t    thProd[NUMPROD]; 
	pthread_t    thCons[NUMCONS]; 
	int  rc, i, *intptr;
	void *ptr; 

	/* inizializzo la coda globale */
	init_FTQ( NUMBUFFER, &global_ftq );
	
	rc = pthread_cond_init(&condProd, NULL);
	if( rc ) PrintERROR_andExit(rc,"pthread_cond_init failed");
	rc = pthread_cond_init(&condCons, NULL);
	if( rc ) PrintERROR_andExit(rc,"pthread_cond_init failed");
	rc = pthread_mutex_init(&mutex, NULL); 
	if( rc ) PrintERROR_andExit(rc,"pthread_mutex_init failed");

	/* all'inizio i Cons devono aspettare il primo Prod */
	numBufferPieni=0; /* 0 o 1 */
	numProdWaiting=0;
	numProdWaitingAndSignalled=0;
	numConsWaiting=0;
	numConsWaitingAndSignalled=0;

	for(i=0;i<NUMCONS;i++) {
		intptr=malloc(sizeof(int));
		if( !intptr ) { printf("malloc failed\n");exit(1); }
		*intptr=i;
		rc=pthread_create(&(thCons[i]),NULL,Consumatore,(void*)intptr); 
		if(rc) PrintERROR_andExit(rc,"pthread_create failed");
	}

	for(i=0;i<NUMPROD;i++) {
		intptr=malloc(sizeof(int));
		if( !intptr ) { printf("malloc failed\n");exit(1); }
		*intptr=i;
		rc=pthread_create(&(thProd[i]),NULL,Produttore,(void*)intptr); 
		if(rc) PrintERROR_andExit(rc,"pthread_create failed");
	}

	for(i=0;i<NUMPROD;i++) {
		rc = pthread_join(thProd[i], &ptr ); 
		if(rc) PrintERROR_andExit(rc,"pthread_join failed");
	}
	for(i=0;i<NUMCONS;i++) {
		rc = pthread_join(thCons[i], &ptr ); 
		if(rc) PrintERROR_andExit(rc,"pthread_join failed");
	}

	rc = pthread_mutex_destroy(&mutex); 
	if(rc) PrintERROR_andExit(rc,"pthread_mutex_destroy failed");
	rc = pthread_cond_destroy(&condProd); 
	if(rc) PrintERROR_andExit(rc,"pthread_cond_destroy failed");
	rc = pthread_cond_destroy(&condCons); 
	if(rc) PrintERROR_andExit(rc,"pthread_cond_destroy failed");

	return(0); 
} 
  
  
  
