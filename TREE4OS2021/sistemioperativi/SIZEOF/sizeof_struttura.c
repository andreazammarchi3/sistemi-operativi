/* sizeof_struttura.c */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>	/* per la definizione dei tipi interi */

int main(void) {

	typedef struct struttura { uint32_t i;   uint8_t  c; uint32_t i2; } STRUTTURA;

	STRUTTURA s;

	printf( "dimensione int %ld \n\n" ,  sizeof(int) );

	printf( "dimensione uint32_t %ld \n" ,  sizeof(uint32_t) );
	printf( "dimensione uint8_t %ld \n\n" ,   sizeof(uint8_t) );
	printf( "dimensione variabile struttura s %ld \n" , sizeof(s) );
	printf( "dimensione tipo STRUTTURA %ld \n" , sizeof(STRUTTURA) );

	return(0);
}

