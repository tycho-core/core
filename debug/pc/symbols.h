//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Tuesday, 25 March 2008 4:59:03 PM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __SYMBOLS_H_CE530013_8589_44CA_9AC8_14A82A04B0DF_
#define __SYMBOLS_H_CE530013_8589_44CA_9AC8_14A82A04B0DF_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "core/core_abi.h"
#include "core/debug/utilities.h"

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

	/// initialise symbol subsystems.
	CORE_ABI bool initialise();
	
	/// shutdown symbol subsystems
	CORE_ABI bool shutdown();
	
	/// Lookup a function name including file name and line number from an address.
	/// All buffers must be able to store SymbolStringMax characters. Parameters
	/// may be 0 if not required.
	CORE_ABI bool lookup_address(core::ptr_type address,
								 char *name_buf,
								 char *file_buf,
								 int *line);

} // end namespace
} // end namespace
} // end namespace
} // end namespace

#endif // __SYMBOLS_H_CE530013_8589_44CA_9AC8_14A82A04B0DF_
