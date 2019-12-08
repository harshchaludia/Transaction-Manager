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

extern  void *begintx(void *);
extern  void *readtx(void *);
extern  void *writetx(void *);
extern  void *aborttx(void *);
extern  void *committx(void *);
extern  zgt_tx* get_tx(long);
extern  int zgt_init_sema(int);
extern void  zgt_init_sema_0(int);
extern void  zgt_init_sema_rest(int);
extern int   zgt_sem_release(int);//added 2014

extern int  zgt_p(int);
extern int  zgt_v(int);
extern int  zgt_nwait(int);
//extern int zgt_nwait1(int);
//extern void  exit(int);

extern zgt_ht *ZGT_Ht;
//extern zgt_tm *ZGT_Sh;
extern zgt_tx *ZGT_Tx;
#define READWRITE 0

extern int errno;


extern key_t ZGT_Key_sem;
extern int ZGT_Semid ;
extern int ZGT_Nsema ;

extern int system(char *);
extern int ZGT_Initp;
extern int Zgt_errno;

struct param
{
  long tid, obno, count;
  char Txtype;
};





