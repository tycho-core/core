//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Wednesday, 26 March 2008 4:30:31 AM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __ALLOCATOR_H_9EAC83CE_D82C_4D63_B763_589DD636BB53_
#define __ALLOCATOR_H_9EAC83CE_D82C_4D63_B763_589DD636BB53_

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

	/// passes through to default memory allocator
	class default_allocator
	{
	public:
		void* allocate(size_t n, int flags = 0)
		{
			return allocator::malloc(n, flags);
		}
		
		void* allocate(size_t n, size_t alignment, int flags = 0)
		{
			return allocator::malloc_aligned(n, alignment, flags);
		}
		
		void deallocate(void* p, size_t /*n*/)
		{
			return allocator::free(p, allocator::f_none);
		}
	};

	template<class T, class Allocator>
	T* default_construct(Allocator &a)
	{
		return new(a.allocate(sizeof(T))) T();
	}

	template<class T, class Allocator>
	T* allocate(Allocator &a)
	{
		return (T*)a.allocate(sizeof(T));
	}
	
	template<class T, class V>
	T* construct(T *t, const V &val)
	{
		t = new(t) T(val);
		return t;
	}


	template<class T, class Allocator>
	void destroy(T *t, Allocator &a)
	{
		t->~T();
		a.deallocate(t, 1);
	}

} // end namespace
} // end namespace

#endif // __ALLOCATOR_H_9EAC83CE_D82C_4D63_B763_589DD636BB53_
