//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Thursday, 10 April 2008 2:40:27 AM
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "core/console.h"
#include "core/debug/assert.h"
#include "core/pc/safe_windows.h"
#include "core/colour/rgba.h"
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <WinCon.h>
#include <algorithm>

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////
namespace tycho
{
namespace core
{

	//------------------------------------------------------------------------------------

	static HANDLE g_output_handle;
	static HANDLE g_input_handle;
	static WORD   g_original_console_colors;
	
	//------------------------------------------------------------------------------------

	void console::initialise(const char *title)
	{
		// if we are a console mode app then AllocConsole will fail, if window app then
		// will create one, so ignore return.
		::AllocConsole();
		TYCHO_WIN_CHECK_BOOL(::SetConsoleTitle(title));
		g_output_handle = ::GetStdHandle(STD_OUTPUT_HANDLE);				
		g_input_handle = ::GetStdHandle(STD_INPUT_HANDLE);				
		TYCHO_WIN_CHECK_BOOL(::SetConsoleMode(g_input_handle, ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT));		
		::CONSOLE_SCREEN_BUFFER_INFO con_info;
		TYCHO_WIN_CHECK_BOOL(::GetConsoleScreenBufferInfo(g_output_handle, &con_info));    
	    g_original_console_colors = con_info.wAttributes;	
	}

	//------------------------------------------------------------------------------------

	void console::shutdown()
	{
		restore_default_colors();
		TYCHO_WIN_CHECK_BOOL(::FreeConsole());
		g_output_handle = INVALID_HANDLE_VALUE;
		g_input_handle  = INVALID_HANDLE_VALUE;
		g_original_console_colors = 0;
	}

	//------------------------------------------------------------------------------------

	void console::write(const char ch)
	{
		TYCHO_WIN_CHECK_BOOL(::WriteFile(g_output_handle, &ch, 1, NULL, NULL));
	}

	//------------------------------------------------------------------------------------

	void console::write_ln(const char *format, ...)
	{
		va_list args;
		va_start(args, format);
		const int BufSize = 1024;
		char buf[BufSize];
		buf[0] = 0;
		int nc = _vsnprintf(buf, BufSize-1, format, args);			
		buf[nc] = '\n';
		buf[nc+1] = 0;
		TYCHO_WIN_CHECK_BOOL(::WriteFile(g_output_handle, buf, nc+1, NULL, NULL));
		va_end(args);
	}

	//------------------------------------------------------------------------------------

	void console::write(const char *format, ...)
	{
		va_list args;
		va_start(args, format);
		const int BufSize = 1024;
		char buf[BufSize];
		buf[0] = 0;
		int nc = _vsnprintf(buf, BufSize, format, args);	
		TYCHO_WIN_CHECK_BOOL(::WriteFile(g_output_handle, buf, nc, NULL, NULL));
	}
	
	//------------------------------------------------------------------------------------

	void console::wait_until_keypress()
	{
		_getch();
	}

	//------------------------------------------------------------------------------------

	void console::set_colors(const core::rgba& background, const core::rgba& foreground)
	{
		int clr = 0;
		if(foreground.r()) clr |= FOREGROUND_RED;
		if(foreground.g()) clr |= FOREGROUND_GREEN;
		if(foreground.b()) clr |= FOREGROUND_BLUE;
		
		if(background.r()) clr |= BACKGROUND_RED;
		if(background.g()) clr |= BACKGROUND_GREEN;
		if(background.b()) clr |= BACKGROUND_BLUE;
			
		clr |= FOREGROUND_INTENSITY;
		
		TYCHO_WIN_CHECK_BOOL(::SetConsoleTextAttribute(g_output_handle, static_cast<WORD>(clr)));
	}
		
	//------------------------------------------------------------------------------------

	void console::restore_default_colors()
	{
		if(g_output_handle != INVALID_HANDLE_VALUE)
			TYCHO_WIN_CHECK_BOOL(::SetConsoleTextAttribute(g_output_handle, static_cast<WORD>(g_original_console_colors)));
	}

	//------------------------------------------------------------------------------------

	int console::get_width()
	{
		::CONSOLE_SCREEN_BUFFER_INFO con_info;
		TYCHO_WIN_CHECK_BOOL(::GetConsoleScreenBufferInfo(g_output_handle, &con_info));    
	    return (int)con_info.dwSize.X;
	}

	//------------------------------------------------------------------------------------

} // end namespace
} // end namespace
