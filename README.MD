# Transaction Manager

In this project, we implement a simple transaction manager using C/C++. The transaction manager manages concurrency control
using strict two phase locking protocol with shared locks for read and exclusive locks for write. The project does not implement
upgrading of locks. The project will complete the implementation of the classes zgt_tm and zgt_tx.

## Getting Started

Below instructions will get you a version of the project up and running on your local machine for development and testing purposes. See the following, for notes on how to deploy the project on a live system.

### Prerequisites

1) Visual Studio Code (File editor)
2) Make File (see an example below in the "Running the test" section)
3) Putty (Command line terminal for remote access) or use any functional terminal.

### Operations

1. Void *readtx(void *arg):
In this, we are looking to acquire the ‘S’ lock and finding the object that the current tx needs. No further operations are
performed if the tx is set to abort. Else the tx is sent to the set_lock method. And we look for the object in the hashtable.
After the set_lock method returns a true value, we know that now we have shared lock and can perform read operation.

2. Void *writetx(void *arg):
Does the writing operation quite similar to readtx. Here, we acquire the critical lock and find the object that the current tx
needs. No further operations are performed if the tx is set to abort. Else the tx is sent to the set_lock method. And we look
for the object in the hashtable. After the set_lock method returns a true value, we know that now we have shared lock and can perform read operation.

3. Void *aborttx(void *arg):
Calls the function do_commit_abort, that passes the tid and the status that is ‘A’ for abort. Here we change the status of
the tx to abort and start a method to initialize do_commit_abort which deals with freeing of locks held by txs and orgranize (removing)them.

4. Void *committx(void *arg):
Calls the function do_commit_abort, that passes ‘C’ for commit in status. Very much similar to abort tx method. Here, 
we modify the status of tx and start the do_commit_abort method which deals with freeing of locks and remove the transactions.

5. Void *do_commit_abort():
This function when invoked, frees the locks and removes a transaction from the transaction manager. It is used by both
commit and abort method. Here, it deals with retrieving of transactions that are waiting on semaphores. It also deals with
checking of wait-queue. It releases all the semaphores 1-by-1 using zgt_v(). After this the released objects can be used again by the txs.

6. Int set_lock():
Checks if the object is present in the hash table or not and grants locks to the transaction on a particular object. Also, if
the tx already holds a lock and is requesting for it, then it is already granted to its status. Every read/ write has an object
assigned and this cannot be shared by neither. There could be a chance, if the particular tx is waiting for X mode, it has to
wait infinitely in case more shared lock enquiries coming in.

7. Void perform_readwrite():
It is used to write in the logfile after everything is sorted out from the set_lock method. Updates the objarray[] value by
incrementing 1 for write and decrementing 1 for read.

## Running the tests

1) Open putty,
2) Put Authentication details,
3) Access the "btree_project_F191" folder,
4) go to src>tests> & enter make command to compile the file with makefile.
5) After this run the file using "make bttest" command. 
6) Insert & delete records.
7) Exit the program using 6.

## Makefile Configurations (Replace '//' with #, if you have trouble running it)

//Makefile for TX Manager project.  Needs GNU make.
//Define DEBUGFLAGS for debugging output
//Warning: make depend overwrites this file.

.PHONY: depend clean backup setup

MAIN=zgt_test

// Change the following line depending on where you have copied and unzipped the files
//solutions dir should have src, includes, and test-files directories
//change with your path if you are not using omega.uta.edu

TXMGR=..

//set DIRPATH to the dir from where you use the g++ compiler, change with your path if you are not using omega.uta.edu omega.uta.edu
DIRPATH=/usr

CC=$(DIRPATH)/bin/g++

// EXAMPLE: In the next line only TX_DEBUG is enabled
#DEBUGFLAGS =  -DTX_DEBUG # -DTM_DEBUG -DHT_DEBUG

//Below, all are enabled; you can disable it as you wish
DEBUGFLAGS = -DTX_DEBUG -DTM_DEBUG -DHT_DEBUG

INCLUDES = -I${TXMGR}/include -I.

LINCLUDES = -L$(DIRPATH)/lib

SRCS = zgt_test.C zgt_tm.C zgt_tx.C zgt_ht.C zgt_semaphore.C

OBJS = $(SRCS:.C=.o)

$(MAIN):  $(OBJS) Makefile
	 $(CC) -lpthread $(CFLAGS) $(DEBUGFLAGS) $(INCLUDES) $(OBJS) -o $(MAIN) $(LFLAGS)

.C.o:
	$(CC) $(CFLAGS) $(INCLUDES) $(LINCLUDES) $(DEBUGFLAGS) -c $<

depend: $(SRCS) Makefile
	makedepend $(INCLUDES)  $^

clean:
	rm -f *.o *~ $(MAIN)

//[comment] Grab the sources for a user who has only the makefile
setup:
	/bin/cp -f $(TXMGR)/src/*.[C] .
	/bin/cp -f $(TXMGR)/test-files/*.txt .
	/bin/cp -f $(TXMGR)/includes/*.[h]

//[comment] DO NOT DELETE THIS LINE -- make depend needs it


