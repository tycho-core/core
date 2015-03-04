//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Tuesday, 25 March 2008 4:59:03 PM
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "symbols.h"
#include "core/memory.h"
#include "core/string.h"
#include "core/pc/safe_windows.h"
#include <DbgHelp.h>

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////
namespace tycho
{
namespace core
{
namespace pc
{
namespace symbols
{

	//------------------------------------------------------------------------------------

	bool initialise()
	{
	    ::SymSetOptions(SYMOPT_LOAD_LINES | SYMOPT_DEFERRED_LOADS | SYMOPT_UNDNAME);
		if(!::SymInitialize(::GetCurrentProcess(), 0, TRUE)) 
			return false;
		return true;				
	}
	
	//------------------------------------------------------------------------------------

	bool shutdown()		
	{
		::SymCleanup(::GetCurrentProcess());
		return true;
	}

	//------------------------------------------------------------------------------------
	
	bool lookup_address(core::ptr_type address,
								 char *name_buf,
								 char *file_buf,
								 int *line)
	{
		IMAGEHLP_LINE64 source_info;
	    SYMBOL_INFO *function_info;
		const int MaxSymbolNameLength = 256;
		const int SymbolBufferSize = sizeof(SYMBOL_INFO) + (MaxSymbolNameLength * sizeof(TCHAR)) - 1;
		unsigned char sym_buf[SymbolBufferSize];
		DWORD	displacement = 0;
		DWORD64 displacement64 = 0;
		HANDLE process = ::GetCurrentProcess();

		// initialise symbol buffer
		function_info = reinterpret_cast<SYMBOL_INFO*>(sym_buf);
		core::mem_zero(function_info, SymbolBufferSize);
		function_info->SizeOfStruct = sizeof(SYMBOL_INFO);
		function_info->MaxNameLen = MaxSymbolNameLength;
		core::mem_zero(source_info);
		source_info.SizeOfStruct = sizeof(IMAGEHLP_LINE64);

		// lookup file and line info for this address, but only if user wants the information
        if((file_buf || line) && ::SymGetLineFromAddr64(process, (DWORD64)address, &displacement, &source_info)) 
        {
			if(file_buf && source_info.FileName != 0)
			{
				core::mem_zero(file_buf, core::debug::SymbolStringMaxLength);
				core::strncpy(file_buf, source_info.FileName, core::debug::SymbolStringMaxLength);
			}
			
			if(line)
				*line = static_cast<int>(source_info.LineNumber);
        }

		// lookup function 
		if(name_buf && ::SymFromAddr(process, (DWORD64)address, &displacement64, function_info))
		{
			if(function_info->Name[0] != 0)
			{
				core::mem_zero(name_buf, core::debug::SymbolStringMaxLength);					
				core::strncpy(name_buf, function_info->Name, core::debug::SymbolStringMaxLength);
			}
		}
		
		return true;
	}

	//------------------------------------------------------------------------------------

} // end namespace
} // end namespace
} // end namespace
} // end namespace
