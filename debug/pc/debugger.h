//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Saturday, 24 May 2008 8:59:05 PM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __DEBUGGER_H_994AC9EE_090C_4DD8_90BA_86FF36B310BA_
#define __DEBUGGER_H_994AC9EE_090C_4DD8_90BA_86FF36B310BA_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "core/core_abi.h"
#include "core/debug/debugger.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace tycho
{
namespace core
{
namespace pc
{
	class CORE_ABI debugger : public tycho::core::debugger
	{
	public:
		/// constructor
		debugger();
		
		/// connect to the game debuggee	
		virtual bool connect();

		/// disconnect from game debuggee
		virtual void disconnect();

		virtual bool send(const char* data);

		virtual bool recv(char* data);

	private:
		/// the connection socket
		unsigned int m_socket;
	};

} // end namespace
} // end namespace
} // end namespace

#endif // __DEBUGGER_H_994AC9EE_090C_4DD8_90BA_86FF36B310BA_
