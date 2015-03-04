//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Sunday, 1 June 2008 5:18:42 PM
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "allocator_2d.h"
#include "core/memory.h"
#include "core/debug/assert.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////
namespace tycho
{
namespace core
{

	//------------------------------------------------------------------------------------

	allocator_2d::allocator_2d(int width, int height) :
		m_width(width),
		m_height(height)
	{
		// initialise vertical whitespace span
		vspan s;
		s.start = 0;
		s.end = height;
		s.height = height;		
		m_whitespace.push_back(s);
	}
			
	//------------------------------------------------------------------------------------

	allocator_2d::~allocator_2d()
	{
		clear();
	}
		
	//------------------------------------------------------------------------------------

	int allocator_2d::get_max_width() const
	{
		return m_width;
	}
	
	//------------------------------------------------------------------------------------

	int allocator_2d::get_max_height() const
	{
		return m_height;
	}
	
	//------------------------------------------------------------------------------------

	void allocator_2d::clear()
	{
		m_used.clear();
		m_whitespace.clear();
	}
	
	//------------------------------------------------------------------------------------

	allocator_2d::tag* allocator_2d::allocate(int width, int height)
	{
		if(width > m_width || height > m_height)
			return 0;
			
		// todo : pick whether to place in used or split whitespace. just using used if available for now		
		float req_area = float(width * height);
		float best_fit = 0;
		vspan*		best_vspan = 0;
		vspan_list::iterator best_vit;
		hspan_list::iterator best_hspan;		
		vspan_list::iterator vit = m_used.begin();
		vspan_list::iterator vend = m_used.end();
		for(; vit != vend; ++vit)
		{
			if(height > vit->height)
				continue;				
			hspan_list::iterator hit = vit->whitespace.begin();
			hspan_list::iterator hend = vit->whitespace.end();
			for(; hit != hend; ++hit)
			{
				if(width > hit->width)
					continue;
				float area = float(vit->height * hit->width);
				float fit = req_area / area;
				if(fit > best_fit)
				{
					best_vspan = &*vit;
					best_hspan = hit;
					best_fit = fit;
					best_vit = vit;
				}
			}			
		}
		
		if(best_vspan)
		{
			TYCHO_ASSERT(best_hspan != best_vspan->whitespace.end());
			// found a spot for it so create a new block and link into the used blocks and
			// reduce and possibly free the whitespace block
			hspan new_span;
			new_span.start = best_hspan->start;
			new_span.end   = best_hspan->start + width;
			new_span.width = width;
			best_vspan->used.push_back(new_span);			
			
			// update the whitespace span
			best_hspan->width -= width;
			if(best_hspan->width == 0)
				best_vspan->whitespace.erase(best_hspan);
			else
				best_hspan->start += width;
			
			// return a tag describing this allocation
			tag* t = new tag;
			t->width = width;
			t->height = height;
			t->x = new_span.start;
			t->y = best_vspan->start;
			t->hspan = --best_vspan->used.end();
			t->vspan = best_vit;
			return t;
		}

		// no space available in used blocks so try to allocate from whitespace
		vit = m_whitespace.begin();
		vend = m_whitespace.end();
		best_vspan = 0;
		best_fit = 0;
		for(; vit != vend; ++vit)
		{
			if(height > vit->height)
				continue;
			float area = float(vit->height * m_width);
			float fit = req_area / area;
			if(fit > best_fit)
			{
				best_vspan = &*vit;
				best_fit = fit;
				best_vit = vit;
			}
		}
		if(!best_vspan)
		{
			// no free space
			return 0;
		}
		
		// split vspan, move new span to used list and allocate block from it
		vspan new_vspan;
		new_vspan.start = best_vspan->start;
		new_vspan.end   = best_vspan->start + height;
		new_vspan.height = height;
		hspan new_hspan;
		new_hspan.start = 0;
		new_hspan.end = m_width;
		new_hspan.width = m_width;
		new_vspan.whitespace.push_back(new_hspan);
		m_used.push_back(new_vspan);
		
		// adjust whitespace span
		best_vspan->height -= height;
		if(best_vspan->height == 0)
			m_whitespace.erase(best_vit);
		else
			best_vspan->start += height;
			
		TYCHO_ASSERT(best_vspan->start >= 0);
		TYCHO_ASSERT(best_vspan->end <= m_height);
		TYCHO_ASSERT(best_vspan->height <= m_height);
		TYCHO_ASSERT(best_vspan->height >= 0);
		
		tag* t = allocate(width, height);						
		
		TYCHO_ASSERT((calc_whitespace_area() + calc_used_area()) == (m_width*m_height));
		
		return t;				
	}
	
	//------------------------------------------------------------------------------------
	
	void allocator_2d::free(tag* t)
	{
		if(!t)
			return;
		
		// move this allocations hspan back onto the whitespace list and merge with any existing 
		// whitespace spans. merging can involve up to 2 merges if this span is straddled by whitespace.
		vspan_list::iterator vspan = t->vspan;
		hspan_list::iterator hspan = t->hspan;
		allocator_2d::hspan s = *hspan;
		vspan->used.erase(hspan);
		
		// loop over existing whitespace trying to merge the span. Its fine to do this is
		// a single loop as there are only a maximum of 2 merges to be done and we grow our
		// block to merge as we encounter a left or right merge. 
		hspan_list::iterator wit = vspan->whitespace.begin();
		hspan_list::iterator wend = vspan->whitespace.end();
		hspan_list::iterator merged = wend;
		for(; wit != wend; ++wit)
		{		
			// check for left or right merges
			if(wit->end == s.start)
			{
				// if we have already merged then merge the two spans, remove one
				// and we're all done. existing merge must be a right merge
				if(merged != wend)
				{
					wit->end += merged->width;
					wit->width += merged->width;
					vspan->whitespace.erase(merged);
					merged = wit;
					break;					
				}
				else
				{
					wit->end += s.width;
					wit->width += s.width;
					merged = wit;
				}				
			}
			else if (s.end == wit->start)
			{
				if(merged != wend)
				{
					wit->start -= merged->width;
					wit->width += merged->width;
					vspan->whitespace.erase(merged);
					merged = wit;
					break;
				}
				else
				{
					wit->start -= s.width;
					wit->width += s.width;
					merged = wit;
				}
			}
		}
		
		// if we were unable to merge with any just add it to the whitespace list
		if(merged == wend)
			vspan->whitespace.push_back(s);		

		// if there are no more used hspan's in this vspan add it back to the whitespace
		// list merging with any existing whitespace spans.
		if(vspan->used.empty())
		{
			allocator_2d::vspan s = *vspan;
			m_used.erase(vspan);
			
			// loop over existing whitespace trying to merge the span
			vspan_list::iterator wit = m_whitespace.begin();
			vspan_list::iterator wend = m_whitespace.end();
			vspan_list::iterator merged = wend;
			for(; wit != wend; ++wit)
			{		
				// check for left or right merges
				if(wit->end == s.start)
				{
					// if we have already merged then merge the two spans, remove one
					// and we're all done. existing merge must be a right merge
					if(merged != wend)
					{
						wit->end += merged->height;
						wit->height += merged->height;
						m_whitespace.erase(merged);
						merged = wit;
						break;					
					}
					else
					{
						wit->end += s.height;
						wit->height += s.height;
						merged = wit;
					}				
				}
				else if (s.end == wit->start)
				{
					if(merged != wend)
					{
						wit->start -= merged->height;
						wit->height += merged->height;
						m_whitespace.erase(merged);
						merged = wit;
						break;
					}
					else
					{
						wit->start -= s.height;
						wit->height += s.height;
						merged = wit;
					}
				}
			}
			
			// if we were unable to merge with any just add it to the whitespace list
			if(merged == wend)
				m_whitespace.push_back(s);					
		}
				
		delete t;
	}

	//------------------------------------------------------------------------------------

	int allocator_2d::calc_whitespace_area()
	{
		int area = 0;
				
		// vspan whitespace
		{
			vspan_list::iterator vit = m_whitespace.begin();
			vspan_list::iterator vend = m_whitespace.end();
			for(; vit != vend; ++vit)
			{
				area += vit->height * m_width;
			}		
		}
		
		// whitespace in used vspan's
		{
			vspan_list::iterator vit = m_used.begin();
			vspan_list::iterator vend = m_used.end();
			for(; vit != vend; ++vit)
			{
				hspan_list::iterator wit = vit->whitespace.begin();
				hspan_list::iterator wend = vit->whitespace.end();
				for(; wit != wend; ++wit)
				{
					area += vit->height * wit->width;	
				}		
			}
		}
		
		return area;
	}
	
	//------------------------------------------------------------------------------------
	
	int allocator_2d::calc_used_area()
	{
		int area = 0;
		vspan_list::iterator vit = m_used.begin();
		vspan_list::iterator vend = m_used.end();
		for(; vit != vend; ++vit)
		{
			hspan_list::iterator wit = vit->used.begin();
			hspan_list::iterator wend = vit->used.end();
			for(; wit != wend; ++wit)
			{
				area += vit->height * wit->width;	
			}		
		}
		return area;
	}

	//------------------------------------------------------------------------------------

} // end namespace
} // end namespace
