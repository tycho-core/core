//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Tuesday, 8 April 2008 10:22:15 PM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __PERFECT_HASH_H_95806932_9586_43E7_96C2_F1FC3F3BB443_
#define __PERFECT_HASH_H_95806932_9586_43E7_96C2_F1FC3F3BB443_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "core/core_abi.h"
#include <vector>

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace tycho
{
namespace core
{

	/// Utility class to generate perfect hashes of a list of strings. A perfect
	/// hash generates a hash function such that each string maps to a unique integer
	/// so guaranteeing no collisions. We use a variation on this called a minimal
	/// hash function which adds the guarantee that for n input strings you only
	/// require n buckets. This uses Bob Jenkins code internally, this has been reworked to
	/// compile in c++, use our memory handling and remember what hash function to use and
	/// its parameters instead of generating C code. http://burtleburtle.net/bob for all the
	/// details of the algorithms.
    class CORE_ABI perfect_hash
    {
    public:
		/// this contains the function to use to generate the hash and all its parameters
		struct hash_function
		{
			core::uint8  version;			///< version of this structure
			core::uint8  size;				///< size of hash function including its tables
			core::uint16 type;				///< type of function \ref gencode
			core::uint16 nkeys;				///< number of keys
			core::uint16 mask;				///< mask value
			core::uint16 shift;				///< shift value
			core::uint16 scramble_offset;	///< offset in table to the start of the scramble table
			core::uint32 init;				///< lookup seed
			core::uint8  tables[1];			///< array of tab and scramble values, extends after struct to size
		};
		
		/// flags used to direct hash function creation
		enum create_flags
		{
			cf_invalid = 0,
			cf_minimal,					///< create a minimal perfect hash, hash will be in 0..nkeys-1
			cf_perfect,					///< create a perfect hash, Hash will be in 0..n-1, where n >= nkeys
			cf_default = cf_minimal,	///< default create flags, use this unless you really want to do some particular.
		};
		
    public:		
		/// create a new perfect hash function for the passed set of strings. 
		/// \returns pointer to new function, the user is responsible for deleting
		/// its memory via \ref tycho::core::allocator::free
		static hash_function* create_function(const std::vector<const char*>& inputs, int create_flags);
		
		/// lookup the index of the passed string or -1 if it doesn't exist
		/// \warning the caller still needs to do a final compare to check that another invalid key hasn't mapped
		///          to the same value
		static int lookup(const hash_function &func, const char *key, int len);
		
	private:
		/// \name hash functions for all the variants that can be generated for the perfect hash function.
		//@{
		static core::uint32 hash_CT_NORM_LE_UB4BITS_SCRAMBLED(const hash_function &func, const char *key, int len);
		static core::uint32 hash_CT_ZERO(const hash_function &func, const char *key, int len);
		//@}	
    };

} // end namespace
} // end namespace

#endif // __PERFECT_HASH_H_95806932_9586_43E7_96C2_F1FC3F3BB443_
