//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Monday, 24 March 2008 4:45:08 AM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __ALLOCATOR_H_A97A4017_8357_43E8_9744_48D7165BE9A2_
#define __ALLOCATOR_H_A97A4017_8357_43E8_9744_48D7165BE9A2_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "core/core_abi.h"

#if TYCHO_LINUX
#include <stddef.h>
#endif // TYCHO_LINUX

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace tycho
{
namespace core
{
	class allocator_layer;
            
    /// allocator interface, normally called indirectly via new overloads
    class CORE_ABI allocator
    {
    public:
		enum allocation_flags
		{
			f_none		= 0,
			f_internal	= (1 << 0)	///< the request has come from within the allocator
		};
		
		static const int default_alignment = 4;
		
    public:
		/// allocate a new region of memory.
		static void*  malloc(size_t, int flags = f_none);
		
		/// allocated a new region of aligned memory.
		/// \warning It is not valid to realloc an aligned memory region
		static void*  malloc_aligned(size_t size, size_t alignment, int flags = f_none);
		
		/// realloc memory block. It will resize the existing block if possible or allocate a new block,
		/// copy the old contents to it and release the existing block.
		/// \warning This is only valid if the original allocation used default alignment.
		static void*  realloc(void* memory, size_t newsize, int flags = f_none);
		
		/// free a previous allocation
		static void   free(void* memory, int flags = f_none);
		
		/// \returns the size that was allocated for the passed pointer
		static size_t get_allocation_size(void* memory);
		
		/// set the allocator to use
		static void   set_allocator(allocator_layer *);
		
		/// set the debug allocator to use
		static void   set_debugger(allocator_layer *);
		
	private:
		static void initialise();
		
    };

} // end namespace
} // end namespace

#endif // __ALLOCATOR_H_A97A4017_8357_43E8_9744_48D7165BE9A2_
