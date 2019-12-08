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

#define TRUE	1
#include <stddef.h>
#define FALSE	0



struct node {
	long tid;
	long sgno;
	long obno;
	char lockmode;
	int semno;
	int level;
	node*	next;
	node*	next_s;
	node*	parent;
};



class wait_for {

	node*	head;
	node*	wtable;
	int	found;
	node*	victim;

	int visited(long);
	node* location(long);
	int traverse(node *);
	node* choose_victim(node *, node *);
	public:
	int deadlock();
	wait_for();
	~wait_for(){};
};
