//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Sunday, 9 March 2008 10:34:43 PM
//////////////////////////////////////////////////////////////////////////////
#include "core.h"
#include "globals.h"
#include "clock.h"
#include "debug/assert.h"
#include "core/debug/pc/debuggee.h"

namespace tycho
{
namespace core
{
	/// initialise the core tycho subsystems
	CORE_ABI void initialise()
	{
		globals::initialise();
		clock::initialise();
		initialise_assert();

		if(globals::get_debuggee())
		    globals::get_debuggee()->connect();
	}

	/// initialise the core tycho subsystems
	CORE_ABI void shutdown()
	{
		globals::shutdown();
	}

} // end namespace
} // end namespace
