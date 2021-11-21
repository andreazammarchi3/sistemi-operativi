#ifndef __USERDEFINEDBUFFER_H__
#define __USERDEFINEDBUFFER_H__

#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

/* serve per dichiarare il tipo di struttura
   che deve essere inserito negli elementi della coda
*/

typedef struct structbuffer {
     uint64_t val;
} STRUCTUREDBUFFER;

#endif  /*  __USERDEFINEDBUFFER_H__  */


