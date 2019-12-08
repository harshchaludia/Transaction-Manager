#include <stdio.h>
#include <string>
#include <stdlib.h>
#include "zgt_tx.h"
#include <iostream>
#include "zgt_def.h"
#include "zgt_ddlock.h"
#define MAX_ITEMS 10
#define MAX_TRANSACTIONS 5
#define MAX_FILENAME  50
#define NTHREADS        50

using namespace std;


class item {
  public:
	int value;

	item(int k){
	  value=k;
	};

	~item();
 };

//class wait_for;

class zgt_tm{
	
	public:
	
	friend class zgt_tx;
	friend class zgt_ht;
	friend class wait_for;

	long lastid;
	zgt_tx *lastr;
	zgt_hlink *head[ZGT_DEFAULT_HASH_TABLE_SIZE];
	pthread_mutex_t mutexpool[MAX_TRANSACTIONS+1];
	pthread_cond_t condpool[MAX_TRANSACTIONS+1];
	int condset[MAX_TRANSACTIONS+1];
	item *objarray[MAX_ITEMS];
	int  optime[MAX_TRANSACTIONS+1];
  	int sem;
	char *logfile;

	//Fall 2014[jay]. Pointer for wait_for => wait for graph
	wait_for *waitgraph;

    //used to generate seq num for operations of EACH Tx.  
    //Every statement within a transaction is given a different number using SEQNUM[tid]
    // and every statement waits on that value of count to match condset. condset is initialized to 0 and 
    //decremented everytime an operation of a Tx finishes.
    int SEQNUM[MAX_TRANSACTIONS+1];
    pthread_t   threadid[NTHREADS];

	public:
	
		zgt_tm();
        void openlog(string lfile);
        //Fall 2014[jay]. BeginTx modified for TxType; R= Read Only, W=Read/Write
		int BeginTx(long tid, int thrNum,char Txtype);
		int CommitTx(long tid, int thrNum);
		int AbortTx(long tid, int thrNum);
		int TxRead(long tid,long obno, int thrNum);
		int TxWrite(long tid,long obno, int thrNum);
        int endTm(int thrNum);
		int ddlockDet();
		int chooseVictim();
		~zgt_tm();
};
