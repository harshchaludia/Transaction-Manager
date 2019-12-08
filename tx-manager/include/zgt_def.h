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

#include <stddef.h>

#define  ZGT_DEFAULT_HASH_TABLE_SIZE  13

#define NTRANSACTION_TYPES 2
#define ODD 1


#define TR_ACTIVE 'P'
#define TR_WAIT   'W'
#define TR_ABORT  'A'
#define TR_END    'E'

