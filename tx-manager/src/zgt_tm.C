/* the Tx mgr functions are implemented here */

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <sys/sem.h>
#include <fstream>
#include "zgt_def.h"
#include "zgt_tm.h"
#include "zgt_extern.h"

#define TEAM_NO 7    //insert your team number here

//Sets the value of member function logfile and opens the file for writing
//in the append mode
void zgt_tm::openlog(string lfile)
{
  FILE *outfile;
#ifdef TM_DEGUG
  printf("entering openlog\n");fflush(stdout);
  fflush(stdout);
#endif
  printf("entering openlog\n");fflush(stdout);
  logfile = (char *) malloc(sizeof(char) * MAX_FILENAME);
    int i=0;
 while(lfile[i] !='\0')
  {
   logfile[i] = lfile[i];
   i++;
  } 
 logfile[i] = '\0';
 if ((outfile = fopen(logfile, "a")) == NULL){
   printf("\nCannot open log file for append:%s\n", logfile);fflush(stdout);
   exit(1);
 }
 fprintf(outfile, "---------------------------------------------------------------\n");
 fprintf(outfile, "TxId\tTxtype\tOperation\tObId:Obvalue:optime\tLockType\tStatus\t\tTxStatus\n");
 fflush(outfile);
#ifdef TM_DEBUG
 printf("leaving openlog\n");fflush(stdout);
 fflush(stdout);
#endif
}

//create a thread and call the constructor of transaction to
//create the object and intialize the other members of zgt_tx in
//begintx(void *thdarg). Pass the thread arguments in a structure

int zgt_tm::BeginTx(long tid, int thrNum, char type)
 {

#ifdef TM_DEBUG
   printf("\nentering BeginTx\n");
   fflush(stdout);
#endif
   struct param *nodeinfo = (struct param*)malloc(sizeof(struct param));
   pthread_t thread1;
   nodeinfo->tid = tid;
   nodeinfo->Txtype = type;
   nodeinfo->obno = -1;
   nodeinfo->count= SEQNUM[tid]=0;
   int status;
   status = pthread_create(&threadid[thrNum], NULL, begintx,(void*)nodeinfo);	// Fork a thr with tid
   if (status){
     printf("ERROR: return code from pthread_create() is:%d\n", status);
     exit(-1);
   }
#ifdef TM_DEBUG
   printf("\nleaving BeginTx\n");
   fflush(stdout);
#endif
   return(0);

 }     

// Call the read function in transaction class. Read operation is just printing
// the value of the item; But to perform the operation, one needs to make sure
// that the strict 2-phase locking is followed.
// now create the thread and call the method readtx(void *)

int zgt_tm::TxRead(long tid, long obno, int thrNum)
 {
	//write your code
	//Entering the Function.
	#ifdef TM_DEBUG
	printf("\n Start OF TxRead\n");
	fflush(stdout);
	#endif
	//Initialization of thread
	pthread_t thread1;
	//Creation of struct *nodeinfo
	struct param *nodeinfo = (struct param*)malloc(sizeof(struct param));
	nodeinfo->tid = tid;
	nodeinfo->obno = obno;
	nodeinfo->Txtype = ' ';
	nodeinfo->count = --SEQNUM[tid];
	int check_status;
	//Calling the READTX method and thread creation
	check_status = pthread_create(&threadid[thrNum], NULL, readtx, (void*) nodeinfo);
	//Thread creation unsuccessful if check_status is 0.
	if (check_status) {
		printf("ERROR => pthread_create() code return is :%d\n", check_status);
		exit(-1);
	}
	#ifdef TM_DEBUG
	printf("\n Ending of TxRead\n");
	fflush(stdout);
	#endif
	//successful operation
	return(0);
 }

// write operation is to increement the value by 1. Again the protocol
// need to be adheared to

int zgt_tm::TxWrite(long tid, long obno, int thrNum)
 {
	//write your code
	//Entering the Function.
	#ifdef TM_DEBUG
	printf("\n Start OF TxWrite\n");
	fflush(stdout);
	#endif
	//Initialization of thread
	pthread_t thread1;
	//Creation of struct *nodeinfo
	struct param *nodeinfo = (struct param*)malloc(sizeof(struct param));
	nodeinfo->tid = tid;
	nodeinfo->obno = obno;
	nodeinfo->Txtype = ' ';
	nodeinfo->count = --SEQNUM[tid];
	int check_status;
	//Calling the READTX method and thread creation
	check_status = pthread_create(&threadid[thrNum], NULL, writetx, (void*) nodeinfo);
	//Thread creation unsuccessful if check_status is 0.
	if (check_status) {
		printf("ERROR => pthread_create() code return is :%d\n", check_status);
		exit(-1);
	}
	#ifdef TM_DEBUG
	printf("\n Ending of TxWrite\n");
	fflush(stdout);
	#endif
	//successful operation
	return(0);
 }

int zgt_tm::CommitTx(long tid, int thrNum)
 {
	//write your code
	//Entering the Function.
	#ifdef TM_DEBUG
	printf("\n Start of CommitTx\n");
	fflush(stdout);
	#endif
	//Initialization of thread
	pthread_t thread1;
	//Creation of struct *nodeinfo
	struct param *nodeinfo = (struct param*) malloc(sizeof(struct param));
	nodeinfo->tid = tid;
	nodeinfo->count = --SEQNUM[tid];
	int check_status;
	//Calling the READTX method and thread creation
	check_status = pthread_create(&threadid[thrNum], NULL, committx, (void*) nodeinfo);
	//Thread creation unsuccessful if check_status is 0.
	if (check_status) {
		printf("ERROR => pthread_create() code return is :%d\n", check_status);
		exit(-1);
	}
	#ifdef TM_DEBUG
	printf("\n End of CommitTx\n");
	fflush(stdout);
	#endif
	//successful operation
	return (0);  
 }
 
int zgt_tm::AbortTx(long tid, int thrNum)
 {
	//write your code
	//Entering the Function.
	#ifdef TM_DEBUG
	printf("\n Start of AbortTx\n");
	fflush(stdout);
	#endif
	//Initialization of thread
	pthread_t thread1;
	//Creation of struct *nodeinfo
    struct param *nodeinfo = (struct param*)malloc(sizeof(struct param));
    nodeinfo->tid = tid;
	//no object for abort
    nodeinfo->obno = -1;
    nodeinfo->Txtype = ' ';
    nodeinfo->count = --SEQNUM[tid];
	int check_status;
	//Calling the READTX method and thread creation
	check_status = pthread_create(&threadid[thrNum], NULL, aborttx, (void*) nodeinfo);
	//Thread creation unsuccessful if check_status is 0.
	if (check_status) {
		printf("ERROR => pthread_create() code return is :%d\n", check_status);
		exit(-1);
	}
	#ifdef TM_DEBUG
    printf("\nleaving AbortTx\n");
    fflush(stdout);
	#endif
	//successful operation
    return(0); 
 }


//currently not used
int zgt_tm::endTm(int thrNum){
    int rc=0;
    int i;
#ifdef TM_DEBUG

   printf("\nentering End of schedule with thrNum: %d\n", thrNum);
   fflush(stdout);
#endif
   printf("Wait for threads and cleanup\n");
  for (i=0; i < thrNum; i++) {
    rc = pthread_join(threadid[i], NULL);
    printf("Thread %d completed with ret value: %d\n", i, rc);
    fflush(stdout);
  }
  printf("ALL threads finished their work\n");
  printf("Releasing mutexes and condpool\n");
  fflush(stdout);
  //release condpool and mutexpool
  for(i=1;i<MAX_TRANSACTIONS+1;++i){
      pthread_mutex_destroy(&mutexpool[i]);
      pthread_cond_destroy(&condpool[i]);
  }
  printf("Releasing all semaphores\n");
  fflush(stdout);
  zgt_sem_release(ZGT_Semid);
  printf("endTm completed\n");
  fflush(stdout);
#ifdef TM_DEBUG
   printf("\nleaving endTm\n");
   fflush(stdout);
#endif
   return(0); //successful operation

 }

// Currently not used
int zgt_tm::ddlockDet()
 {       
#ifdef TM_DEBUG
   printf("\nentering ddlockDet\n");
   fflush(stdout);
#endif

   pthread_t thread1;
   int  status;
   //   status=pthread_create(&thread1, NULL,ddlockdet,(void*)NULL);	// Fork a thread and pass tid
   //if (status){
   //printf("ERROR: return code from pthread_create() is:%d\n", status);
   //exit(-1);
   //}
#ifdef TM_DEBUG
   printf("\nleaving ddlockDet\n");
   fflush(stdout);
#endif
   return(0);  //successful operation
 }

// Currently not used

int zgt_tm::chooseVictim()
 {       
#ifdef TM_DEBUG
   printf("\nentering chooseVictim\n");
   fflush(stdout);
#endif

   pthread_t thread1;
   int  status;
   //   status=pthread_create(&thread1, NULL,choosevictim,(void*)NULL);	// Fork a thread and pass tid
   //if (status){
   //printf("ERROR: return code from pthread_create() is:%d\n", status);
   //exit(-1);
   //}
#ifdef TM_DEBUG
   printf("\nleaving chooseVictim\n");
   fflush(stdout);
#endif
   return(0);  //successful operation
 }


zgt_tm::zgt_tm()
{
  int i,init;

   lastr = NULL;
  //initialize objarray; each element points to a different item
  for(i=0;i<MAX_ITEMS;++i)
    objarray[i] = new item(0);  //all init'd to zero

  //initialize optime for the thread to sleep;
  //get a int from random function to sleep 

  int seed = 10000;   
  srand(seed);  /*initialize random number generator*/ 
  int M = 1000;  /* Multiplier */ 
  for(i=1; i<MAX_TRANSACTIONS+1; ++i) 
    { 
      double r = ( (double)rand() / (double)(RAND_MAX+1.0) ); //RAND_MAX is defined in stdlib
      double x = (r * M); 
      int y = (int) x; 
      optime[i]= abs(y); 
    }


  //initialize condpool and mutexpool
  for(i=1;i<MAX_TRANSACTIONS+1;++i)
    {
      pthread_mutex_init(&mutexpool[i],NULL);
      pthread_cond_init(&condpool[i],NULL);
      condset[i] = 0; 
      SEQNUM[i] = 0;      
    }
  ZGT_Nsema = MAX_TRANSACTIONS+1 ; //setting the no of semaphore 
  
  ZGT_Key_sem = TEAM_NO; //setting the key_t data to const 
  
  //semget() gets a array of semaphore for a particular key.Here
  //	creating a semaphore with  key 1
  
  
  if ((sem= zgt_init_sema(IPC_CREAT))<0){
    cout<< "Error creating semaphores \n";
    exit(1);
  }
  printf("\n sem here %d n",sem);
     fflush(stdout);
  ZGT_Semid = sem;
  
  //intialising the semaphore value with 0 to 1 and the rest to 0
  zgt_init_sema_0(ZGT_Semid);
  zgt_init_sema_rest(ZGT_Semid);  
};
