//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Monday, 24 March 2008 4:30:24 PM
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "debuggee.h"
#include "assert.h"
#include "utilities.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////
namespace tycho
{
namespace core
{
	/// constructor
	debuggee::debuggee() :
		num_handlers(0),
		is_connected(false)
	{
	}
	
	/// run any debugger commands queued up, this should be called regularly to process the
	/// command queue. if commands can modify global state care should be taken to run this
	/// in a thread safe manner
	void debuggee::run()
	{
		// check for a client connection if one does not exist
		if(!is_connected)
		{
			is_connected = accept();
			return;
		}

		// check for incoming data
		char buff[msg_size] = "";

		if(recv(buff))
		{
			// handle command
			debug::write_ln("Command: %s", buff);
			send("Hi from the debuggee.");
		}
	}

	/// register a command handler
	void debuggee::register_handler(command_handler* handler)
	{
		// if this assert breaks then we have run out of space in the handler array, increase
		// size of max_size to fix.
		TYCHO_ASSERT(num_handlers < max_size);
		if(num_handlers >= max_size)
		{
			return;
		}

		handler_list[num_handlers] = handler;
		++num_handlers;
	}

} // end namespace
} // end namespace
