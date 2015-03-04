//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Monday, 24 March 2008 6:15:38 AM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __ANSI_ALLOCATOR_H_8AC55944_530C_415E_B41D_5011EE777B96_
#define __ANSI_ALLOCATOR_H_8AC55944_530C_415E_B41D_5011EE777B96_

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

	/// uses compiler supplied functions
    class CORE_ABI ansi_allocator : public allocator_layer
    {
    public:
		ansi_allocator();
		~ansi_allocator();
		virtual void*  malloc(size_t size, int flags, allocator_layer *next);
		virtual void*  malloc_aligned(size_t size, size_t alignment, int flags, allocator_layer *next);
		virtual void*  realloc(void* memory, size_t newsize, int flags, allocator_layer *next);
		virtual void   free(void *, int flags, allocator_layer *next);              
		virtual size_t get_allocation_size(void *, allocator_layer *next);
    };

} // end namespace
} // end namespace

#endif // __ANSI_ALLOCATOR_H_8AC55944_530C_415E_B41D_5011EE777B96_
