//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Sunday, 30 March 2008 7:31:58 PM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __CUSTOM_ALLOCATOR_H_D3DBE306_C767_4C05_8F78_B9E7701D6089_
#define __CUSTOM_ALLOCATOR_H_D3DBE306_C767_4C05_8F78_B9E7701D6089_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "core/core_abi.h"
#include "allocator_layer.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace tycho
{
namespace core
{

	/// allocator that allows :-
	///			- high / low memory allocation to reduce fragmentation. temporary
	///			  allocations go high and long term (i.e. per level) go low.
	///			- coalesce on free to maximise success when allocating large blocks
	///			- prefer to leave large blocks large and use the smallest possible
	///			  free block to maximise large contiguous blocks
	///			- multi-threading support, avoid locking and false sharing (cache line
	///			  access from 2 or more different threads).
    class CORE_ABI custom_allocator
    {
    public:
    
    private:
    
    };

} // end namespace
} // end namespace

#endif // __CUSTOM_ALLOCATOR_H_D3DBE306_C767_4C05_8F78_B9E7701D6089_
