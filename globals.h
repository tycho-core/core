//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Saturday, 29 March 2008 7:39:33 PM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __GLOBALS_H_9F339E44_892A_4983_8CD2_1AE48A342973_
#define __GLOBALS_H_9F339E44_892A_4983_8CD2_1AE48A342973_

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
	class program_config;
	class debuggee;

namespace globals
{
			
	CORE_ABI void initialise();
	CORE_ABI void shutdown();
	CORE_ABI tycho::core::program_config* get_config();
	CORE_ABI tycho::core::debuggee*		  get_debuggee();
//	CORE_ABI tycho::io::interface&  get_io();

} // end namespace
} // end namespace
} // end namespace

#endif // __GLOBALS_H_9F339E44_892A_4983_8CD2_1AE48A342973_
