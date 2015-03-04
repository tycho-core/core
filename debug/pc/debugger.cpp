//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Saturday, 24 May 2008 8:59:05 PM
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "debugger.h"
#include "core/debug/assert.h"
#include "core/debug/utilities.h"
#include <WinSock2.h>
#include <Ws2tcpip.h>

#pragma comment(lib,"Ws2_32.lib")

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////
namespace tycho
{
namespace core
{
namespace pc
{
	//------------------------------------------------------------------------------------

	debugger::debugger() :
		m_socket(0)
	{
	}

	//------------------------------------------------------------------------------------

	bool debugger::connect()
	{
		WORD		sock_version;
		WSADATA		wsa_data;
		int			ret;
		SOCKADDR_IN address;
		char		host_name[256];
		char		serv_info[256];


		// set Winsock version 2.2
		sock_version = MAKEWORD(2,2);


		// initialise WinSock
		ret = WSAStartup(sock_version, &wsa_data);
		if(ret != NO_ERROR)
		{
			// TODO: lookup error and report it
			ret = WSAGetLastError();
			TYCHO_ASSERT(!"Error on WSAStartup");
			WSACleanup();
			return false;
		}


		// setup server vars
		address.sin_family		= AF_INET;
		address.sin_port		= htons(3000);
		address.sin_addr.s_addr	= inet_addr("127.0.0.1");
		

		// get server name info
		ret = getnameinfo((SOCKADDR*)&address,
						  sizeof(SOCKADDR),
						  host_name,
						  256,
						  serv_info,
						  256,
						  NI_NUMERICSERV);
		if(ret != NO_ERROR)
		{
			// TODO: lookup error and report it
			ret = WSAGetLastError();
			TYCHO_ASSERT(!"Error on getnameinfo");
			WSACleanup();
			return false;
		}


		// create the socket
		m_socket = socket(AF_INET,			// TCP/IP
						  SOCK_STREAM,		// Stream oriented socket
						  IPPROTO_TCP);		// TCP
		if(m_socket == INVALID_SOCKET)
		{
			// TODO: lookup error and report it
			ret = WSAGetLastError();
			TYCHO_ASSERT(!"Error creating client socket");
			WSACleanup();
			return false;
		}


		// connect to server
		ret = ::connect(m_socket, (LPSOCKADDR)&address, sizeof(SOCKADDR));
		if(ret != NO_ERROR)
		{
			// TODO: lookup error and report it
			ret = WSAGetLastError();
			WSACleanup();
			return false;
		}

		debug::write_ln("Connection found!");
		return true;
	}

	//------------------------------------------------------------------------------------

	void debugger::disconnect()
	{
		int ret = ::closesocket(m_socket);
		if(ret == SOCKET_ERROR)
		{
			// TODO: lookup error and report it
			ret = WSAGetLastError();
			TYCHO_ASSERT(!"Error closing connection");
		}

		ret = WSACleanup();
		if(ret == SOCKET_ERROR)
		{
			// TODO: lookup error and report it
			ret = WSAGetLastError();
			TYCHO_ASSERT(!"Error closing connection");
		}
	}

	//------------------------------------------------------------------------------------

	bool debugger::send(const char* data)
	{
		int ret = ::send(m_socket, data, strlen(data), 0);
		if(ret == SOCKET_ERROR)
		{
			return false;
		}

		return true;
	}

	//------------------------------------------------------------------------------------

	bool debugger::recv(char* data)
	{
		char buff[msg_size] = "";
		int ret = ::recv(m_socket, buff, msg_size, 0);

		// error in receiving data
		if(ret == SOCKET_ERROR || ret == 0)
		{
			return false;
		}
		else
		{
			// copy data and indicate a message has been received
			strcpy(data, buff);
			return true;
		}
	}

	//------------------------------------------------------------------------------------

} // end namespace
} // end namespace
} // end namespace
