//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Saturday, 29 March 2008 7:50:55 PM
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "core/process.h"
#include "core/string.h"

extern "C" 
{
	// defined by libc
    int __libc_argc;
    char** __libc_argv;
}

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////
namespace tycho
{
namespace core
{
namespace process
{
		
	//------------------------------------------------------------------------------------
		
	CORE_ABI const char* command_line()
	{
	    static bool init = true;
	    static char str[1024] = { 0 };
	    if(init)
	    {
			for(int i = 1; i < __libc_argc; ++i)
			{
				core::strcat(str, __libc_argv[i]);
				core::strcat(str, " ");
			}

			init = false;
	    }
	    return str;
	}

	//------------------------------------------------------------------------------------

} // end namespace
} // end namespace
} // end namespace
