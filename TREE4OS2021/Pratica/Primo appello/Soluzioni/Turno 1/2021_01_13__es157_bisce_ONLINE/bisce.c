/* file:  bisce.c 
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

#define MAXNUMBISCEGROVIGLIO 3
#define DIMENSIONEVETTOREGROVIGLI 10
#define COMPLETO 1
#define NONCOMPLETO 0

#define INTERVALLOACCOPPIAMENTOsec 4
#define NUMBISCEFIGLIE 3
#define NUMBISCEINIZIALI 5

/* dati da proteggere */
unsigned int IndiceUltimoGroviglio=0;
unsigned int BisceInUltimoGroviglio=0;
int StatoGroviglio[DIMENSIONEVETTOREGROVIGLI];
/* variabili per la sincronizzazione */
pthread_mutex_t  mutex;
pthread_cond_t   cond;
/* variabile per creare thread detached */
pthread_attr_t attr;
/* variabile per assegnare un indice univoco ai pthread */
uintptr_t IndiceBisciaCreata=0;

/* da completare, altre vostre variabili globali, se vi servono */


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

/* eseguire mentre si detiene la mutua esclusione */
void GroviglioNonCompleto(unsigned int IndiceGroviglio) {
	StatoGroviglio[IndiceGroviglio%DIMENSIONEVETTOREGROVIGLI]=0;
}
void GroviglioCompleto(unsigned int IndiceGroviglio) {
	StatoGroviglio[IndiceGroviglio%DIMENSIONEVETTOREGROVIGLI]=1;
}
int SituazioneGroviglio(unsigned int IndiceGroviglio) {
	return( StatoGroviglio[IndiceGroviglio%DIMENSIONEVETTOREGROVIGLI] );
}


void *Biscia (void *arg) 
{ 
	char Plabel[128];
	intptr_t indiceBiscia;
	int myIndiceGroviglio, UltimaBiscia;

	indiceBiscia=(intptr_t)arg;
	sprintf(Plabel,"b%" PRIiPTR "", indiceBiscia);
	printf("biscia %s appena nata\n", Plabel);
	fflush(stdout);

	/* biscia diventa adulta 3-5 sec */
	attendi(3,5);

	/* biscia entra nel groviglio */
	DBGpthread_mutex_lock(&mutex,Plabel); 
	myIndiceGroviglio=IndiceUltimoGroviglio;
	BisceInUltimoGroviglio++;
	if( BisceInUltimoGroviglio==MAXNUMBISCEGROVIGLIO) {
		/* ho completato il groviglio attuale, sono l'ultima biscia */
		UltimaBiscia=1;
		/* il groviglio attuale e' completo */
		GroviglioCompleto(IndiceUltimoGroviglio);
		printf("biscia %s completa groviglio %i \n", Plabel, IndiceUltimoGroviglio);
		fflush(stdout);

		/* sveglio le biscie "del groviglio" */
		/* prima parte DA COMPLETARE A PARTIRE DA QUI SOTTO */
		DBGpthread_cond_broadcast(&cond,Plabel);

		/* FINE prima parte DA COMPLETARE FINO A QUI */
		/* impostiamo nuovo groviglio vuoto */
		IndiceUltimoGroviglio++;
		BisceInUltimoGroviglio=0;
		GroviglioNonCompleto(IndiceUltimoGroviglio);
	}
	else {
		/* non sono l'ultima biscia */
		UltimaBiscia=0;

		printf("biscia %s entra in groviglio %i \n", Plabel, myIndiceGroviglio);
		fflush(stdout);
		/* attendo completamento del mio groviglio */

		/* seconda parte DA COMPLETARE A PARTIRE DA QUI SOTTO */
		do {
			DBGpthread_cond_wait(&cond,&mutex,Plabel);
		} while ( SituazioneGroviglio(myIndiceGroviglio) == NONCOMPLETO );

		/* FINE seconda parte DA COMPLETARE FINO A QUI */
	}


	DBGpthread_mutex_unlock(&mutex,Plabel); 
	/* groviglio completato, accoppiamento alla cieca, 2 sec*/
	printf("biscia %s si accoppia nel groviglio %i \n", Plabel, myIndiceGroviglio);
	fflush(stdout);
	attendi(INTERVALLOACCOPPIAMENTOsec,INTERVALLOACCOPPIAMENTOsec);

	if(  UltimaBiscia ) {
		/* se sono l'ultima biscia genero 10 bisce figlie */
		/* proteggo la variabile condivisa IndiceBisciaCreata */
		pthread_t    th; 
		int  rc;
		intptr_t i;
		printf("biscia %s genera figlie nel groviglio %i \n", Plabel, myIndiceGroviglio);
		fflush(stdout);
		DBGpthread_mutex_lock(&mutex,Plabel); 
		for(i=0; i<NUMBISCEFIGLIE; i++, IndiceBisciaCreata++) {
			rc=pthread_create(&th,  &attr, Biscia,(void*)IndiceBisciaCreata); 
			if(rc) PrintERROR_andExit(rc,"pthread_create failed");
		}
		DBGpthread_mutex_unlock(&mutex,Plabel); 
	}

	/* muoio */
	pthread_exit(NULL); 
}

int main ( int argc, char* argv[] ) 
{ 
	pthread_t    th; 
	int  rc;
	intptr_t i;

	/* AGGIUNGETE LE VOSTRE VARIABILI se vi servono ed inizializzatele */

        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);

	rc = pthread_mutex_init(&mutex, NULL); 
	if( rc ) PrintERROR_andExit(rc,"pthread_mutex_init failed");

	rc = pthread_cond_init(&cond, NULL);
	if( rc ) PrintERROR_andExit(rc,"pthread_cond_init failed");

	for( i=0; i<DIMENSIONEVETTOREGROVIGLI; i++ ) {
		StatoGroviglio[i]=NONCOMPLETO;
	}

	/* creo i thread proteggendo la variabile condivisa IndiceBisciaCreata */
	DBGpthread_mutex_lock(&mutex,"main"); 
	for(IndiceBisciaCreata=0; IndiceBisciaCreata<NUMBISCEINIZIALI; IndiceBisciaCreata++) {
		rc=pthread_create(&th, &attr, Biscia, (void*)IndiceBisciaCreata); 
		if(rc) PrintERROR_andExit(rc,"pthread_create failed");
	}
	DBGpthread_mutex_unlock(&mutex,"main"); 
	
	pthread_exit(NULL);


	return(0); 
} 
  
  
  
