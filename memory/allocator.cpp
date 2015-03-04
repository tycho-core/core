//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Monday, 24 March 2008 4:45:08 AM
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "core/debug/assert.h"
#include "core/memory/allocator.h"
#include "core/memory/allocator_layer.h"
#include "core/memory/dl_allocator.h"
#include "core/memory/ansi_allocator.h"
#include "core/memory/debug_allocator.h"
#include "core/program_config.h"
#include "core/globals.h"
#include "core/string.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////
#if TYCHO_PC
// force initialisation early in startup so we can set our atexit handler 
// so it gets called after all other static initialisation.
#pragma warning(disable : 4073) // initializers put in library initialization area.
#pragma init_seg(lib)
#define GCC_INIT_PRIORITY(_n)
#elif TYCHO_LINUX
#define GCC_INIT_PRIORITY(_n) __attribute__ ((init_priority(_n)))
#endif

namespace tycho
{
namespace core
{

	//------------------------------------------------------------------------------------

	struct alloc_ptrs
	{
		alloc_ptrs() : 
			alloc(0), debugger(0) 
		   {}

		allocator_layer* alloc;
		allocator_layer* debugger;
	};

	// on GCC make sure all are allocated before everything else and in the correct order
	static alloc_ptrs		g_alloc			GCC_INIT_PRIORITY(113);
	static ansi_allocator	ansiallocator	GCC_INIT_PRIORITY(114);
	static dl_allocator		dlalloc			GCC_INIT_PRIORITY(114);
	static debug_allocator	debug_alloc		GCC_INIT_PRIORITY(115);

	//------------------------------------------------------------------------------------
	
	static void atexit_handler()
	{
		debug_alloc.print_allocated();		
	}

	//------------------------------------------------------------------------------------


#if TYCHO_PC
	int register_atexit_handler()
	{
		atexit(atexit_handler);
		return (int)&atexit_handler;
	}

	static int foo = register_atexit_handler();
#elif TYCHO_LINUX
	struct sentinal
	{
		sentinal()
		{
		}	
		~sentinal()
		{
			atexit_handler();
		}
	};
	static sentinal g_sentinal GCC_INIT_PRIORITY(116);
#endif
	
	//------------------------------------------------------------------------------------
	
	void* allocator::malloc(core::uint32 size, int flags)
	{
		initialise();
		if(g_alloc.debugger)
			return g_alloc.debugger->malloc(size, flags, g_alloc.alloc);
			
		return g_alloc.alloc->malloc(size, flags, 0);
	}
	
	//------------------------------------------------------------------------------------

	void* allocator::malloc_aligned(core::uint32 size, core::uint32 alignment, int flags) 
	{
		initialise();
		if(g_alloc.debugger)
			return g_alloc.debugger->malloc_aligned(size, alignment, flags, g_alloc.alloc);

		return g_alloc.alloc->malloc_aligned(size, alignment, flags, 0);		
	}

	//------------------------------------------------------------------------------------

	void* allocator::realloc(void* memory, size_t newsize, int flags)
	{
		initialise();
		if(g_alloc.debugger)
			return g_alloc.debugger->realloc(memory, newsize, flags, g_alloc.alloc);

		return g_alloc.alloc->realloc(memory, newsize, flags, 0);
	}
	
	//------------------------------------------------------------------------------------

	void allocator::free(void *ptr, int flags)
	{
		if(ptr == 0)
			return;
			
		TYCHO_ASSERT(g_alloc.alloc);
		if(g_alloc.debugger)
		    g_alloc.debugger->free(ptr, flags, g_alloc.alloc);
		else
			g_alloc.alloc->free(ptr, flags, 0);			
	}

	//------------------------------------------------------------------------------------

	size_t allocator::get_allocation_size(void* memory)
	{
		if(g_alloc.debugger)
			return g_alloc.debugger->get_allocation_size(memory, g_alloc.alloc);

		return g_alloc.alloc->get_allocation_size(memory, 0);
	}
	
	//------------------------------------------------------------------------------------

	void allocator::set_allocator(allocator_layer *l)
	{
		g_alloc.alloc = l;
	}
	
	//------------------------------------------------------------------------------------

	void allocator::set_debugger(allocator_layer *l)
	{
		g_alloc.debugger = l;
	}

	//------------------------------------------------------------------------------------

	void allocator::initialise()
	{
		if(g_alloc.alloc == 0)
		{
			typedef program_config pc;
			set_allocator(&ansiallocator);

			// get config options
			bool debug=false, check_leaks=false, check_overruns=false, fill_new=false, fill_freed=false;
			const char *buf_allocator = 0;
			static pc::option_pair options[] = {
				pc::opt("debug", 0, "Enable memory debugging support", "1", &debug),
				pc::opt("check_leaks", 0, "Enable memory leak detection", "1", &check_leaks),
				pc::opt("check_overruns", 0, "Enable allocation over and under run detection", "1", &check_overruns),			
				pc::opt("fill_new",   0, "Fill all new allocations with a fixed pattern", "1", &fill_new),	
				pc::opt("fill_freed", 0, "Fill all freed allocations with a fixed pattern", "1", &fill_freed),
				pc::opt("allocator", 0, "Name of memory allocator to use", "ansi", &buf_allocator),
				pc::null_opt()
			};						
			globals::get_config()->process_option_group("mem", "Memory debugging support", options);				

			int debug_flags = debug_allocator::f_none;
			if(check_leaks)
				debug_flags |= debug_allocator::f_memleaks;
			if(check_overruns)
				debug_flags |= debug_allocator::f_overrun_detect;
			if(fill_freed)
				debug_flags |= debug_allocator::f_fill_freed;
			if(fill_new)
				debug_flags |= debug_allocator::f_fill_allocated;
								
			if (core::stricmp(buf_allocator, "douglea") == 0)
			{
				set_allocator(&dlalloc);
			}
			else
			{
				// warn if this default behaviour was unintended
				TYCHO_ASSERT(core::stricmp(buf_allocator, "ansi") == 0);
				set_allocator(&ansiallocator);
			}

#if TYCHO_DEBUG
			if(debug)
			{					
				debug_alloc.set_features(debug_flags);
				set_debugger(&debug_alloc);
			}			
#endif // TYCHO_DEBUG		
		}
	}

	//------------------------------------------------------------------------------------

} // end namespace
} // end namespace
