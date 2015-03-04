//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Saturday, 17 May 2008 1:20:07 PM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __DEBUGGEE_H_64150036_49EF_47F7_8CFF_9C0980FB1D69_
#define __DEBUGGEE_H_64150036_49EF_47F7_8CFF_9C0980FB1D69_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "core/core_abi.h"
#include "core/debug/debuggee.h"


//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace tycho
{
namespace core
{
namespace pc
{
	class CORE_ABI debuggee : public tycho::core::debuggee
	{
	public:
		/// constructor
		debuggee();

		/// connect to a specific debugger
		virtual void connect();

		/// disconnect
		virtual void disconnect();
		
		/// accept an incoming connection
		virtual bool accept();

		virtual bool send(const char* data);

		virtual bool recv(char* data);

	private:
		/// number of connections
		static const int m_num_connections = 5;
		
		/// random port number
		static const int m_port = 3000;

		/// socket for listening for client connections
		unsigned int m_listening_socket;

		/// socket for connected clients
		unsigned int m_client_socket;

		bool m_connected;

	};
} // end namespace
} // end namespace
} // end namespace

#endif // __DEBUGGEE_H_64150036_49EF_47F7_8CFF_9C0980FB1D69_
