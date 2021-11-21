/* file:  birra.c 
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

#define NUMCLIENTI 4
/* stato del distributore */
#define PIATTOVUOTO 0
#define APPOGGIANDOBICCHIERE 1
#define BICCHIEREVUOTO 2
#define BICCHIEREPIENO 3

/* tempistiche */
#define MINSEC_SVUOTAMENTOVESCICA 2
#define MAXSEC_SVUOTAMENTOVESCICA 4
#define MINSEC_BEVUTA 5
#define MAXSEC_BEVUTA 8
#define SEC_APPOGGIOBICCHIERE 2
#define SEC_RIEMPIMENTOBICCHIERE 2

/* dati da proteggere */
unsigned int NumClientiInCodaDeposito=0;
unsigned int NumClientiInCodaPrelievo=0;
int StatoDistributore=PIATTOVUOTO;

/* variabili per la sincronizzazione */
pthread_mutex_t  mutex;
pthread_cond_t	condAttesaPiattoVuoto,
		condAttesaBicchierePieno,
		condDistributoreAttendeBicchiereVuoto;

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

/* IL DISTRIBUTORE VA GIA' BENE COSI', non serve modificarlo */
void *Distributore (void *arg) 
{ 
	char Plabel[128];
	intptr_t indice;

	indice=(intptr_t)arg;
	sprintf(Plabel,"D%" PRIiPTR "", indice);
	/* il distributore parte con il piatto vuoto */

	while(1) {
		/* distributore controlla se c'e' gia' un bicchiere vuoto nel piatto */
		DBGpthread_mutex_lock(&mutex,Plabel); 
		while( StatoDistributore!=BICCHIEREVUOTO ) {
			/* aspetto che arrivi un bicchiere vuoto */
			printf("distributore %s attende bicchiere vuoto \n", Plabel);
			fflush(stdout);
			DBGpthread_cond_wait(&condDistributoreAttendeBicchiereVuoto,&mutex,Plabel);
		}
		/* un cliente ha depositato bicchiere vuoto
		   ora StatoDistributore==BICCHIEREVUOTO
		*/
		printf("distributore %s comincia a riempire bicchiere \n", Plabel);
		fflush(stdout);
		DBGpthread_mutex_unlock(&mutex,Plabel);

		/* il distributore riempie il bicchiere */
		sleep(SEC_RIEMPIMENTOBICCHIERE);

		/* avviso un cliente in coda per prelevare il bicchiere pieno */
		DBGpthread_mutex_lock(&mutex,Plabel); 
		printf("distributore %s ha finito di riempire bicchiere\n", Plabel);
		fflush(stdout);
		StatoDistributore=BICCHIEREPIENO;
		DBGpthread_cond_signal(&condAttesaBicchierePieno,Plabel);
		DBGpthread_mutex_unlock(&mutex,Plabel);
	}
}


void *Cliente (void *arg) 
{ 
	char Plabel[128];
	intptr_t indice;

	indice=(intptr_t)arg;
	sprintf(Plabel,"c%" PRIiPTR "", indice);

	/* il cliente parte con il bicchiere vuoto in mano */

	while(1) {
		/* cliente vuole depositare bicchiere vuoto */
		DBGpthread_mutex_lock(&mutex,Plabel); 
		printf("cliente %s vuole depositare\n", Plabel);
		fflush(stdout);
		NumClientiInCodaDeposito++;
		/* se il piatto non e' vuoto oppure 
		   se ci sono altri clienti in coda deposito oltre a me
		   ALLORA mi devo mettere in coda deposito */
		/* prima parte DA COMPLETARE qui sotto */





		/* sono uscito da coda deposito, ora tocca a me depositare */
		/* ma devo impedire ad altri clienti
		   di appoggiare il bicchiere mentre lo sto depositando io*/





		/* fine prima parte da COMPLETARE  FINO A QUI */
		printf("cliente %s inizia deposito bicchiere\n", Plabel);
		fflush(stdout);
		DBGpthread_mutex_unlock(&mutex,Plabel)
; 
		/* impiego qualche sec per appoggiare il bicchiere */
		sleep(SEC_APPOGGIOBICCHIERE);

		DBGpthread_mutex_lock(&mutex,Plabel); 
		/* ho appoggiato */
		NumClientiInCodaDeposito--;
		/* finito il deposito avviso il distributore affinche' riempia */
		StatoDistributore=BICCHIEREVUOTO;
		DBGpthread_cond_signal(&condDistributoreAttendeBicchiereVuoto,Plabel);
		printf("cliente %s finisce deposito\n", Plabel);
		fflush(stdout);
		DBGpthread_mutex_unlock(&mutex,Plabel); 

		/* vado a svuotare la vescica */
		printf("cliente %s fa pipi'\n", Plabel);
		fflush(stdout);
		attendi(MINSEC_SVUOTAMENTOVESCICA,MAXSEC_SVUOTAMENTOVESCICA);

		/* cliente vuole prelevare bicchiere pieno */
		DBGpthread_mutex_lock(&mutex,Plabel); 
		printf("cliente %s vuole prelevare\n", Plabel);
		fflush(stdout);
		
		/* se non c' e' un bicchiere pieno nel piatto
		   oppure se ci sono altri clienti in coda prelievo
		   ALLORA mi devo mettere in coda prelievo
		*/
		/* seconda parte DA COMPLETARE qui sotto */







		/* fine seconda parte da COMPLETARE  FINO A QUI */
		/* ora tocca a me prelevare bicchiere pieno */
		StatoDistributore=PIATTOVUOTO;
		NumClientiInCodaPrelievo--;

		printf("cliente %s preleva bicchiere\n", Plabel);
		fflush(stdout);
		/* ora il piatto e' vuoto, avviso che si puo' depositarea bicchiere */
		/* terza parte DA COMPLETARE qui sotto */





		/* fine terza parte da COMPLETARE  FINO A QUI */
		DBGpthread_mutex_unlock(&mutex,Plabel); 

		/* bevo dal bicchiere */
		printf("cliente %s beve\n", Plabel);
		fflush(stdout);
		attendi(MINSEC_BEVUTA,MAXSEC_BEVUTA);

	}
}

int main ( int argc, char* argv[] ) 
{ 
	pthread_t    th; 
	int  rc;
	intptr_t i;

	/* AGGIUNGETE LE VOSTRE VARIABILI se vi servono ed inizializzatele */

	rc = pthread_mutex_init(&mutex, NULL); 
	if( rc ) PrintERROR_andExit(rc,"pthread_mutex_init failed");

	rc = pthread_cond_init(&condAttesaPiattoVuoto, NULL);
	if( rc ) PrintERROR_andExit(rc,"pthread_cond_init failed");
	rc = pthread_cond_init(&condAttesaBicchierePieno, NULL);
	if( rc ) PrintERROR_andExit(rc,"pthread_cond_init failed");
	rc = pthread_cond_init(&condDistributoreAttendeBicchiereVuoto,NULL);
	if( rc ) PrintERROR_andExit(rc,"pthread_cond_init failed");

	i=0;
	rc=pthread_create(&th, NULL, Distributore, (void*)i); 
	if(rc) PrintERROR_andExit(rc,"pthread_create failed");

	/* creo i thread */
	for(i=0; i<NUMCLIENTI; i++) {
		rc=pthread_create(&th, NULL, Cliente, (void*)i); 
		if(rc) PrintERROR_andExit(rc,"pthread_create failed");
	}

	pthread_exit(NULL);
	return(0); 
} 
  
  
  
