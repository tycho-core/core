//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Tuesday, 25 March 2008 9:39:16 PM
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "core/string.h"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////
namespace tycho
{
namespace core
{

	//------------------------------------------------------------------------------------

	/// pass through to std lib for now	
	/// \todo  replace with platform independent one guaranteed to not allocate heap memory so we can use it execptions etc..
	CORE_ABI int snprintf(char *dst, int buffer_size, const char *format, ...)
	{
		va_list args;
		va_start(args, format);
		return _vsnprintf(dst, buffer_size, format, args);
	}

	//------------------------------------------------------------------------------------

	CORE_ABI int vsnprintf(char *dst, int buffer_size, const char* format, va_list args)
	{
		return _vsnprintf(dst, buffer_size-1, format, args);			
	}

	//------------------------------------------------------------------------------------	

} // end namespace
} // end namespace
