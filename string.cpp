//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Tuesday, 25 March 2008 6:09:57 PM
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "string.h"
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////
namespace tycho
{
namespace core
{

	//------------------------------------------------------------------------------------

	CORE_ABI char* strncpy(char *dst, const char *src, int buffer_len)
	{
		// only copy until all of src has been or we run out of room.
		for(;*src && buffer_len; --buffer_len)
		{
			*dst++ = *src++;
		}
		
		*dst = 0;
		return dst;
	}
	
	//------------------------------------------------------------------------------------

	CORE_ABI char* strcpy(char *dst, const char *src)
	{
		while(*src)
			*dst++ = *src++;
		*dst = 0;
		return dst;
	}
	
	
	//------------------------------------------------------------------------------------

	CORE_ABI int strlen(const char *s)
	{
	    size_t rc = 0;
 	    while(s[rc])
 	        ++rc;
 	    return rc;
	}
	
	//------------------------------------------------------------------------------------

	CORE_ABI int strcmp(const char* s1, const char* s2)
	{	
		while(*s1 && (*s1 == *s2))
		{
			++s1;
			++s2;
		}
		return (*s1 - *s2);
	}

	//------------------------------------------------------------------------------------

	CORE_ABI int stricmp(const char* s1, const char* s2)
	{
		while(*s1 && (to_lower(*s1) == to_lower(*s2)))
		{
			++s1;
			++s2;
		}
		return (*s1 - *s2);
	}
	
	//------------------------------------------------------------------------------------

	CORE_ABI int strncmp(const char* s1, const char* s2, int n)
	{
 	    while(n && (*s1 == *s2))
 	    {
 	        ++s1;
 	        ++s2;
 	        --n;
 	    }
	    if(n == 0)
 	        return 0;
		return (*s1 - *s2);
	}

	//------------------------------------------------------------------------------------

	CORE_ABI char* strchr(const char* s, int c)
	{
		do
		{
			if(*s == (char)c)
				return (char *)s;
		} while(*s++);
		return 0;	
	}

	//------------------------------------------------------------------------------------

	CORE_ABI char* strrchr(const char* s, int c)
	{
		int i = 0;
		while(s[i++]);
		do
		{
			if(s[--i] == (char)c)
				return (char *)s + i;
		} while(i);
		return 0;	
	}

	//------------------------------------------------------------------------------------

	CORE_ABI char* strdup(const char* str)
	{
		if(!str)
			return 0;
		int len = core::strlen(str);
		char* n = (char*)core::malloc(len+1);
		core::strcpy(n, str);
		return n;
	}

	//------------------------------------------------------------------------------------

	CORE_ABI void eat_whitespace(const char *&p)
	{
		if(!*p)
			return;
		while(*p && core::is_whitespace(*p))
			++p;
	}
	
	//------------------------------------------------------------------------------------	

    CORE_ABI char* strcat(char* s1, const char* s2)
    {
		char *s = s1;
		while (*s != '\0')
			s++;

		strcpy(s, s2);
		return s1;
    }

	//------------------------------------------------------------------------------------	

	CORE_ABI int strrep(char* s, char from, char to)
	{
		if(!s)
			return 0;
		int c = 0;
		while(*s != '\0')
		{
			if(*s == from)
			{
				*s = to;
				++c;
			}
			++s;
		}
		return c;
	}

	//------------------------------------------------------------------------------------	

	CORE_ABI int atoi(const char* s)
	{
		return ::atoi(s);
	}

	//------------------------------------------------------------------------------------	

	CORE_ABI double atof(const char* s)
	{
		return ::atof(s);
	}

	//------------------------------------------------------------------------------------	

} // end namespace
} // end namespace
