//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Saturday, 29 March 2008 7:39:33 PM
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "core/globals.h"
#include "core/program_config.h"
#include "core/process.h"

#if TYCHO_PC
// force initialisation early in startup so we can set our atexit handler 
// so it gets called after all other static initialisation.
//#pragma warning(disable : 4073) // initializers put in library initialization area.
//#pragma init_seg(lib)
#include "core/debug/pc/debuggee.h"
#define GCC_INIT_PRIORITY(_n)
#elif TYCHO_LINUX
#define GCC_INIT_PRIORITY(_n) __attribute__ ((init_priority(_n)))
#endif


//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////
namespace tycho {
namespace core  {
namespace globals {

	//------------------------------------------------------------------------------------

	static program_config g_config GCC_INIT_PRIORITY(113);
//	static io::interface  g_io_interface;

#if TYCHO_PC
	static pc::debuggee	  g_debuggee;
#elif TYCHO_GC
	static gc::debuggee	  g_debuggee;
#endif

	//------------------------------------------------------------------------------------
	
	CORE_ABI void initialise()
	{
		static bool initialised = false;
		if(!initialised)
		{					
			g_config.parse_command_line(process::command_line());
			initialised = true;
		}
	}			

	//------------------------------------------------------------------------------------

	CORE_ABI void shutdown()
	{
//		g_io_interface.clear();
#if!TYCHO_LINUX
		g_debuggee.disconnect();
#endif 
	}
	
	//------------------------------------------------------------------------------------

	CORE_ABI tycho::core::program_config* get_config()
	{
		initialise();
		return &g_config;
	}

	//------------------------------------------------------------------------------------

	CORE_ABI tycho::core::debuggee* get_debuggee()
	{
#if TYCHO_LINUX
	    return 0;
#else
	    initialise();
		return &g_debuggee;
#endif
	}

	//------------------------------------------------------------------------------------
	
//	CORE_ABI io::interface& get_io()
//	{
//		initialise();
//		return g_io_interface;
//	}

	//------------------------------------------------------------------------------------

} // end namespace
} // end namespace
} // end namespace
