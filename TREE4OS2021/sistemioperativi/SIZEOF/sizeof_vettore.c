/* sizeof_vettore.c */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>	/* per la definizione dei tipi interi */

int main(void) {

	typedef struct struttura { 
		uint32_t i;
		uint8_t  c;
	} STRUTTURA;

	STRUTTURA s;
	STRUTTURA vet[10];

        printf( "dimensione variabile struttura s %ld \n" , sizeof(s) );
        printf( "dimensione vettore di strutture vet %ld \n" , sizeof(vet) );

	return(0);
}

