//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Tuesday, 25 March 2008 6:09:57 PM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __STRING_H_271BD3F0_F43F_47DF_B686_2F44D31C65CA_
#define __STRING_H_271BD3F0_F43F_47DF_B686_2F44D31C65CA_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "core/core_abi.h"
#include <stdarg.h>

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace tycho
{
namespace core
{

	/// copies a maximum on buffer_len-1 characters from source to destination,
	/// always null terminates.
	CORE_ABI char* strncpy(char *dst, const char *src, int buffer_len);
	
	/// copies a string from source to destination. 
	/// \warning sizes are not check so must be validated before calling
	CORE_ABI char* strcpy(char *dst, const char *src);

	/// pass through to std lib for now
	/// \todo  replace with platform independent one guaranteed to not allocate heap memory
	//      so we can use it during exceptions etc when we cannot.
	CORE_ABI int snprintf(char *dst, int buffer_size, const char *pattern, ...);

	/// \ref snprintf
	CORE_ABI int vsnprintf(char *dst, int buffer_size, const char* format, va_list args);

	/// \returns the length of the passed string
	CORE_ABI int strlen(const char *p);
	
	/// compare two strings
	CORE_ABI int strcmp(const char* s1, const char* s2);

	/// compare two strings, case insensitive
	CORE_ABI int stricmp(const char* s1, const char* s2);
	
	/// compare n characters between two strings
	CORE_ABI int strncmp(const char* s1, const char* s2, int n);
	
	/// find first instance of a character from the start of a string
	CORE_ABI char* strchr(const char* s, int c);	
	
	/// find first instance of a character from the end of string.
	CORE_ABI char* strrchr(const char* s, int c);	
	
	/// duplicate the string
	CORE_ABI char* strdup(const char*);
	
	/// concatenate strings
	CORE_ABI char* strcat(char* s1, const char* s2);

	/// for all occurences of from in the string replace it with to
	/// \returns number of characters replaced.
	CORE_ABI int strrep(char* s, char from, char to);

	/// \returns true if the passed character is alphabetic (maybe lower or upper case)		
	inline bool is_alpha(char c)
	{
		return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
	}
	
	/// \returns true if the passed character is numeric
	inline bool is_numeric(char c)
	{
		return (c >= '0' && c <= '9');
	}
	
	/// \returns true if the passed character is a hex digit
	inline bool is_hex(char c)
	{
		return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'Z');
	}
	
	/// \returns true if the passed character is whitespace
	inline bool is_whitespace(char c)
	{
		return c <= ' ' && c > 0;
	}

	/// \returns Lowercase character if input is in [A, Z] otherwise the 
	///          input character
	inline char to_lower(char c)
	{
		if(c >= 'A' && c <= 'Z')
			return c - 'A' + 'a';
		return c;
	}
	
	/// \returns true if the characters match
	inline bool is_key(const char *p, char c)
	{
		if(!*p)
			return false;
		return (*p == c);
	}

	/// iterates over the string until a non whitespace character or the 
	/// null terminator is found.
	/// \param p reference to pointer to be incremented	
	CORE_ABI void eat_whitespace(const char *&p);

	/// convert ascii string to an integer.
	CORE_ABI int atoi(const char*);

	/// convert ascii string to a float.
	CORE_ABI double atof(const char*);
		
} // end namespace
} // end namespace

#endif // __STRING_H_271BD3F0_F43F_47DF_B686_2F44D31C65CA_
