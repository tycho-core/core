//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Thursday, 3 July 2008 7:03:20 PM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __GROW_ONLY_POD_ALLOCATOR_H_13B8F97C_A322_4FFD_8C71_44283ACCFE4C_
#define __GROW_ONLY_POD_ALLOCATOR_H_13B8F97C_A322_4FFD_8C71_44283ACCFE4C_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "core/core_abi.h"
#include "core/forward_decls.h"
#include "core/noncopyable.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace tycho
{
namespace core
{

	/// simple grow only allocator, very fast but all elements must be free'd together 
	/// and be pod type. this is useful when there are many small allocations and few
	/// deallocations. 
	class CORE_ABI grow_only_pod_allocator : noncopyable
	{
	public:
		/// constructor
		grow_only_pod_allocator(size_t page_size);
		
		/// destructor
		~grow_only_pod_allocator();
		
		/// cleanup all allocated memory
		void release_all();
		
		/// allocate some bytes
		void* allocate(size_t size);
		
		/// free some bytes, does nothing.
		void deallocate(void*) 
			{ /* no-op */ }		
		
		virtual void print_usage_stats(core::printing::text::formatter *printer);

	private:
		/// per page header
		struct page_header
		{
			size_t		page_size;
			size_t		used;
			char*		user;
			page_header *next_page;
		};

	private:
		void add_page();
		
	private:
		size_t		 m_page_size;
		page_header *m_current_page;
	}; 
	
} // end namespace
} // end namespace

#endif // __GROW_ONLY_POD_ALLOCATOR_H_13B8F97C_A322_4FFD_8C71_44283ACCFE4C_
