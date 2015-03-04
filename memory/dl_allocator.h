//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Monday, 24 March 2008 4:50:23 AM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __dl_allocator_H_5D66B5C5_DC7A_44E9_BF52_076A9D291444_
#define __dl_allocator_H_5D66B5C5_DC7A_44E9_BF52_076A9D291444_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "core/core_abi.h"
#include "core/memory/allocator_layer.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace tycho
{
namespace core
{
	struct dl_allocation_summary
	{
		size_t pool_size;	/* maximum total allocated space */
		size_t free_blocks; /* number of free chunks */
		size_t allocated;	/* total allocated space */
		size_t free;		/* total free space */
	};


	/// Doug Lea allocator, modified from the official version in places.
    class CORE_ABI dl_allocator : public allocator_layer
    {
    public:
		/// will attempt to allocate ideal_size for our pool, 0 indicates get as much as possible
		dl_allocator(const size_t ideal_size = 0);

		/// relinquish pool
		~dl_allocator();

		/// allocation and deallocation
		void*  malloc(size_t size, int flags = 0, allocator_layer* next = NULL);
		void*  malloc_aligned(size_t size, size_t alignment, int flags = 0, allocator_layer* next = NULL);
		void*  realloc(void* memory, size_t newsize, int flags, allocator_layer* next);
		void   free(void* p, int flags = 0, allocator_layer* next = NULL);        

		/// info
		bool can_grow();
		size_t get_allocation_size(void* p, tycho::core::allocator_layer* next = NULL);

		/// NB: Only returns info for internal pool, not memory allocated from the system
		dl_allocation_summary get_alloction_summary();
	

	private:
		void initialise(size_t ideal_size );

		void* m_pool;
    };

} // end namespace
} // end namespace

#endif // __dl_allocator_H_5D66B5C5_DC7A_44E9_BF52_076A9D291444_
