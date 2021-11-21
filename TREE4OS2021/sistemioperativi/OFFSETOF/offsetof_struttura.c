/* offsetof_struttura.c */

#include <stddef.h>	/* per la definizione di offsetof, Conforming To C89, C99, POSIX.1-2001. */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>	/* per la definizione dei tipi interi */

int main(void) {

	typedef struct struttura { uint32_t i;   uint8_t  c; uint32_t i2; } STRUTTURA;

	STRUTTURA s;

	printf( "dimensione int %ld \n" ,  sizeof(int) );
	printf( "dimensione uint32_t %ld \n" ,  sizeof(uint32_t) );
	printf( "dimensione tipo STRUTTURA %ld \n" , sizeof(STRUTTURA) );
	printf( "dimensione variabile di tipo STRUTTURA %ld \n" , sizeof( s ) );
	printf("\n");

	printf( "offsetof uint32_t i   %ld \n" ,     offsetof( STRUTTURA, i ) );
	/* COSI' NON VA  printf( "offsetof uint8_t   c  %ld \n" ,     offsetof( S, c ) ); */
	printf( "offsetof uint8_t   c  %ld \n" ,     offsetof( STRUTTURA, c ) );
	printf( "offsetof uint32_t i2  %ld \n" ,     offsetof( STRUTTURA, i2 ) );

	return(0);
}

