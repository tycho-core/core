//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Tuesday, 8 April 2008 9:49:21 PM
// ------------------------------------------------------------------------------
// perfect.h: code to generate code for a hash for perfect hashing.
// (c) Bob Jenkins, September 1996
// You may use this code in any way you wish, and it is free.  No warranty.
// I hereby place this in the public domain.
// Source is http://burtleburtle.net/bob/c/perfect.h
// ------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __PERFECT_H_78B797CE_FFAA_424F_A0C4_A57024857C95_
#define __PERFECT_H_78B797CE_FFAA_424F_A0C4_A57024857C95_

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
    
#define MAXKEYLEN 30                              /* maximum length of a key */
#define USE_SCRAMBLE  4096           /* use scramble if blen >= USE_SCRAMBLE */
#define SCRAMBLE_LEN ((ub4)1<<16)                    /* length of *scramble* */
#define RETRY_INITKEY 2048  /* number of times to try to find distinct (a,b) */
#define RETRY_PERFECT 1     /* number of times to try to make a perfect hash */
#define RETRY_HEX     200               /* RETRY_PERFECT when hex keys given */

/* the generated code for the final hash, assumes initial hash is done */
struct gencode
{
  char **line;                       /* array of text lines, 80 bytes apiece */
  /*
   * The code placed here must declare "ub4 rsl" 
   * and assign it the value of the perfect hash using the function inputs.
   * Later code will be tacked on which returns rsl or manipulates it according
   * to the user directives.
   *
   * This code is at the top of the routine; it may and must declare any
   * local variables it needs.
   *
   * Each way of filling in **line should be given a comment that is a unique
   * tag.  A testcase named with that tag should also be found which tests
   * the generated code.
   */
  ub4    len;                    /* number of lines available for final hash */
  ub4    used;                         /* number of lines used by final hash */

  ub4    lowbit;                          /* for HEX, lowest interesting bit */
  ub4    highbit;                        /* for HEX, highest interesting bit */
  ub4    diffbits;                         /* bits which differ for some key */
  ub4    i,j,k,l,m,n,o;                      /* state machine used in hexn() */

  // mslater: added members
  enum 
  {
	CT_INVALID,
	CT_NORM_GT_UB4BITS,
	CT_NORM_LE_UB4BITS,
	CT_NORM_LE_UB4BITS_SCRAMBLED,
	CT_HASH_AB,
	CT_HASH_AB_SINGLE,
	CT_HASH_AB_SCRAMBLED,
	CT_ZERO
  } p_type;
	
  ub4 p_shift;
  ub4 p_mask;
  ub4 p_initlev;
};
typedef  struct gencode  gencode;

/* user directives: perfect hash? minimal perfect hash? input is an int? */
struct hashform
{
  enum {
    NORMAL_HM,                                            /* key is a string */
    INLINE_HM,    /* user will do initial hash, we must choose salt for them */
    HEX_HM,              /* key to be hashed is a hexidecimal 4-byte integer */
    DECIMAL_HM,              /* key to be hashed is a decimal 4-byte integer */
    AB_HM,      /* key to be hashed is "A B", where A and B are (A,B) in hex */
    ABDEC_HM                                   /* like AB_HM, but in decimal */
  } mode;
  enum {
    STRING_HT,                                            /* key is a string */
    INT_HT,                                             /* key is an integer */
    AB_HT             /* dunno what key is, but input is distinct (A,B) pair */
  } hashtype;
  enum {
    NORMAL_HP,                                   /* just find a perfect hash */
    MINIMAL_HP                                /* find a minimal perfect hash */
  } perfect;
  enum {
    FAST_HS,                                                    /* fast mode */
    SLOW_HS                                                     /* slow mode */
  } speed;
};
typedef  struct hashform  hashform;

/* representation of a key */
struct key
{
  ub1        *name_k;                                      /* the actual key */
  ub4         len_k;                         /* the length of the actual key */
  ub4         hash_k;                 /* the initial hash value for this key */
  struct key *next_k;                                            /* next key */
/* beyond this point is mapping-dependent */
  ub4         a_k;                            /* a, of the key maps to (a,b) */
  ub4         b_k;                            /* b, of the key maps to (a,b) */
  struct key *nextb_k;                               /* next key with this b */
};
typedef  struct key  key;

/* things indexed by b of original (a,b) pair */
struct bstuff
{
  ub2  val_b;                                        /* hash=a^tabb[b].val_b */
  key *list_b;                   /* tabb[i].list_b is list of keys with b==i */
  ub4  listlen_b;                                        /* length of list_b */
  ub4  water_b;           /* high watermark of who has visited this map node */
};
typedef  struct bstuff  bstuff;

/* things indexed by final hash value */
struct hstuff
{
  key *key_h;                   /* tabh[i].key_h is the key with a hash of i */
};
typedef  struct hstuff hstuff;

/* things indexed by queue position */
struct qstuff
{
  bstuff *b_q;                        /* b that currently occupies this hash */
  ub4     parent_q;     /* queue position of parent that could use this hash */
  ub2     newval_q;      /* what to change parent tab[b] to to use this hash */
  ub2     oldval_q;                              /* original value of tab[b] */
};
typedef  struct qstuff  qstuff;

/* return ceiling(log based 2 of x) */
ub4 mylog2(ub4 x);

/* Given the keys, scramble[], and hash mode, find the perfect hash */
void findhash(bstuff **tabb, ub4 *alen, ub4 *blen, ub4 *salt,
		gencode *final, ub4 *scramble, ub4 *smax, key *keys, ub4 nkeys, 
		hashform *form);

/* private, but in a different file because it's excessively verbose */
int inithex(
	key      *keys,                                          /* list of all keys */
	ub4       nkeys,                                   /* number of keys to hash */
	ub4       alen,                    /* (a,b) has a in 0..alen-1, a power of 2 */
	ub4       blen,                    /* (a,b) has b in 0..blen-1, a power of 2 */
	ub4       smax,                   /* maximum range of computable hash values */
	ub4       salt,                     /* used to initialize the hash function */
	gencode  *final,                          /* output, code for the final hash */
	hashform *form);                                           /* user directives */

} // end namespace
} // end namespace
} // end namespace
} // end namespace

#endif // __PERFECT_H_78B797CE_FFAA_424F_A0C4_A57024857C95_
