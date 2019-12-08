/*------------------------------------------------------------------------------
//                         RESTRICTED RIGHTS LEGEND
//
// Use,  duplication, or  disclosure  by  the  Government is subject 
// to restrictions as set forth in subdivision (c)(1)(ii) of the Rights
// in Technical Data and Computer Software clause at 52.227-7013. 
//
// Copyright 1989, 1990, 1991 Texas Instruments Incorporated.  All rights reserved.
//------------------------------------------------------------------------------
*/



#include <stdio.h>
#include <stdlib.h>
#include "zgt_def.h"
#include "zgt_tm.h"


extern zgt_tm *ZGT_Sh;

// finds the object in the hash table; returns NULL if it is not there

zgt_hlink *zgt_ht::find (long sgno, long obno){
  zgt_hlink *linkp;

  //  Hash on the object number in the obno and search for the obno
    //  in the bucket

  linkp = ZGT_Sh->head[hashing(sgno,obno)];
  while (linkp != NULL){
      if ((linkp->obno == obno)&& (linkp->sgno == sgno)) 
        return (linkp);
      else  linkp = linkp->next;
    }
  //  Return unsuccessfully
    return (NULL); 
}

zgt_hlink *zgt_ht::findt (long tid, long sgno, long obno)
{
  zgt_hlink *linkp; 
  int h;

  //  Hash on the object number in the obno and search for the obno
    //  in the bucket

  linkp =ZGT_Sh->head[hashing(sgno, obno)];

  while (linkp != NULL)  //check this
    {
      if ((linkp->obno == obno)&& (linkp->sgno == sgno)&&
	                   (tid == linkp->tid)) 
        return (linkp);
      else linkp = linkp->next;

    }

  //  Return unsuccessfully

    return (NULL); 
}

// adds and object to the hash table. Need to pass Tx object to make sure
// links are set properly

int zgt_ht::add ( zgt_tx *tp,long sgno, long obno,  char lockmode )
{
  zgt_hlink *tmp, *linkp;
     
  tmp = ZGT_Sh->head[hashing(sgno, obno)]; //save the lock node ptr
  
  linkp = ZGT_Sh->head[hashing(sgno, obno)]=(zgt_hlink*)malloc(sizeof(struct zgt_hlink));
  if (linkp == NULL) return(-1); //memory not there

  linkp->next = tmp;
  linkp->obno = obno;
  linkp->sgno = sgno;
  linkp->lockmode =lockmode ;
  linkp->tid = tp->tid;    
  linkp->pid = tp->pid;

  // Add the ep to the front of the transaction it belongs to
  linkp->nextp=tp->head;
  tp->head = linkp;

  return (0);   //  Return successfully 
}

int zgt_ht::remove ( zgt_tx *tr,long sgno, long obno )
{
  zgt_hlink *prevp, *linkp;
  zgt_hlink *tprev, *tlink;

    prevp = linkp = ZGT_Sh->head[hashing(sgno,obno)];

  while (linkp){
      if ((linkp->tid==tr->tid)&&(linkp->obno==obno)&&(linkp->sgno==sgno)) 
        break;
      prevp = linkp;
      linkp = linkp->next;
    }

  if (linkp == NULL) return (1);  // linkp not found

    // first remove it off the transaction link

  if (tr->head == linkp) tr->head = linkp->nextp;
  else {
	tprev = tlink = tr->head;
	while ((tlink)&&(tlink!=linkp))
	  {
	    if (tlink== linkp) {
	      tprev->nextp = tlink->nextp;
	      break;
	    };
	    tprev = tlink;
	    tlink = tlink->nextp;
	  }
  };

  if (prevp != linkp) prevp->next = linkp->next;
  else ZGT_Sh->head[hashing(sgno,obno)] = linkp->next;
  //  Return successfully
  return (0);
};

// prints the hash table if the HT_DEBUG flag is set. Shows all the elements
// along with the lockmode etc. Useful for debugging.

void zgt_ht::print_ht(){

  zgt_hlink *hlink;
  int i;
#ifdef HT_DEBUG
  printf("printing the Hash table\n");
  printf("Bucket \t Tid \t \t objno \t lockmode \n");
  fflush(stdout);
#endif
  for (i=0;i< this->size;i++){
    hlink=ZGT_Sh->head[i];
    if (hlink !=NULL){
#ifdef HT_DEBUG
      printf("%d: ", i);
      fflush(stdout);
#endif
      while (hlink != NULL) {
#ifdef HT_DEBUG
	printf("%d %d %c ->", hlink->tid, hlink->obno, hlink->lockmode);
	fflush(stdout);
#endif
	hlink = hlink->next;
      }
      printf("\n");
    }
  }
  fflush(stdout);
}

//initializes the  hash table


zgt_ht::zgt_ht (int ht_size) 
{
	int i;
	zgt_hlink *linkp;

	for (i=0;i<ht_size;i++)
		ZGT_Sh->head[i]=NULL; 

  this->mask = ht_size - 1;
  this->size = ZGT_DEFAULT_HASH_TABLE_SIZE;

}
zgt_ht::~zgt_ht () {};














