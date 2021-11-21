#ifndef __FINITETAILQUEUE_H__
#define __FINITETAILQUEUE_H__

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/queue.h>

#include "userdefinedBuffer.h"

/* serve per dichiarare il tipo struct tailhead 
   e per dichiarare l'esistenza del tipo entry
   e per dire che la coda maneggia le entry 
*/
TAILQ_HEAD(tailhead, entry);
struct entry {
     STRUCTUREDBUFFER buffer;
     TAILQ_ENTRY(entry) entries;   /* Tail queue. */
};
typedef struct FiniteTailQueue {
  struct tailhead head;
  int maxnumelem;
  int numelem;
} FTQ;

extern void init_FTQ(int maxnumelem,FTQ *pftq);
extern int empty_FTQ(FTQ *pftq);
extern int full_FTQ(FTQ *pftq);
extern int numelem_FTQ(FTQ *pftq);
extern void add_to_tail_FTQ(FTQ *pftq, STRUCTUREDBUFFER buffer);
extern void remove_from_head_FTQ(FTQ *pftq, STRUCTUREDBUFFER *pbuffer);

#endif   /*  __FINITETAILQUEUE_H__  */


