//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Monday, 24 March 2008 5:15:16 AM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __NEW_H_E1A8D455_3D03_44B6_B934_607713A2A795_
#define __NEW_H_E1A8D455_3D03_44B6_B934_607713A2A795_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "core/core_abi.h"
#include "core/memory/allocator.h"
#include <new>

#if 0
namespace std { struct nothrow_t; }

/// override global new operator
void* __CRTDECL operator new(size_t size);

/// override global new operator
void* __CRTDECL operator new(size_t size, const std::nothrow_t & ) throw();

/// override global array new operator
void* __CRTDECL operator new[](size_t size);

/// override global array new operator
void* __CRTDECL operator new[](size_t size, const std::nothrow_t & ) throw();

/// override global delete operator
void __CRTDECL operator delete(void *ptr);

/// override global delete operator
void __CRTDECL operator delete(void *ptr, const std::nothrow_t & ) throw();

/// override global array delete operator
void __CRTDECL operator delete[](void *ptr);

/// override global array delete operator
void __CRTDECL operator delete[](void *ptr, const std::nothrow_t & ) throw();

#endif // 0

namespace tycho
{
namespace core
{

	inline void* malloc(size_t size)
	{
		return allocator::malloc(static_cast<tycho::core::uint32>(size));
	}	

	inline void free(void* ptr)
	{
		return allocator::free(ptr);
	}
	
	
	/// Reallocate a previously allocated pointer.
	/// \warning This is only valid if the original allocation used default alignment.
	inline void* realloc(void* ptr, size_t size)
	{
		return allocator::realloc(ptr, size);
	}
	
} // end namespace
} // end namespace

#if TYCHO_GC
// stl allocation function
namespace std 
{
	inline void* __stl_new(size_t n) 
	{
		return tycho::core::allocator::malloc(n);
	}

	inline void __stl_delete(void* p) 
	{
		tycho::core::allocator::free(p);
	} 
} // end namespace
#endif


#endif // __NEW_H_E1A8D455_3D03_44B6_B934_607713A2A795_
