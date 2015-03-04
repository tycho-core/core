//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Tuesday, 25 March 2008 8:02:49 PM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __HASH_H_027B03DD_140D_459E_BA08_25671DF3BF05_
#define __HASH_H_027B03DD_140D_459E_BA08_25671DF3BF05_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "core/core_abi.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace tycho
{
namespace core
{

	/// hash a null terminated string
	inline unsigned int js_hash(const char *str)
	{
		unsigned int hash = 1315423911;
		unsigned int i    = 0;

		for(i = 0; *str; str++, i++)
		{
			hash ^= ((hash << 5) + (*str) + (hash >> 2));
		}

		return hash;			
	}

	/// hash a memory region
	inline unsigned int js_hash(const char *str, int len)
	{
		unsigned int hash = 1315423911;
		unsigned int i    = 0;

		for(i = 0; len; str++, i++, --len)
		{
			hash ^= ((hash << 5) + (*str) + (hash >> 2));
		}

		return hash;			
	}
	
	
	/// hash a memory region using the default hash function
	inline unsigned int hash(const char *str, int len)
	{
		// default is js_hash
		return js_hash(str, len);
	}

	/// hash a null terminated string using the default hash function
	inline unsigned int hash(const char *str)
	{
		// default is js_hash
		return js_hash(str);
	}

	/// hash the raw bytes of a structure, user must guarantee all unused bits in the 
	/// structure are initialise to a fixed value.
	template<class Type> unsigned int hash_raw_bytes(const Type &t)
	{
		return hash((const char*)&t, sizeof(Type));
	}
	
	inline unsigned int hash(core::ptr_type p)
	{
		return hash_raw_bytes(p);		
	}
	
} // end namespace
} // end namespace

#endif // __HASH_H_027B03DD_140D_459E_BA08_25671DF3BF05_
