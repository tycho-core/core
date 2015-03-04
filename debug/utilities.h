//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Monday, 24 March 2008 7:52:52 PM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __UTILTIES_H_EA48BED2_373B_49D3_AD4F_B75B6D0B6624_
#define __UTILTIES_H_EA48BED2_373B_49D3_AD4F_B75B6D0B6624_

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
namespace debug
{

	const int SymbolStringMaxLength = 256;

	/// generate a stack trace.
	/// \returns number of addresses in trace
	CORE_ABI int stacktrace(core::ptr_type *trace, size_t max_depth);
	
	/// convert a frame address to function name
	CORE_ABI bool convert_address_to_function(core::ptr_type address, char *name_buf, char *file_buf, int *line);
	
	/// write a line to debug output
	CORE_ABI void write_ln(const char *ln, ...);

	/// write a string to the debug output
	CORE_ABI void write(const char *ln, ...);
	
} // end namespace
} // end namespace
} // end namespace

#endif // __UTILTIES_H_EA48BED2_373B_49D3_AD4F_B75B6D0B6624_
