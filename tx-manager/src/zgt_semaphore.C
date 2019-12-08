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
#include <sys/signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include "zgt_def.h"
#include "zgt_tm.h"
#include "zgt_extern.h"

extern FILE *logfile;

 union semun {
             int val;
             struct semid_ds *buf;
             ushort *array;
             } ZGT_arg;

struct sembuf  ZGT_sopbuf, *ZGT_sops= &ZGT_sopbuf;

int zgt_init_sema(int create)
{
  int semid;  
if (create != IPC_CREAT) create = 0;
    
  if ((semid= semget(21, 6, create| 0666))< 0) {
    /* error handling */
    printf("could not acquire semaphores: %s\n", semid);
    exit(1);
  }
  return(semid);

}

// 0 (zero) semaphore is used for the Tx manager data structure; hence it is 
// initialized to 1 to permit the first operation to proceed

void zgt_init_sema_0(int semid)
{
      ZGT_arg.val = 1;
      semctl(semid, 0, SETVAL, ZGT_arg);

}

// The rest of the semaphores are used by the transactions to wait on other Txs
// if a lock is NOT obtained. Hence they are initialized to 0. ZGT_Nsema is 
// initialized to MAX_TRANSATIONS+1. A transaction can be waiting only for 
// one other tx. Mutiple Txs can be waiting for a Tx

void zgt_init_sema_rest(int semid) {
  int k;
  for (k = 1; k < ZGT_Nsema ; k++)
    {
      ZGT_arg.val = 0;
      semctl(semid, k, SETVAL, ZGT_arg);
    }  

}

// executes the p operation on the semaphore indicated.

int zgt_p(int sem)
{
  int errno;
  ZGT_sops->sem_num= sem;
  ZGT_sops->sem_op= -1;
  ZGT_sops->sem_flg = 0;
  if (semop(ZGT_Semid, ZGT_sops,1)<0){
    printf("could not do a P semaphore operation on sem:%d\n", sem);
    fflush(stdout);
    fprintf(logfile, "could not do a P semaphore operation on sem:%d\n", sem);
    fflush(logfile);
    exit(1);
  }     

  return(0);
}

// executes the v operation on the semaphore indicated

int zgt_v(int sem)
{
  ZGT_sops->sem_num= sem;
  ZGT_sops->sem_op= 1;
  ZGT_sops->sem_flg = 0;
  if (semop(ZGT_Semid,ZGT_sops,1)< 0){
    printf("could not do a V semaphore operation on sem:%d\n", sem);
    fflush(stdout);
    fprintf(logfile, "could not do a V semaphore operation on sem:%d\n", sem);
    fflush(logfile);
    exit(1);
  }

  return(0);
}

// Returns the # of Txs waiting on a given semaphore; This is needed to release
// all Txs that are waiting on a tx when it commits or aborts.

int zgt_nwait(int sem)
{
  return( semctl(ZGT_Semid,sem,GETNCNT,ZGT_arg));
}

int zgt_sem_release(int semid){
    int k;
    for (k = 0; k < ZGT_Nsema ; k++){
      ZGT_arg.val = 0;
      semctl(semid, k, IPC_RMID, ZGT_arg);
    }  
  return(0);
}


