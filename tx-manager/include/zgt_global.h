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

/* The main data structures used in this project: */
// ZGT_Ht -- hash table data structure
// ZGT_Sh  -- main tx manager data structure
// ZGT_Key_sem  -- you need to set it to make it unique for each team
// ZGT_Nsema -- total number of semaphores 

#include<stddef.h>
#define READWRITE 0

int ZGT_Nsema;
int errno;
key_t  ZGT_Key_sem;
int  ZGT_Semid;

zgt_ht * ZGT_Ht;
zgt_tm * ZGT_Sh;
zgt_tx * ZGT_Tx;
int  ZGT_Initp;
int Zgt_errno=0;
