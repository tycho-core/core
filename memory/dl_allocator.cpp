//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Monday, 24 March 2008 4:50:23 AM
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "core/platform.h"
#include "core/memory/dl_allocator.h"
#include "core/memory/dlmalloc.h"
#include "core/debug/assert.h"
#include <stdlib.h>

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////


// INVARIANTS
// - TYCHO_ASSERT does not use malloc, otherwise asserting a malloc failure 
//   would loop until the stack was blown.

namespace tycho
{
namespace core
{
	using namespace dlmalloc;

	//------------------------------------------------------------------------------------

	dl_allocator::dl_allocator(const size_t ideal_size) : m_pool(NULL) 
	{
		initialise(ideal_size);
	}

	//------------------------------------------------------------------------------------

	dl_allocator::~dl_allocator()
	{
		if (m_pool)
		{
			destroy_mspace(m_pool);
			m_pool = NULL;
		}
	}

	//------------------------------------------------------------------------------------

	void* dl_allocator::malloc(size_t size, int, allocator_layer *)
	{
		if (!m_pool)
		{
			/// initial construction failed, get as much as possible
			initialise(0);
		}
		TYCHO_ASSERT(m_pool);

		if (m_pool)
		{
			return mspace_malloc(m_pool, size);
		}

		return NULL;
	}
	
	//------------------------------------------------------------------------------------

	void* dl_allocator::malloc_aligned(size_t size, size_t alignment, int, allocator_layer*)
	{
		if (!m_pool)
		{
			/// initial construction failed, get as much as possible
			initialise(0);
		}
		TYCHO_ASSERT(m_pool);

		if (m_pool)
		{
			return mspace_memalign(m_pool, alignment, size);
		}

		return NULL;
	}

	//------------------------------------------------------------------------------------

	void* dl_allocator::realloc(void* memory, size_t newsize, int /*flags*/, allocator_layer*)
	{
		if (m_pool)
		{
			return mspace_realloc(m_pool, memory, newsize);
		}
		
		return NULL;
	}
	
	//------------------------------------------------------------------------------------

	void dl_allocator::free(void* p, int, allocator_layer*)
	{
		if (m_pool)
		{
			mspace_free(m_pool, p);
		}
	}

	//------------------------------------------------------------------------------------

	size_t dl_allocator::get_allocation_size(void* p, allocator_layer*)
	{
		if (m_pool)
		{
			return memsize(p);
		} 
		else
		{
			return 0;
		}
	}

	//------------------------------------------------------------------------------------

	void dl_allocator::initialise(size_t ideal_size )
	{
		TYCHO_ASSERT(!m_pool);

		void* base = 0;
		if (ideal_size > 0)
		{
			base = get_os_mem(ideal_size);
		}
		else
		{
			static const size_t max_size  = 25165824; //24MB		//get_max_os_mem_size
			static const size_t step_down =     4096; // 4KB
			for (ideal_size = max_size; ideal_size > 0; ideal_size -= step_down)
			{
				base = get_os_mem(ideal_size);
				if (base)
				{
					break;
				}
			}
		}

		if (base)
		{
			m_pool = create_mspace_with_base(base, ideal_size, 0);
			return;
		}
	}

	//------------------------------------------------------------------------------------

	dl_allocation_summary dl_allocator::get_alloction_summary()
	{
		dl_allocation_summary summary = {0};
		if (m_pool)
		{
			// arena should = usmblks as we disallow internal system allocation
			mallinfo info		= mspace_mallinfo(m_pool);
			summary.pool_size	= info.usmblks;
			summary.free_blocks = info.ordblks;
			summary.allocated	= info.uordblks;
			summary.free		= info.fordblks;			
		}
		return summary;
	}

	//------------------------------------------------------------------------------------

	bool dl_allocator::can_grow()
	{
		return dl_only_mspaces();
	}

	//------------------------------------------------------------------------------------

} // end namespace
} // end namespace
