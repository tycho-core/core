//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Tuesday, 25 March 2008 4:27:57 PM
//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "../utilities.h"
#include "core/debug/assert.h"
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <execinfo.h>

//////////////////////////////////////////////////////////////////////////////
// DEFINES
//////////////////////////////////////////////////////////////////////////////

namespace tycho
{
namespace core
{
namespace debug
{
	//------------------------------------------------------------------------------------

	namespace detail
	{
		static bool g_initialised = false;
		
		void initialise()
		{
			if(g_initialised)
				return;
								
			g_initialised = true;
		}
	
	} // end namespace
	
	//------------------------------------------------------------------------------------

	/// generate a stack trace
	/// \warning For this function to work correctly with optimisations enabled you need to disable the 
	///          framer pointer omission optimisation using /Oy-
	CORE_ABI int stacktrace(core::ptr_type *trace, size_t max_depth)
	{	
		detail::initialise();
		int size = sizeof(ptr_type) * max_depth;
		return ::backtrace(trace, size);
	}

	//------------------------------------------------------------------------------------

	CORE_ABI bool convert_address_to_function(core::ptr_type address, char *name_buf, char *file_buf, int *line)
	{
		detail::initialise();
		return false;
	}

	//------------------------------------------------------------------------------------

	CORE_ABI void write_ln(const char *format, ...)
	{
		va_list args;
		va_start(args, format);
		const int BufSize = 4096;
		char buf[BufSize];
		buf[0] = 0;
		int nc = vsnprintf(buf, BufSize-1, format, args);			
		TYCHO_ASSERT(nc > 0);
		if(nc > 0)
		{
			buf[nc] = '\n';
			buf[nc+1] = 0;
			printf("%s", buf);
		}
	}

	//------------------------------------------------------------------------------------

	CORE_ABI void write(const char *format, ...)
	{
		va_list args;
		va_start(args, format);
		const int BufSize = 4096;
		char buf[BufSize];
		buf[0] = 0;
		vsnprintf(buf, BufSize, format, args);
		int nc = vsnprintf(buf, BufSize-1, format, args);			
		TYCHO_ASSERT(nc > 0);
		if(nc > 0)
		{
			buf[nc] = ' ';
			buf[nc+1] = 0;
			printf("%s", buf);
		}
	}

	//------------------------------------------------------------------------------------
	
} // end namespace
} // end namespace
} // end namespace
