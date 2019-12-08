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
#include <sys/signal.h>
#include <pthread.h>

struct zgt_hlink
{
  char lockmode;
  long sgno;
  long obno;
  long tid;
  pthread_t pid;

  zgt_hlink *next;        //links nodes hashed to the same bucket
  zgt_hlink *nextp;       //links nodes of the same transaction
};

// Local declarations
class zgt_tx {

 public:
  
  friend class zgt_ht;
  friend class wait_for;
   

  long tid;         //need a friend class here
  pthread_t pid;
  long sgno;
  long obno;
  char status;
  char lockmode;
  char Txtype; //transaction type R = Read-only or W = Read/Write
  int semno;
  zgt_hlink *head;           // head of lock table
  zgt_hlink *others_lock(zgt_hlink *, long, long); 
  zgt_tx *nextr;
  
  public :
    
    int free_locks();
  int remove_tx();
  //zgt_tx *get_tx(long);
  void print_wait();
  void print_lock();
  long get_tid(){return tid;}
  long set_tid(long t){tid = t; return tid;}
  char get_status() {return status;}
  int set_lock(long, long, long, int, char);
  int end_tx();
  int cleanup();
  zgt_tx(long,char,char,pthread_t);
  void perform_readWrite(long, long, char);
  int  setTx_semno(long, int);
  void print_tm();
  //  void  wait_for_operation(long );
  //void  finish_operation(long);
  zgt_tx(){};
};


// The Zeitgeist encapsulation object hash table class

class zgt_ht
{

  friend class zgt_tx;
  friend class wait_for;
    
 public:

  /*  methods  */
  
  zgt_hlink *find (long, long); //find the obj in hash table 
  zgt_hlink *findt (long, long, long); //find the tx obj belongs to
  int add ( zgt_tx *, long, long, char); //add an obj for a tx to hash table
  int remove ( zgt_tx *, long, long);  //remove a lock entry
  void print_ht();
  
  /*  constructors & destructors  */
  
  zgt_ht (int ht_size = ZGT_DEFAULT_HASH_TABLE_SIZE);
  ~zgt_ht ();
  
 private:
  
  int  mask;
  int size;	
  int hashing(long sgno, long obno)
    {return((++sgno)*obno)&mask;}

};







