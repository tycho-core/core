//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Thursday, 10 April 2008 2:36:28 AM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __CONSOLE_H_32EBCD9B_2CD1_453C_BF83_044AFA51E273_
#define __CONSOLE_H_32EBCD9B_2CD1_453C_BF83_044AFA51E273_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "core/core_abi.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace tycho
{
namespace core
{
	
	/// output console
    class CORE_ABI console
    {
    public:
		/// create a console output window
		static void initialise(const char *title);
		
		/// cleanup the console output window
		static void shutdown();

		/// write a single character to console output
		static void write(const char ch);
		
		/// write a line to console output
		static void write_ln(const char *ln, ...);

		/// write a string to the console  output
		static void write(const char *ln, ...);
        
		/// wait until the user presses a key
		static void wait_until_keypress();

		/// set the console text colours
		static void set_colors(const core::rgba& foreground, const core::rgba& background);
		
		/// restore default foreground and background colours
		static void restore_default_colors();

		/// \returns the width of the console
		static int get_width();
    };

} // end namespace
} // end namespace

#endif // __CONSOLE_H_32EBCD9B_2CD1_453C_BF83_044AFA51E273_
