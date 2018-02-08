//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Tuesday, 8 April 2008 10:19:03 PM
// --------------------------------------------------------------------
// By Bob Jenkins, September 1996.  recycle.h
// You may use this code in any way you wish, and it is free.  No warranty.
// 
// This manages memory for commonly-allocated structures.
// It allocates RESTART to REMAX items at a time.
// Timings have shown that, if malloc is used for every new structure,
//   malloc will consume about 90% of the time in a program.  This
//   module cuts down the number of mallocs by an order of magnitude.
// This also decreases memory fragmentation, and freeing all structures
//   only requires freeing the root.
// --------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __RECYCLE_H_B485D0BA_315D_4F9D_BE4D_3EEA7BFB5A5E_
#define __RECYCLE_H_B485D0BA_315D_4F9D_BE4D_3EEA7BFB5A5E_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "core/core_abi.h"
#include "standard.h"

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

 
#define RESTART    0
#define REMAX      32000

struct recycle
{
   struct recycle *next;
};
typedef  struct recycle  recycle;

struct reroot
{
   struct recycle *list;     /* list of malloced blocks */
   struct recycle *trash;    /* list of deleted items */
   size_t          size;     /* size of an item */
   size_t          logsize;  /* log_2 of number of items in a block */
   word            numleft;  /* number of bytes left in this block */
};
typedef  struct reroot  reroot;

/* make a new recycling root */
reroot  *remkroot(size_t mysize);

/* free a recycling root and all the items it has made */
void     refree(struct reroot *r);

/* get a new (cleared) item from the root */
#define renew(r) ((r)->numleft ? \
   (((char *)((r)->list+1))+((r)->numleft-= (word)((r)->size))) : renewx(r))

char    *renewx(struct reroot *r);

/* delete an item; let the root recycle it */
/* void     redel(/o_ struct reroot *r, struct recycle *item _o/); */
#define redel(root,item) { \
   ((recycle *)item)->next=(root)->trash; \
   (root)->trash=(recycle *)(item); \
}

/* malloc, but complain to stderr and exit program if no joy */
/* use plain free() to free memory allocated by remalloc() */
char    *remalloc(size_t len);
inline char    *remalloc(size_t len, const char*) 
{
	return remalloc(len);
}

} // end namespace
} // end namespace
} // end namespace
} // end namespace

#endif // __RECYCLE_H_B485D0BA_315D_4F9D_BE4D_3EEA7BFB5A5E_
