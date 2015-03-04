//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Monday, 24 March 2008 1:04:52 PM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __ALLOCATOR_LAYER_H_3227F50B_0578_4C14_825C_902C70DD0273_
#define __ALLOCATOR_LAYER_H_3227F50B_0578_4C14_825C_902C70DD0273_

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

	/// base allocator, this defines the interface for all memory allocation and is 
	/// overridable by the user on the first call to allocate memory. 
    class CORE_ABI allocator_layer
    {
    public:
		allocator_layer() {}
		virtual ~allocator_layer() {}
		virtual void*  malloc(size_t size, int flags, allocator_layer* next) = 0;
		virtual void*  realloc(void* memory, size_t newsize, int flags, allocator_layer* next) = 0;
		virtual void*  malloc_aligned(size_t size, size_t alignment, int flags, allocator_layer* next) = 0;
		virtual void   free(void* memory, int flags, allocator_layer *next) = 0;        

		/// get the size of the allocation for the passed pointer 
		virtual size_t get_allocation_size(void* memory, allocator_layer *next) = 0;
    };
    
} // end namespace
} // end namespace

#endif // __ALLOCATOR_LAYER_H_3227F50B_0578_4C14_825C_902C70DD0273_
