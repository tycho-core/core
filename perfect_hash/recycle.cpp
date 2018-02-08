//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Tuesday, 8 April 2008 10:20:06 PM
// --------------------------------------------------------------------
// By Bob Jenkins, September 1996.  recycle.c
// You may use this code in any way you wish, and it is free.  No warranty.
// 
// This manages memory for commonly-allocated structures.
// It allocates RESTART to REMAX items at a time.
// Timings have shown that, if malloc is used for every new structure,
//   malloc will consume about 90% of the time in a program.  This
//   module cuts down the number of mallocs by an order of magnitude.
// This also decreases memory fragmentation, and freeing structures
//   only requires freeing the root.
// --------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "standard.h"
#include "recycle.h"



//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////
namespace tycho
{
namespace core
{
namespace detail
{   
namespace perfect_hash
{

reroot *remkroot(size_t size)
{
   reroot *r = (reroot *)remalloc(sizeof(reroot));
   r->list = (recycle *)0;
   r->trash = (recycle *)0;
   r->size = align(size);
   r->logsize = RESTART;
   r->numleft = 0;
   return r;
}

void refree(struct reroot *r)
{
   recycle *temp = r->list;
   if(temp) 
   {
	   while (r->list)
	   {
		  temp = r->list->next;
		  delete[] ((char *)r->list);
		  r->list = temp;
	   }
	}
   delete[] ((char *)r);
   return;
}

/* to be called from the macro renew only */
char  *renewx(struct reroot *r)
{
   recycle *temp;
   if (r->trash)
   {  /* pull a node off the trash heap */
      temp = r->trash;
      r->trash = temp->next;
      (void)tycho::core::mem_zero((void *)temp, r->size);
   }
   else
   {  /* allocate a new block of nodes */
      r->numleft = (word)(r->size * (1ULL << r->logsize));
      if (r->numleft < REMAX) ++r->logsize;
      temp = (recycle *)remalloc(sizeof(recycle) + r->numleft);
      temp->next = r->list;
      r->list = temp;
      r->numleft-=(word)r->size;
      temp = (recycle *)((char *)(r->list+1)+r->numleft);
   }
   return (char *)temp;
}

char* remalloc(size_t  len)
{
  return (char*)core::allocator::malloc(len);
}

} // end namespace
} // end namespace
} // end namespace
} // end namespace

