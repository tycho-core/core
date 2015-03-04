//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Tuesday, 8 April 2008 9:46:48 PM
// ------------------------------------------------------------------------------
// By Bob Jenkins, September 1996.
// lookupa.h, a hash function for table lookup, same function as lookup.c.
// Use this code in any way you wish.  Public Domain.  It has no warranty.
// Source is http://burtleburtle.net/bob/c/lookupa.h
// ------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __LOOKUPA_H_31E27863_476B_4074_9D55_73CF3804156C_
#define __LOOKUPA_H_31E27863_476B_4074_9D55_73CF3804156C_

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

	#define CHECKSTATE 8
	#define hashsize(n) ((ub4)1<<(n))
	#define hashmask(n) (hashsize(n)-1)

	ub4  lookup(ub1 *k, ub4 length, ub4 level);
	void checksum(ub1 *k, ub4 length, ub4 *state);

} // end namespace
} // end namespace
} // end namespace
} // end namespace

#endif // __LOOKUPA_H_31E27863_476B_4074_9D55_73CF3804156C_
