//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Tuesday, 25 March 2008 5:54:01 PM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __MEMORY_H_C3F57E38_22F3_4587_9B09_BA513A88CCE0_
#define __MEMORY_H_C3F57E38_22F3_4587_9B09_BA513A88CCE0_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "core/core_abi.h"
#if TYCHO_PC
#include <memory.h>
#endif

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace tycho
{
namespace core
{
#if TYCHO_PC
#	define TYCHO_ALLOCA(size_)	_alloca(size_)
#elif TYCHO_GC
#	define TYCHO_ALLOCA(size_)	alloca(size_)
#elif TYCHO_LINUX
#	define TYCHO_ALLOCA(size_)	__builtin_alloca(size_)
#else
#	error "Unsupported Platform"
#endif

	/// write byte pattern to a memory region
	CORE_ABI void* mem_set(void *ptr, int pattern, size_t len);

	/// copy non-overlapping memory regions
	CORE_ABI void* mem_cpy(void *dst, const void *src, size_t len);

	/// write a 32bit pattern to a memory region
	CORE_ABI void* mem_set32(void *ptr, int pattern, size_t len);
		
	/// zero a memory region
	CORE_ABI void* mem_zero(void *p, size_t len);

	/// compare two memory regions
	CORE_ABI int mem_cmp(const void *p1, const void *p2, size_t len);
	
	/// zero memory region
	template<class T> void mem_zero(T &t)
	{
		core::mem_set(&t, 0L, sizeof(T));
	}

	///	deletes the pointer and nulls it out to avoid stray writes
	template<class T> void safe_delete(T *&t)
	{
		delete t;
		t = 0;
	}        
	
	///	deletes the pointer to array and nulls it out to avoid stray writes
	template<class T> void safe_delete_array(T *&t)
	{
		delete[] t;
		t = 0;
	}        
	

} // end namespace
} // end namespace

#endif // __MEMORY_H_C3F57E38_22F3_4587_9B09_BA513A88CCE0_
