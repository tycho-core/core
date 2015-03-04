//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Saturday, 12 April 2008 1:30:57 PM
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "core/memory/ansi_allocator.h"
#include "core/debug/assert.h"
#include <stdlib.h>


//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////
namespace tycho
{
namespace core
{

	//------------------------------------------------------------------------------------

	void* ansi_allocator::malloc(size_t size, int /*flags*/, allocator_layer *)
	{
		return ::malloc(size);
	}
	
	//------------------------------------------------------------------------------------

	void* ansi_allocator::malloc_aligned(size_t size, size_t alignment, int flags, allocator_layer *)
	{
		if (alignment == 0)
			return malloc(size, flags, 0);
#if !TYCHO_OSX //OSXTODO
		else			
		    return memalign(alignment, size);
#else
        TYCHO_NOT_IMPLEMENTED;
        return 0;
#endif
	}
	
	//------------------------------------------------------------------------------------

	void* ansi_allocator::realloc(void* memory, size_t newsize, int /*flags*/, allocator_layer*)
	{
		return ::realloc(memory, newsize);
	}

	//------------------------------------------------------------------------------------

	void ansi_allocator::free(void *p, int /*flags*/, allocator_layer *)
	{
		::free(p);
	}

	/// get the size of the allocation for that passed pointer 
	size_t ansi_allocator::get_allocation_size(void *ptr, allocator_layer * /*next*/)
	{
#ifdef TYCHO_OSX //PORTME
        return 0;
#else
		return malloc_usable_size(ptr);
#endif
	}	

	//------------------------------------------------------------------------------------

} // end namespace
} // end namespace

