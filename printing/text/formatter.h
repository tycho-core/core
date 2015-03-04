//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Tuesday, 6 October 2009 1:37:40 PM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __FORMATTER_H_19393F85_3CF9_4168_AC02_CCDA4EB5807F_
#define __FORMATTER_H_19393F85_3CF9_4168_AC02_CCDA4EB5807F_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "core/core_abi.h"
#include "core/colour/rgba.h"

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

	/// base formating class, this defines the output format.
	/// i.e.  text, html, rtf
	class formatter
	{
	public:
		virtual void newline() = 0;
		virtual void space() = 0;
		virtual void tab() = 0;
		virtual void indent() = 0;
		virtual void inc_indent() = 0;
		virtual void dec_indent() = 0;
		virtual void horizontal_bar(size_t width) = 0;
		virtual void write(const char *str, core::rgba foregroud, core::rgba background) = 0;
		virtual void writeln(const char *str, core::rgba foregroud, core::rgba background) = 0;
		virtual void write(const char *str) = 0;
		virtual void writeln(const char *str) = 0;
		virtual void format(const char*, ...) = 0;	
		virtual void set_page_width(int c) = 0;
		virtual void set_default_background_color(core::rgba clr) = 0;
		virtual void set_default_foreground_color(core::rgba clr) = 0;
	};
	

} // end namespace
} // end namespace
} // end namespace
} // end namespace

#endif // __FORMATTER_H_19393F85_3CF9_4168_AC02_CCDA4EB5807F_
