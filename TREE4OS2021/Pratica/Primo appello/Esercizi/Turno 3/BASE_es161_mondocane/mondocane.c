/* file:  mondocane.c 
*/

#ifndef _THREAD_SAFE
	#define _THREAD_SAFE
#endif

#include "printerror.h"

#include <unistd.h> 
#include <stdlib.h> 
#include <stdio.h> 
#include <stdint.h>
#include <inttypes.h>
#include <pthread.h> 

#include "DBGpthread.h"

#define NUMCANI 4
/* tempistiche */
#define SECPIPI   6
#define SECGIRO   2
#define SECANNUSA 3

/* dati da proteggere */
int NumCaniDietro[NUMCANI];

/* variabili per la sincronizzazione */
pthread_mutex_t  mutex;
pthread_cond_t	cond;

/* NON potete aggiungere altre vostre variabili globali */


void attendi( int secmin, int secmax) {
        int secrandom=0;
	static int inizializzato=0;
	if( inizializzato==0 ) {
		srandom( time(NULL) );
		inizializzato=1;
	}
        if( secmin > secmax ) return;
        else if ( secmin == secmax )
                secrandom = secmin;
        else
                secrandom = secmin + ( random()%(secmax-secmin+1) );
        do {
                /* printf("attendi %i\n", secrandom);fflush(stdout); */
                secrandom=sleep(secrandom);
                if( secrandom>0 )
                        { printf("sleep interrupted - continue\n"); fflush(stdout); }
        } while( secrandom>0 );
        return;
}

void *Cane (void *arg) 
{ 
	char Plabel[128];
	intptr_t indice;
	int K;

	indice=(intptr_t)arg;
	sprintf(Plabel,"c%" PRIiPTR "", indice);

	while(1) {
		do {
			DBGpthread_mutex_lock(&mutex,Plabel); 
			K=random()%(NUMCANI+2);
			DBGpthread_mutex_unlock(&mutex,Plabel); 
		} while ( K==indice );
		/* estratto numero casuale K != indice cane */
		printf("cane %s estrae numero K=%i \n", Plabel, K);
		fflush(stdout);

		DBGpthread_mutex_lock(&mutex,Plabel); 
		if( K>=NUMCANI ) { /* vorrei fare pipi */
			printf("cane %s vorrebbe fare pipi \n", Plabel);
			fflush(stdout);
			/* attendo fino a che nessun cane mi annusa le chiappe
			   e poi posso andare a fare pipi  */
			/* prima parte DA COMPLETARE A PARTIRE DA QUI SOTTO */
			while(NumCaniDietro[indice]) {
				/* al posto del while avrei potuto usare un if e al posto della broadcast una signal solo se avevo un vettore di cond perche' non c'e' modo
				   di far arrivare la signal al cane giusto che ha finito i cani che gli annusavano il culo, quindi ho dovuto fare il while*/
				DBGpthread_cond_wait(&cond, &mutex, Plabel);
			}

			/* setto la variabile NumCaniDietro per indicare che
			   sono andato a fare pipi */


			NumCaniDietro[indice]=-1;


			/* FINE prima parte DA COMPLETARE */
			printf("cane %s fa pipi \n", Plabel);
			fflush(stdout);
			DBGpthread_mutex_unlock(&mutex,Plabel);

			sleep(SECPIPI);

			DBGpthread_mutex_lock(&mutex,Plabel); 
			printf("cane %s finisce di fare pipi \n", Plabel);
			fflush(stdout);
			NumCaniDietro[indice]=0; /* ho finito di fare pipi */
			DBGpthread_mutex_unlock(&mutex,Plabel);
			/* vado ad estrare nuovo numero casuale */
		}
		else if( K<NUMCANI ) { /* vorrebbe annusare cane K */
			printf("cane %s vorrebbe annusare %i\n", Plabel, K);
			fflush(stdout);
			/* verifico se il cane di indice K sta facendo pipi */
			if ( NumCaniDietro[K] == -1 ) { /* cane K fa pipi */
				/* cane fa un giro per 2 secondi */
				printf("cane %s fa giro perche' cane K %i fa pipi \n", Plabel, K);
				fflush(stdout);
				DBGpthread_mutex_unlock(&mutex,Plabel);

				sleep(SECPIPI);

				printf("cane %s finisce giro\n", Plabel);
				fflush(stdout);
				/* vado ad estrare nuovo numero casuale */
				continue;
			} else { /* cane K NON STA FACENDO pipi, annuso */
				printf("cane %s annusa cane %i\n", Plabel, K);
				fflush(stdout);

				/* mi aggiungo a chi annusa cane K */
				/* seconda parte DA COMPLETARE A PARTIRE DA QUI SOTTO */

				NumCaniDietro[K]++;

				DBGpthread_mutex_unlock(&mutex, Plabel);

				/* FINE seconda parte DA COMPLETARE */
				/* annuso */
				sleep(SECANNUSA);

				DBGpthread_mutex_lock(&mutex,Plabel);
				printf("cane %s finisce di annusare %i\n", Plabel, K);
				fflush(stdout);
				/* mi tolgo da chi annusa cane K */
				NumCaniDietro[K]--;
				/* avviso che ho smesso di annusare */
				/* terza parte DA COMPLETARE A PARTIRE DA QUI SOTTO */

				if(NumCaniDietro[K] == 0){
					/* qua se avessi potuta avere un array di cond potevo usare una signal per svegliare direttamente il cane giusto
					   invece visto che ho una condition variable sola e potenzialmente piu' cani che la usano devo fare in questo modo */
					DBGpthread_cond_broadcast(&cond, Plabel);
				}

				/* il cane ha avvisato che ha finito con la signal e ora va ad annusare le margerite per riprendersi */
				/* FINE terza parte DA COMPLETARE */
				DBGpthread_mutex_unlock(&mutex,Plabel);
				/* vado ad estrare nuovo numero casuale */
				printf("Ho finito di annusare il culo ora vada per le margerite [%s]\n", Plabel);
				sleep(SECGIRO);
			}
		} /* fine caso K<NUMCANI */
	} /* fine while piu' esterno */
}


int main ( int argc, char* argv[] ) 
{ 
	pthread_t    th; 
	int  rc;
	intptr_t i;

	rc = pthread_mutex_init(&mutex, NULL); 
	if( rc ) PrintERROR_andExit(rc,"pthread_mutex_init failed");
	rc = pthread_cond_init(&cond, NULL);
	if( rc ) PrintERROR_andExit(rc,"pthread_cond_init failed");

	/* all'inizio ogni cane non annusa e non fa pipi */
	for(i=0; i<NUMCANI; i++) NumCaniDietro[i]=0;

	/* creo i thread */
	for(i=0; i<NUMCANI; i++) {
		rc=pthread_create(&th, NULL, Cane, (void*)i); 
		if(rc) PrintERROR_andExit(rc,"pthread_create failed");
	}

	pthread_exit(NULL);
	return(0); 
} 
  
  
  
