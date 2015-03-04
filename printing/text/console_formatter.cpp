//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Tuesday, 6 October 2009 1:37:45 PM
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "console_formatter.h"
#include "core/debug/utilities.h"
#include "core/console.h"
#include "math/utilities.h"
#include <stdarg.h> 

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////
namespace tycho
{
namespace core
{
namespace printing
{
namespace text
{

    //--------------------------------------------------------------------


	console_formatter::console_formatter() :
		m_cursor_pos(0),
		m_tab_size(4),
		m_indent_level(0)
	{
		m_width = m_console_width = core::console::get_width();
	}

    //--------------------------------------------------------------------

	console_formatter::~console_formatter()
	{
		core::console::restore_default_colors();
	}

    //--------------------------------------------------------------------

	void console_formatter::newline() 
	{
		set_color(m_default_foreground, m_default_background);
		int width = m_width - m_cursor_pos;
		TYCHO_ASSERT(width);
		for(int i = 0; i < width; ++i)
			space();		
			
		if(m_console_width != m_width)
			put_char('\n');
		core::debug::write("\n");
	}
	
    //--------------------------------------------------------------------

	void console_formatter::horizontal_bar(size_t width)
	{
		if(width == 0)
		{
			width = m_width - m_cursor_pos;
		}
		
		for(size_t i = 0; i < width; ++i)
			put_char('-');	
			
		if(m_cursor_pos != 0)
			newline();
	}	
	
    //--------------------------------------------------------------------

	void console_formatter::put_char(char a)
	{
		core::console::write(a);
		char od[2] = { 0, 0 };
		od[0] = a;
		core::debug::write(od);
		if(a == '\n')
			m_cursor_pos = 0;
		else
			move_cursor(1);
		if(m_cursor_pos == 0)
			indent();
	}	
	
    //--------------------------------------------------------------------

	void console_formatter::space() 
	{
		put_char(' ');
	}

    //--------------------------------------------------------------------

	void console_formatter::tab()
	{
		for(size_t i = 0; i < m_tab_size; ++i)
			space();
	}
	
    //--------------------------------------------------------------------

	void console_formatter::indent()
	{
		for(size_t i = 0; i < m_indent_level; ++i)
			tab();
	}
		
    //--------------------------------------------------------------------

	void console_formatter::write(const char *str, core::rgba foreground, core::rgba background) 
	{
		set_color(foreground, background);
		
		const char *ch = str;
		while(*ch)
		{
			if(*ch == '\t')
				tab();
			else if(*ch == '\n')
				newline();
			else
			{
				put_char(*ch);
			}
			
			++ch;
		}		
	}
	
    //--------------------------------------------------------------------

	void console_formatter::writeln(const char *str, core::rgba foregroud, core::rgba background)
	{
		write(str, foregroud, background);
		newline();
	}
	
    //--------------------------------------------------------------------

	void console_formatter::write(const char *str)
	{
		write(str, m_default_foreground, m_default_background);	
	}
	
    //--------------------------------------------------------------------

	void console_formatter::writeln(const char *str)
	{
		write(str);
		newline();	
	}

    //--------------------------------------------------------------------

    void console_formatter::format(const char *str, ...)
    {
		const int BufLen = 2048;
        char buffer[BufLen] = { 0 };
		va_list args;
		va_start(args, str);
#if TYCHO_PC
		_vsnprintf(buffer, BufLen, str, args);
#else
		vsnprintf(buffer, BufLen, str, args);
#endif // TYCHO_PC
		write(buffer);
    }

    //--------------------------------------------------------------------

	void console_formatter::set_color(core::rgba foreground, core::rgba background)
	{
		console::set_colors(foreground, background);
	}

	//--------------------------------------------------------------------

	void console_formatter::set_page_width(int c)
	{
		if(c == 0)
			m_width = m_console_width;
		else
			m_width = math::min(c, m_console_width);	
	}

    //--------------------------------------------------------------------

} // end namespace
} // end namespace
} // end namespace
} // end namespace
