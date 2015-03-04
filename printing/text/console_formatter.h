//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Tuesday, 6 October 2009 1:37:45 PM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __CONSOLE_FORMATTER_H_031959BA_E5AE_4724_B1C3_3A2CA4D9A2A5_
#define __CONSOLE_FORMATTER_H_031959BA_E5AE_4724_B1C3_3A2CA4D9A2A5_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "core/core_abi.h"
#include "core/printing/text/formatter.h"

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

	// colour console printer
	class CORE_ABI console_formatter : public formatter
	{
	public:
		console_formatter();
		~console_formatter();
				
		virtual void inc_indent() { m_indent_level++; }
		virtual void dec_indent() { m_indent_level--; }
		virtual void newline();
		virtual void space();
		virtual void tab();
		virtual void indent();
		virtual void horizontal_bar(size_t width);		
		virtual void write(const char *str, core::rgba foregroud, core::rgba background);
		virtual void writeln(const char *str, core::rgba foregroud, core::rgba background);
		virtual void write(const char *str);
		virtual void writeln(const char *str);	
		virtual void format(const char *str, ...);
		virtual void set_page_width(int c);
		virtual void set_tab_size(size_t s) { m_tab_size = s; }
		virtual void set_default_background_color(core::rgba clr) { m_default_background = clr; }
		virtual void set_default_foreground_color(core::rgba clr) { m_default_foreground = clr; }
		
	private:
		void operator=(const console_formatter&); // undefined
		
		void put_char(char a);
		
		void move_cursor(int n) 
		{
			m_cursor_pos += n;
			if(m_cursor_pos >= m_console_width)
				m_cursor_pos -= m_console_width;
		}
		
		void set_color(core::rgba fore, core::rgba back);
		
	private:		
		void *m_console; // HANDLE
		
		/// real width of the console
		int m_console_width; 
	
		/// width of console for formatting
		int m_width;
			
		/// current cursor position on the line
		int m_cursor_pos;
	
		int m_original_clrs;
		core::rgba m_default_foreground;
		core::rgba m_default_background;
		size_t m_tab_size;
		size_t m_indent_level;
	};

} // end namespace
} // end namespace
} // end namespace
} // end namespace

#endif // __CONSOLE_FORMATTER_H_031959BA_E5AE_4724_B1C3_3A2CA4D9A2A5_
