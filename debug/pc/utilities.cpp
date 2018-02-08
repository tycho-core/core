//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Tuesday, 25 March 2008 4:27:57 PM
//////////////////////////////////////////////////////////////////////////////

#if TYCHO_PC == 0
#error "PC only file"
#endif 

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "../utilities.h"
#include "core/pc/safe_windows.h"
#include "core/debug/assert.h"
#include "symbols.h"
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

//////////////////////////////////////////////////////////////////////////////
// DEFINES
//////////////////////////////////////////////////////////////////////////////
// Frame pointer omission (FPO) optimization should be turned off for this
// entire file. The release libs don't include FPO debug information, so FPO
// optimization will interfere with stack walking.
#pragma optimize ("y", off)
 
// Architecture-specific definitions for x86 and x64
#if defined(_M_IX86)
#define SIZEOFPTR 4
#define ARCHITECTURE IMAGE_FILE_MACHINE_I386
#define AXREG eax
#define BPREG ebp
#elif defined(_M_X64)
#define SIZEOFPTR 8
#define ARCHITECTURE IMAGE_FILE_MACHINE_AMD64
#define AXREG rax
#define BPREG rbp
#endif // _M_IX86
#ifdef _WIN64
#define ADDRESSFORMAT      "0x%.16X" // Format string for 64-bit addresses
#else
#define ADDRESSFORMAT      "0x%.8X"  // Format string for 32-bit addresses
#endif // _WIN64 

#pragma auto_inline(off)
static DWORD_PTR get_program_counter()
{
//TODO: FIXME
#ifdef _WIN64
	return 0;
#else
	DWORD_PTR pc;
	__asm mov AXREG, [BPREG + SIZEOFPTR] // Get the return address out of the current stack frame
	__asm mov [pc], AXREG				 // Put the return address into the variable we'll return
	return pc;
#endif
}
#pragma auto_inline(on)

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
				
			tycho::core::pc::symbols::initialise();
				
			g_initialised = true;
		}
	
	} // end namespace
	
	//------------------------------------------------------------------------------------

#if _WIN64
	CORE_ABI int stacktrace(core::ptr_type *trace, size_t max_depth)
	{
		return 0;
	}
#else
	/// generate a stack trace
	/// \warning For this function to work correctly with optimisations enabled you need to disable the 
	///          framer pointer omission optimisation using /Oy-
	CORE_ABI int stacktrace(core::ptr_type *trace, size_t max_depth)
	{	
		detail::initialise();
		const int num_frames_to_skip = 1; // skip over our own address, user doesn't want that.
		unsigned int ip = get_program_counter();
		unsigned int *frame, *prevFrame;
		__asm mov frame, BPREG
		unsigned long count = (unsigned long)max_depth; // rely on a fact that negative depth will give huge count to trace			
		if(max_depth >= 0)
			count += num_frames_to_skip;
		unsigned long index = 0;
		for (;count;--count,++index)
		{
			if(index == max_depth)
				break;
				
			// eliminate two levels of the stack
			if(index >= num_frames_to_skip)
				trace[index-num_frames_to_skip] = (ptr_type)ip;

			// next ip
			ip = frame[1];

			// zero is no go
			if(!ip) 
				break;

			prevFrame = frame;
			frame = (unsigned int *)frame[0];

			// stop if frame not aligned (an invalid frame)
			if((unsigned int)frame & 3) break;

			// stop if we didn't go any further
			if(frame <= prevFrame) break;

			// stop if two dwords can't be read (an invalid frame)
			if(IsBadReadPtr(frame, sizeof(unsigned int)*2)) break;
		}		
		
		return index;
	}
#endif // _WIN64

	//------------------------------------------------------------------------------------

	CORE_ABI bool convert_address_to_function(core::ptr_type address, char *name_buf, char *file_buf, int *line)
	{
		detail::initialise();
		return pc::symbols::lookup_address(address, name_buf, file_buf, line);
	}

	//------------------------------------------------------------------------------------

	CORE_ABI void write_ln(const char *format, ...)
	{
		va_list args;
		va_start(args, format);
		const int BufSize = 4096;
		char buf[BufSize];
		buf[0] = 0;
		int nc = _vsnprintf(buf, BufSize-1, format, args);			
		TYCHO_ASSERT(nc > 0);
		if(nc > 0)
		{
			buf[nc] = '\n';
			buf[nc+1] = 0;
			::OutputDebugStringA(buf);
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
		_vsnprintf(buf, BufSize, format, args);
		int nc = _vsnprintf(buf, BufSize-1, format, args);			
		TYCHO_ASSERT(nc > 0);
		if(nc > 0)
		{
			buf[nc] = ' ';
			buf[nc+1] = 0;
			::OutputDebugStringA(buf);
		}
	}

	//------------------------------------------------------------------------------------
	
} // end namespace
} // end namespace
} // end namespace
