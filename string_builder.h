//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Sunday, 27 April 2008 6:50:08 PM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __STRING_BUILDER_H_9099C22C_A74D_49DF_817E_50902F495174_
#define __STRING_BUILDER_H_9099C22C_A74D_49DF_817E_50902F495174_

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

	/// wrapper to simplify piecemeal construction of a string 
    class CORE_ABI string_builder
	{
	public:
		/// use user supplied memory, caller is responsible for freeing
		/// this memory if necessary.
		string_builder(char* buffer, int buffer_size);
	
		/// constructor, allocates memory off the heap
		string_builder(int buffer_size);
		
		/// destructor
		~string_builder();
			
		/// \returns null terminated string
		const char* string() const;
		
		/// append a single character to the string	
		void append(char c);
		
		/// append an integer to the string
		void append(int c);
		
		/// append a printf style formatted string
		void append(const char* format, ...);
		
		/// copy to the destination string, truncating if it is too small.
		void copy_to(char *dst, int len);
		
		/// \returns free space left in internal buffer
		int free() const
			{ return m_size - m_pos - 1; }
		
		/// length of the string currently in the buffer
		int length() const
			{ return m_pos; }
		
		/// \returns true if the buffer is full
		bool full() const
			{ return m_pos == m_size-1; }
		
		/// reset the buffer
		void clear() 
			{ m_pos = 0; *m_str = 0;}	
		
	private:
		// noncopyable
		void operator=(const string_builder &);
			
	private:
		mutable char *m_str; // needs to be writable in the string() function so it can be null terminated
		bool m_delete_str;
		int  m_pos;
		int	 m_size;
	}; 
	
} // end namespace
} // end namespace

#endif // __STRING_BUILDER_H_9099C22C_A74D_49DF_817E_50902F495174_
