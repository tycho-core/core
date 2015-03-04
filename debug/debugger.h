//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Monday, 24 March 2008 3:41:43 PM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __DEBUGGER_H_2FC5A304_6038_45F9_A119_204150E5CEFF_
#define __DEBUGGER_H_2FC5A304_6038_45F9_A119_204150E5CEFF_

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
	
	/// information describing a particular debuggee available
	/// for debugging.
	struct debuggee_info
	{
		/// maximum length of the name member
		static const int max_name_len = 128;
		
		/// name of debuggee
		char name[max_name_len];
		
		/// version of the debugger they are running
		int  version;
		
		/// platform it is running on
		int  platform;
	};

	/// connection from debugger to a specific debuggee
	class CORE_ABI debug_connection
	{
	public:
		/// cause the debuggee to execute a break interrupt
		void execute_break();						
	};
	
	/// Engine debugger interface. This mediates all debug requests and information
	/// to a particular running game session. A debuggee and debugger communicate through
	/// a simple command response interface that are handled via debugger plugins
    class CORE_ABI debugger
    {
    public:
		/// connect to a debuggee
		virtual bool connect();
		
		/// disconnect a debug session
		virtual void disconnect();
	
		/// send a command to connected debuggee
		virtual bool send(const char* data) = 0;

		/// receive data from debuggee
		virtual bool recv(char* data) = 0;

    protected:
		static const int msg_size = 256;
    
    };

} // end namespace
} // end namespace

#endif // __DEBUGGER_H_2FC5A304_6038_45F9_A119_204150E5CEFF_
