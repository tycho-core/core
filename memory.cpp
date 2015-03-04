//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Tuesday, 25 March 2008 5:54:01 PM
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "memory.h"


//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////
namespace tycho
{
namespace core
{

	//------------------------------------------------------------------------------------

	CORE_ABI void* mem_set32(void *ptr, int pattern, size_t len)
	{			
		// write 32 bits at a time
		int *ip = reinterpret_cast<int*>(ptr); 
		int whole = (len & ~3) >> 2;
		int remainder = len & 3;
		for(int i = 0; i < whole; ++i)
		{
			*ip++ = pattern;
		}		
		const char *src = (char*)&pattern;	
		char *dst = (char*)ip;
		switch(remainder)
		{
			case 3 : *dst++ = *src++;
			case 2 : *dst++ = *src++;
			case 1 : *dst++ = *src++;
		}
		return ptr;
	}

	//------------------------------------------------------------------------------------

	CORE_ABI void* mem_set(void *s, int c, size_t n)
	{
		unsigned char * p = (unsigned char *)s;
 	    while(n--)
 	        *p++ = (unsigned char)c;
 	    return s;
	}

	//------------------------------------------------------------------------------------

	CORE_ABI void* mem_cpy(void *s1, const void *s2, size_t n)
	{
	    char * dest = (char *)s1;
 	    const char * src = (const char *)s2;
 	    while(n--)
 	        *dest++ = *src++;
 	    return s1;
	}		
	
	//------------------------------------------------------------------------------------

	CORE_ABI void* mem_zero(void *p, size_t len)
	{
		return core::mem_set(p, 0L, len);
	}

	//------------------------------------------------------------------------------------

   	CORE_ABI int mem_cmp(const void *p1, const void *p2, size_t len)
	{
	    const unsigned char *us1 = (const unsigned char *)p1;
	    const unsigned char *us2 = (const unsigned char *)p2;
	    while(len-- != 0) 
	    {
			if(*us1 != *us2)
			    return (*us1 < *us2) ? -1 : +1;
			us1++;
			us2++;
	    }
	    return 0;
	}	

	//------------------------------------------------------------------------------------
	
} // end namespace
} // end namespace
