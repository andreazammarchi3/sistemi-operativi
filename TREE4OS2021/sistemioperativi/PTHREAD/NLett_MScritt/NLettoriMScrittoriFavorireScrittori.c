/* file:  NLettoriMScrittoriFavorireScrittori.c 
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

#define NUMLETTORI 10
#define NUMSCRITTORI 5
/* 1/10 secondo */
#define SCRITTOREATTESAPRIMADISEZIONECRITICA 100000000 
/* 1/100 secondo */
#define SCRITTOREATTESADENTROSEZIONECRITICA   10000000
/* 1/100 secondo */
#define LETTOREATTESADENTROSEZIONECRITICA 10000000

/* Buffer da proteggere */
uint64_t valGlobale=0;
/* var da proteggere ma usate solo per fini statistici */
uint64_t numLettureTotali=0, numScrittureTotali=0;
/* var per controllo automatico */
char ultimoaccesso='0';

/* variabili per la sincronizzazione */
pthread_mutex_t  mutex;    /* protegge l'accesso alle variabili contatore*/ 
pthread_mutex_t  mutexBuffer; /* permette accesso a singoli scrittori o a tutti i lettori */
int numLettoriInLettura=0;  /* numero lettori che stanno leggendo */
pthread_cond_t condScrittori;	/* coda in attesa per scrivere */
pthread_cond_t condLettori;	/* coda in attesa per leggere */
int numScrittoriInAttesa=0;	/* num scrittori in attesa di scrivere */
int numLettoriInAttesa=0;	/* num lettori in attesa di leggere */

void *Scrittore (void *arg) 
{ 
	char Slabel[128];
	char Slabelsignal[128];
	int indice;
	uint64_t valProdotto=0;

	indice=*((int*)arg);
	free(arg);
	sprintf(Slabel,"S%d",indice);
	sprintf(Slabelsignal,"S%d->",indice);

	while(1) {
		/* QUI LO SCRITTORE PRODUCE IL DATO, un intero.
		   supponiamo che lo scrittore impieghi circa
		   1/10 secondo a produrre il dato
		*/
		DBGnanosleep( SCRITTOREATTESAPRIMADISEZIONECRITICA ,Slabel);
		printf("Scrittore %s PRODUCE dato %lu\n", Slabel, valProdotto+1 ); 
		fflush(stdout);
		valProdotto++;

		/* prendo la mutua esclusione sulle variabili */
		DBGpthread_mutex_lock( &mutex, Slabel );
		numScrittoriInAttesa++;
		/* se c'e' qualche lettore che sta ancora leggendo
		   oppure se c'e' qualche ALTRO scrittore oltre a me che vuole scrivere
		   allora devo aspettare e mettermi in coda
		   NOTARE CHE 
			NON CONTROLLO SE ci sono lettori in attesa, passo avanti io!
			CONTROLLO SE c'e' uno scrittore che sta scrivendo
				perche' la variabile numScrittoriInAttesa comprende anche
				lo scrittore che sta' scrivendo adesso, infatti controllo se > 1
				la sua scrittura in corso e' protetta dalla mutexBuffer
		*/

		/* notare che aspetto una volta sola, perche' c'e' mutexBuffer a proteggere il buffer */
		if (   numLettoriInLettura  > 0   ||    numScrittoriInAttesa > 1  )
			DBGpthread_cond_wait( &condScrittori, &mutex, Slabel);
		
		/* prendo la mutua esclusione sul buffer*/
		DBGpthread_mutex_lock( &mutexBuffer, Slabel );

		/* rilascio mutua esclusione sulle variabili */
		DBGpthread_mutex_unlock( &mutex, Slabel);

		/* INIZIO SEZIONE CRITICA con buffer protetto ma altre variabili non protette */
		ultimoaccesso='w'; /* controllo automatizzato */
		/* riempie il buffer col dato prodotto prima */
		valGlobale=valProdotto;

		/* lo scrittore impiega un po' di tempo nella sezione critica, 1/100 sec */
		DBGnanosleep( SCRITTOREATTESADENTROSEZIONECRITICA , Slabel );

		printf("Scrittore %s inserisce %lu \n", Slabel, valGlobale ); 
		fflush(stdout);
		numScrittureTotali++; /* aggiorno statistiche */
		ultimoaccesso='1';
		/* FINE SEZIONE CRITICA a protezione del buffer ma con altre variabili non protette */

		DBGpthread_mutex_lock( &mutex, Slabel);
		numScrittoriInAttesa--;
		DBGpthread_mutex_unlock( &mutexBuffer, Slabel );
		if (numScrittoriInAttesa > 0 )  {
			 DBGpthread_cond_signal ( &condScrittori, Slabel );
		} else  if (  numLettoriInAttesa > 0 )  {
			 DBGpthread_cond_broadcast ( &condLettori, Slabel );
		}
		/* rilascio mutua esclusione */
		DBGpthread_mutex_unlock( &mutex, Slabel );

		/* ERRORE DBGpthread_mutex_unlock( &mutexBuffer, Slabel ); */

	}
	pthread_exit(NULL); 
} 

void *Lettore (void *arg) 
{ 
	uint64_t val;
	uint64_t numLetture=0;  /* conta letture effettuate per decidere quando riposarsi */
	char Llabel[128];
	char Llabelsignal[128];
	int indice;

	indice=*((int*)arg);
	free(arg);
	sprintf(Llabel,"L%d",indice);
	sprintf(Llabelsignal,"L%d->P",indice);

	
	while(1) {
		/* prendo la mutua esclusione sulle variabili */
		DBGpthread_mutex_lock(&mutex,Llabel); 

		/* se ci sono scrittori in attesa, faccio passare avanti loro */	
		numLettoriInAttesa++;
		while ( numScrittoriInAttesa > 0 )  {
			/* mi metto in coda */
			DBGpthread_cond_wait( &condLettori, &mutex, Llabel );
		}
		/* arrivo qui se non ci sono piu' scrittori in attesa */
		  
		/* se non ci sono altri lettori gia' a leggere dal buffer
		   significa che nessun altro lettore ha gia' preso la mutua esclusione
		   per il gruppo dei lettori, quindi
		   devo prendere io la mutua esclusione per tutto il gruppo dei lettori
		*/
		if ( numLettoriInLettura <= 0 ) {
			DBGpthread_mutex_lock(&mutexBuffer,Llabel); 
			ultimoaccesso='R';
		}
		/* ora vado in lettura anche io */
		numLettoriInLettura++;
		numLettoriInAttesa--;

		/* ma permetto ad altri lettori di vedere come stanno le cose */
		/* rilascio la mutua esclusione sulla variabile dei lettori */
		DBGpthread_mutex_unlock ( &mutex, Llabel );

		/* SEZIONE CRITICA: leggo cio' che c'e' nel buffer */
		val=valGlobale;
		numLettureTotali++; /* aggiorno statistiche */

		/* il lettore impiega un po' di tempo nella sezione critica 1/100 di sec */
		DBGnanosleep( LETTOREATTESADENTROSEZIONECRITICA , Llabel );

		printf("Lettore %s legge %lu \n", Llabel, val ); 
		fflush(stdout);

		if( ultimoaccesso != 'R' ) {
			printf( "errore !R in lettura \n");
			fflush(stdout);
			exit(1);
		}
		/* FINE SEZIONE CRITICA */

		/* riprendo la mutua esclusione sulla variabile dei lettori */
		DBGpthread_mutex_lock(&mutex,Llabel); 
		/* io non leggo piu' */
		numLettoriInLettura--;
		/* se sono l'ultimo lettore che era in lettura,
		   devo rilasciare la mutua esclusione per il gruppo dei lettori
		*/
		if ( numLettoriInLettura <= 0 ) {
			ultimoaccesso='0';

			/* rilascio mutua esclusione sul buffer */
			DBGpthread_mutex_unlock(&mutexBuffer,Llabel); 

			if ( numScrittoriInAttesa > 0 )  {
				 DBGpthread_cond_signal ( &condScrittori, Llabel );
			} else  if (  numLettoriInAttesa > 0 )  {
				 DBGpthread_cond_broadcast ( &condLettori, Llabel );
			}

		}
		/* rilascio la mutua esclusione sulle variabili */
		DBGpthread_mutex_unlock(&mutex,Llabel); 


		/* fuori da mutua esclusione, uso il dato 
		   supponiamo che ci voglia 1 secondo
		*/
		printf("Lettore %s USA dato %lu \n", Llabel, val ); 
		fflush(stdout);

		numLetture++;
		/* dopo 500 letture, il lettore aspetta 10 secondi */
		if( numLetture%500 == 0 ) {
			printf("Lettore %s si riposa per 10 secondi \n", Llabel ); 
			fflush(stdout);
			DBGsleep(10,Llabel);
		}
	}
	pthread_exit(NULL); 
} 

int main (int argc, char* argv[] ) 
{ 
	pthread_t    th; 
	int  rc, i, *intptr;
	struct timeval t0, t1;

	rc = pthread_mutex_init(&mutex, NULL); 
	if( rc ) PrintERROR_andExit(rc,"pthread_mutex_init failed");
	rc = pthread_mutex_init(&mutexBuffer, NULL); 
	if( rc ) PrintERROR_andExit(rc,"pthread_mutex_init failed");
	rc = pthread_cond_init(&condScrittori, NULL); 
	if( rc ) PrintERROR_andExit(rc,"pthread_cond_init failed");
	rc = pthread_cond_init(&condLettori, NULL); 
	if( rc ) PrintERROR_andExit(rc,"pthread_cond_init failed");

	gettimeofday( &t0, NULL );

	numLettoriInLettura=0;	/* all'inizio nessun lettore sta leggendo */
	numScrittoriInAttesa=0;	/* all'inizio nessuno scrittore in attesa di scrivere */
	numLettoriInAttesa=0;	/* all'inizio nessun lettore in attesa di leggere */

	for(i=0;i<NUMLETTORI;i++) {
		intptr=malloc(sizeof(int));
		if( !intptr ) { printf("malloc failed\n");exit(1); }
		*intptr=i;
		rc=pthread_create( &th,NULL,Lettore,(void*)intptr); 
		if(rc) PrintERROR_andExit(rc,"pthread_create failed");
	}

	for(i=0;i<NUMSCRITTORI;i++) {
		intptr=malloc(sizeof(int));
		if( !intptr ) { printf("malloc failed\n");exit(1); }
		*intptr=i;
		rc=pthread_create( &th,NULL,Scrittore,(void*)intptr); 
		if(rc) PrintERROR_andExit(rc,"pthread_create failed");
	}

	/* ora il main ogni 10 secondi stampa in output le statistiche di utilizzo
	   ok, e' un modo un po' alla cazzo di cane, perche' dovrei accedere alle variabili
	   contenenti le statistiche in mutua esclusione, ma cosi' cambierei il flusso
	   di esecuzione .....
	*/
	while(1) {
		DBGsleep( 10, "main" );
		gettimeofday( &t1, NULL );
		printf("******************* STATISTICHE D\'USO approssimate  "
			"numLettuteTotali %lu  numScrittuteTot %lu NumLettureAlSecondo %f\n", 
			numLettureTotali, numScrittureTotali,
			((float)numLettureTotali) / (t1.tv_sec-t0.tv_sec) );
		fflush(stdout);
		printf("******** STATISTICHE VALORE ATTUALE VARIABILI "
			"numLettoriInLettura %i  numScrittoriInAttesa %i NumLettoriInAttesa %i\n", 
			numLettoriInLettura, numScrittoriInAttesa, numLettoriInAttesa );
		fflush(stdout);
	}
	pthread_exit(NULL); 

	return(0); 
} 
  
  
  
