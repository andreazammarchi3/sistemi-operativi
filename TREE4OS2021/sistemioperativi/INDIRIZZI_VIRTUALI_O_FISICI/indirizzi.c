/*  indirizzi.c */

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int main() { int i; int *p; i=17; p=&i;   printf( " i=%d   p=%p \n", i, (void*)p  ); return(0); }

