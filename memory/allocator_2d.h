//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Sunday, 1 June 2008 5:18:41 PM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __ALLOCATOR_2D_H_41ACB549_4976_47BD_870D_9D43D3B85168_
#define __ALLOCATOR_2D_H_41ACB549_4976_47BD_870D_9D43D3B85168_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "core/core_abi.h"
#include "core/noncopyable.h"
#include <list>
#include <algorithm>

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace tycho
{
namespace core
{

	/// simple 2d allocator, subdivides textures vertically into spans that are subdivided into allocation units.
	/// This is initially intended for string allocation which has lots of short wide allocations, need to look
	/// at its actual performance when we have a realistic load.
	class CORE_ABI allocator_2d : core::noncopyable
	{
	private:
		struct vspan;
		
		/// horizontal span
		struct hspan
		{
			hspan() :
				start(-1), end(-1), width(-1)
			{}
			
			int start;
			int end;
			int width;
		};
		
		typedef std::list<hspan> hspan_list;
		
		/// vertical span 
		struct vspan
		{
			vspan() : 
				start(-1), end(-1), height(-1)
			{}
			
			int start;			///< starting point of this span
			int end;			///< ending point of this span
			int height;			///< height of this vertical span
			
			hspan_list used;		///< list of used or partially used spans
			hspan_list whitespace;	///< list of whitespace spans
		};

		typedef std::list<vspan> vspan_list;
			
	public:
		struct tag
		{
			int x;
			int y;
			int width;
			int height;
		
		private:
			friend class allocator_2d;
			vspan_list::iterator vspan;
			hspan_list::iterator hspan;
		};
		
	public:
		/// constructor
		/// \param width width of surface to subdivide
		/// \param height height of surface to subdivide		
		allocator_2d(int width, int height);
				
		/// destructor
		~allocator_2d();

		/// \returns the maximum allocation width
		int get_max_width() const;
		
		/// \returns the maximum allocation height
		int get_max_height() const;
		
		/// clear all allocations
		void clear();
		
		/// allocate a chunk 
		tag* allocate(int width, int height);
				
		/// free a chunk
		void free(tag*);
				
	private:
		int calc_whitespace_area();
		int calc_used_area();
				
	private:		
		int	   m_width;
		int	   m_height;
		vspan_list m_used;
		vspan_list m_whitespace;		
	};

} // end namespace
} // end namespace

#endif // __ALLOCATOR_2D_H_41ACB549_4976_47BD_870D_9D43D3B85168_
