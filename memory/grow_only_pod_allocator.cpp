//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Thursday, 3 July 2008 7:03:21 PM
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "grow_only_pod_allocator.h"
#include "core/printing/text/formatter.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////
namespace tycho
{
namespace core
{

	//------------------------------------------------------------------------------------

	grow_only_pod_allocator::grow_only_pod_allocator(size_t page_size) :
		m_page_size(page_size),
		m_current_page(0)
	{			
	}

	//------------------------------------------------------------------------------------

	grow_only_pod_allocator::~grow_only_pod_allocator()
	{
		release_all();
	}		
	
	
	//------------------------------------------------------------------------------------

	void grow_only_pod_allocator::release_all()
	{
		page_header *page = m_current_page;
		while(page)
		{
			page_header *next = page->next_page;
			core::free(page);
			page = next;
		}
		m_current_page = 0;
	}
	
	//------------------------------------------------------------------------------------

	void* grow_only_pod_allocator::allocate(size_t size)
	{
		// check to see if we have room in current page
		if(!m_current_page || ((m_current_page->used + size + sizeof(page_header)) > m_current_page->page_size))
			add_page();
		
		void *ptr = (char*)m_current_page->user + m_current_page->used;
		m_current_page->used += size;				
		return ptr;
	}
		
	//------------------------------------------------------------------------------------
		
	void grow_only_pod_allocator::add_page()
	{
		// allocate space and initialise
		page_header *new_page = (page_header *)core::malloc(m_page_size);
		new_page->page_size = m_page_size;
		new_page->used		= 0;
		new_page->user		= (char*)new_page + sizeof(page_header);
		
		// link to front of list
		new_page->next_page = m_current_page;
		m_current_page = new_page;
	}

	//------------------------------------------------------------------------------------

	void grow_only_pod_allocator::print_usage_stats(core::printing::text::formatter*printer)
	{
		size_t num_pages = 0;
		size_t total_mem = 0;
		size_t used_mem = 0;
		
		page_header *page = m_current_page;
		while(page)
		{
			++num_pages;
			total_mem += page->page_size;
			used_mem += page->used + sizeof(page_header);
			page = page->next_page;
		}
		
		printer->format("Memory usage : %d pools : %d kb total : %d kb used : %d kb free\n", num_pages, total_mem / 1024, used_mem / 1024, (total_mem-used_mem) / 1024);
	}

    //--------------------------------------------------------------------

} // end namespace
} // end namespace
