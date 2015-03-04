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
#include "core/colour/rgba.h"
#include "math/vector4.h"
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

#if 0 //TYCHO_OSX
#include <curses.h>
#else
#include <ncurses.h>
#endif

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////
namespace tycho
{
namespace core
{

	//------------------------------------------------------------------------------------

	/// console output handle
	static FILE* g_output_handle;
	static FILE* g_input_handle;
	static int   g_screen_width;

	//------------------------------------------------------------------------------------

	void console::initialise(const char *title)
	{
	    g_output_handle = stdout;
	    g_input_handle = stdin;

		// initialise ncurses
//		initscr();

		// disable line buffering on the input so we get key presses
		// immediately
//		::raw();
		
		// enable F keys, arrows and keypad
//		::keypad(stdscr, TRUE);	  

		// get width of console window
		g_screen_width = 80;// getmaxx(stdscr);

		// enable color
//		if(has_colors())
//			start_color();
	}
	
	

	//------------------------------------------------------------------------------------

	void console::shutdown()
	{
#if !TYCHO_OSX
		g_output_handle = 0;
		g_input_handle = 0;

		// shutdown ncurses
		endwin();
#else
        //PORTME
#endif
	}

	//------------------------------------------------------------------------------------

	void console::write_ln(const char *format, ...)
	{
#if !TYCHO_OSX
		va_list args;
		va_start(args, format);
		const int BufSize = 1024;
		char buf[BufSize];
		buf[0] = 0;
		int nc = vsnprintf(buf, BufSize-1, format, args);			
		buf[nc] = '\n';
		buf[nc+1] = 0;
		fprintf(g_output_handle, buf);
#else
        //PORTME
#endif
	}

	//------------------------------------------------------------------------------------

    void console::write(const char ch)
    {
        //PORTME
    }

	//------------------------------------------------------------------------------------

	void console::write(const char *format, ...)
	{
#if !TYCHO_OSX
		va_list args;
		va_start(args, format);
		const int BufSize = 1024;
		char buf[BufSize];
		buf[0] = 0;
		int nc = vsnprintf(buf, BufSize, format, args);	
		fprintf(g_output_handle, buf);
#else
        //PORTME
#endif
	}
	
	//------------------------------------------------------------------------------------

	void console::wait_until_keypress()
	{
#if !TYCHO_OSX
		getch();
#else
        //PORTME
#endif
	}

	//------------------------------------------------------------------------------------


	void console::set_colors(const core::rgba& background, const core::rgba& foreground)
	{
		//math::vector4i bg = (to_vector4<float>(background) * 1000).cast_to<int>();
		//math::vector4i fg = (to_vector4<float>(foreground) * 1000).cast_to<int>();

		// setup the foreground
	}
		
	//------------------------------------------------------------------------------------

	void console::restore_default_colors()
	{
//		use_default_colors();
	}

	//------------------------------------------------------------------------------------

	int console::get_width()
	{
	    return g_screen_width;
	}

	//------------------------------------------------------------------------------------

} // end namespace
} // end namespace
