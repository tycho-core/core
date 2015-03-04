//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Monday, 24 March 2008 4:30:24 PM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __DEBUGGEE_H_AE75F83A_64E2_4F96_8AA9_C98937CAD573_
#define __DEBUGGEE_H_AE75F83A_64E2_4F96_8AA9_C98937CAD573_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "core/core_abi.h"

// TODO: REMOVE - Include for test handler only
#include "core/globals.h"


//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace tycho
{
namespace core
{

	/// Client side debugger interface, this is the bit running in the game. Commands
	/// are handled by plugins allowing arbitrary tools to be added. This is intended to
	/// rely on as little of the rest of the library as possible to enable it to be run
	/// as early in the execution as possible until as late as possible, this also implies  
	/// using minimal memory allocation to try and ensure it can still operate in the face of
	/// exceptions and memory exhaustion by user code even though its capabilities will be
	/// limited in this situation.
    class CORE_ABI debuggee
    {
    public:
		/// command handler interface
		class CORE_ABI command_handler
		{
		public:
			/// command name
			virtual const char* name() = 0;
			
			/// run the command
			virtual void execute(const char *params) = 0;
			
			/// user help text for this command
			virtual const char* help_text() = 0;
		};
		
    public:
		debuggee();

		/// connect to a specific debugger
		virtual void connect() = 0;

		/// accept a connection
		virtual bool accept() = 0;

		virtual bool send(const char* data) = 0;

		virtual bool recv(char* data) = 0;
		
		/// run any debugger commands queued up, this should be called regularly to process the
		/// command queue. if commands can modify global state care should be taken to run this
		/// in a thread safe manner
		void run();

		/// register a command handler
		void register_handler(command_handler* handler);

	protected:
		static const int msg_size = 256;
					
    private:

		/// size of handler list
		static const int max_size = 32;	
		
		/// list of registered command handlers
		command_handler* handler_list[max_size];

		/// number of registered command handlers
		unsigned int num_handlers;

		/// queue of requested debug commands
		//char command_queue[msg_size][max_size];

		bool is_connected;
    
    };


    /// test example command handler
	class CORE_ABI debug_test_class
	{
	public:
		class CORE_ABI test_command_handler : public debuggee::command_handler
		{
		public:
			virtual const char* name() { return "oh"; }
			virtual void execute(const char * /*params*/)
			{
				// read input string executing the object modification
				// commands in it to syncronise our shared object tree
				// with the remote caller.
			}
			virtual const char* help_text() { return "help me"; }
		};

		void initialise()
		{
			test_command_handler* handler = new test_command_handler();
			
			// register with debugger
			globals::get_debuggee()->register_handler(handler);
		}
	};

} // end namespace
} // end namespace

#endif // __DEBUGGEE_H_AE75F83A_64E2_4F96_8AA9_C98937CAD573_
