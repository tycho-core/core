//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Sunday, 27 April 2008 6:50:09 PM
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "string_builder.h"
#include "core/debug/assert.h"
#include "core/string.h"
#include "core/memory.h"
#include <stdarg.h> 
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////
namespace tycho
{
namespace core
{

	//------------------------------------------------------------------------------------

	string_builder::string_builder(char* buffer, int buffer_size) :
		m_str(buffer),
		m_delete_str(false),
        m_pos(0),
        m_size(buffer_size)
	{
		core::mem_zero(buffer, buffer_size);
	}
	
	//------------------------------------------------------------------------------------

	string_builder::string_builder(int buffer_size) : 
        m_delete_str(true),
		m_pos(0),
		m_size(buffer_size)
	{
		TYCHO_ASSERT(buffer_size);
		m_str = new char[buffer_size];
	}
	
	//------------------------------------------------------------------------------------

	string_builder::~string_builder()
	{
		if(m_delete_str)
			delete[] m_str;			
	}
		
	//------------------------------------------------------------------------------------

	const char* string_builder::string() const 
	{
		m_str[m_pos] = 0;
		return m_str; 
	}
	
	//------------------------------------------------------------------------------------

	void string_builder::append(char c)
	{
		if(full())
			return;
		m_str[m_pos++] = c;
	}
	
	
	//------------------------------------------------------------------------------------

	void string_builder::append(const char* format, ...)
	{
		va_list args;
		va_start(args, format);
#if TYCHO_PC
		int nc = _vsnprintf(&m_str[m_pos], free(), format, args);
#else
		int nc = vsnprintf(&m_str[m_pos], free(), format, args);
#endif // TYCHO_PC
		m_pos += nc;
	}
		
	//------------------------------------------------------------------------------------

	void string_builder::append(int c)
	{
		char buf[64] = { 0 };
		int len = core::snprintf(buf, 63, "%d", c);
		int free_space = free();
		len = (len < free_space) ? len : free_space;
		strncpy(&m_str[m_pos], buf, len);
		m_pos += len;
	}
	
	//------------------------------------------------------------------------------------

	void string_builder::copy_to(char *dst, int len)
	{
		int c = (len < length()) ? len : length();
		strncpy(dst, string(), c);
	}  
	
	//------------------------------------------------------------------------------------
	
} // end namespace
} // end namespace
