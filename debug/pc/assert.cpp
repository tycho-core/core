//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Sunday, 30 March 2008 10:38:54 AM
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "core/debug/assert.h"
#include "core/pc/safe_windows.h"
#include "core/program_config.h"
#include "core/globals.h"
#include "core/string.h"
#include <cwchar>
#include <cstdlib>


//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////
namespace tycho
{
namespace core
{
	//------------------------------------------------------------------------------------

	namespace options
	{
		bool log_only;
	}

	//------------------------------------------------------------------------------------

	//\todo should be platform independent
	void initialise_assert()
	{
		static bool initialised = false;
		if(!initialised)
		{
			typedef program_config pc;
			// get config options
			static pc::option_pair options[] = {
				pc::opt("log_only", 0, "log the assert but do not display the message box", "0", &options::log_only),
				pc::null_opt()
			};						
			globals::get_config()->process_option_group("assert", "Assert behaviour", options);				
			initialised = true;
		}
	}

	//------------------------------------------------------------------------------------

	void default_assert(const char *expr_txt, const char* msg_txt, const char *file, unsigned line)
	{
		initialise_assert();
		
		const int BufLen = 1024;
		char title_buffer[BufLen] = "Assert Failed";
		char msg_buffer[BufLen] = { 0 };

		if(msg_txt)
		{
			core::snprintf(msg_buffer, BufLen, "%s\n%s\n%s(%u)", msg_txt, expr_txt, file, line);
		}
		else
		{
			core::snprintf(msg_buffer, BufLen, "%s\n%s(%u)", expr_txt, file, line);
		}
		
		if(!options::log_only)
		{
			int res = ::MessageBoxA(NULL, msg_buffer, title_buffer, MB_ICONSTOP | MB_ABORTRETRYIGNORE | MB_SETFOREGROUND | MB_TOPMOST);

			if(res == IDABORT)
				::abort();
			else if(res == IDIGNORE)
				return;
			else if(res == IDRETRY)
				::DebugBreak();		
		}
	}
	
	//------------------------------------------------------------------------------------	

	void dopanic(const char* msg_txt, const char *file, unsigned line)
	{
		const int BufLen = 1024;
		char msg_buffer[BufLen] = { 0 };
		if(msg_txt)
		{
			core::snprintf(msg_buffer, BufLen, "%s\n%s(%u)", msg_txt, file, line);
		}
		else
		{
			core::snprintf(msg_buffer, BufLen, "%s(%u)", file, line);
		}

		if(!options::log_only)
		{
			int res = ::MessageBoxA(NULL, msg_buffer, "Tycho Panic!", MB_ICONSTOP | MB_OK | MB_SETFOREGROUND | MB_TOPMOST);

			if(res == IDABORT)
				::abort();	
		}
		else
		{	
			::abort();
		}
	}

	//------------------------------------------------------------------------------------

} // end namespace
} // end namespace
