#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/queue.h>

#include "userdefinedBuffer.h"
#include "finiteTailQueue.h"

void init_FTQ(int maxnumelem,FTQ *pftq) {
  TAILQ_INIT( & (pftq->head) );
  if( pftq == NULL ) {
    printf("init_FTQ: parameter pftq NULL - Quit\n"); exit(1); 
  }
  if( maxnumelem < 0 ) {
    printf("init_FTQ: parameter maxnumelem < 0 - Quit\n"); exit(1); 
  }
  pftq->maxnumelem=maxnumelem;
  pftq->numelem=0;
}

static void CHECKLEN_FTQ( FTQ *pftq, const char *functionName ) {
  if( pftq->numelem > pftq->maxnumelem ) {
    printf("%s: ftq overflow - Quit\n", functionName ); exit(1); 
  }
  if( pftq->numelem < 0 ) {
    printf("%s: ftq underflow - Quit\n", functionName ); exit(1); 
  }
}

int empty_FTQ(FTQ *pftq) {
  CHECKLEN_FTQ(pftq,"empty_FTQ");
  if(pftq->numelem==0) return(1);
  else                return(0);
}

int full_FTQ(FTQ *pftq) {
  CHECKLEN_FTQ(pftq,"full_FTQ");
  if(pftq->numelem==pftq->maxnumelem) return(1);
  else                              return(0);
}

int numelem_FTQ(FTQ *pftq) {
  CHECKLEN_FTQ(pftq,"numelem_FTQ");
  return(pftq->numelem);
}

void add_to_tail_FTQ(FTQ *pftq, STRUCTUREDBUFFER buffer ) {
  struct entry *elem;
  CHECKLEN_FTQ(pftq,"add_to_tail_FTQ");
  if( pftq->numelem == pftq->maxnumelem ) {
    printf("add_to_tail_FTQ: ftq full - Quit\n"); exit(1); 
  }
  elem = malloc(sizeof(struct entry));
  if (elem) {
    elem->buffer = buffer;
  }
  else { 
    printf("add_to_tail_FTQ: malloc failed\n"); exit(1); 
  } 
  TAILQ_INSERT_TAIL( &(pftq->head), elem, entries);
  pftq->numelem++;
}

void remove_from_head_FTQ(FTQ *pftq, STRUCTUREDBUFFER *pbuffer) {

  CHECKLEN_FTQ(pftq,"remove_from_head_FTQ");
  if(pftq->numelem==0) {
    printf("remove_from_head_FTQ: ftq empty - Quit\n"); exit(1); 
  }
  if( pftq->head.tqh_first == NULL ) {
    printf("remove_from_head_FTQ: ftq empty strano - Quit\n"); exit(1); 
  }
  if( pftq->head.tqh_first != NULL ) {
    struct entry *elem=NULL;

    /* non vuota */
    elem = pftq->head.tqh_first;
    TAILQ_REMOVE(&(pftq->head), pftq->head.tqh_first, entries);
    pftq->numelem--;
    /* consegno elemento rimosso */
    *pbuffer=elem->buffer;
    free(elem);
  }
}


