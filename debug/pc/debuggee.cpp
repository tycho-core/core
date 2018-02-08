//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Saturday, 17 May 2008 1:20:07 PM
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "debuggee.h"
#include "core/debug/assert.h"
#include "core/debug/utilities.h"

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>


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

	debuggee::debuggee() :
		m_listening_socket(0),
		m_client_socket(0),
		m_connected(false)
	{
	}

	//------------------------------------------------------------------------------------

	void debuggee::connect()
	{
		WORD	sock_version;
		WSADATA wsa_data;
		int		ret;

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
			return;
		}


		// create a listening socket
		m_listening_socket = socket(AF_INET,			// TCP/IP
									SOCK_STREAM,		// Stream oriented socket
									IPPROTO_TCP);		// TCP

		if(m_listening_socket == INVALID_SOCKET)
		{
			// TODO: lookup error and report it
			ret = WSAGetLastError();
			TYCHO_ASSERT(!"Error creating listening socket");
			WSACleanup();
			return;
		}


		// fill in socket address information
		SOCKADDR_IN sock_address;
		memset(&sock_address, 0, sizeof(sock_address));

		sock_address.sin_family		 = AF_INET;
		sock_address.sin_addr.s_addr = inet_addr("127.0.0.1");
		sock_address.sin_port		 = htons(m_port);


		// set socket to non-blocking (i.e. does not hang waiting for a client to accept)
		u_long arg = 1;
		ioctlsocket(m_listening_socket, FIONBIO, &arg);


		// bind the socket to the local server address
		ret = bind(m_listening_socket, (LPSOCKADDR)&sock_address, sizeof(sock_address));
		if(ret == SOCKET_ERROR)
		{
			// TODO: lookup error and report it
			ret = WSAGetLastError();
			TYCHO_ASSERT(!"Error binding socket");
			WSACleanup();
			return;
		}


		// listen for client connections
		ret = listen(m_listening_socket, 5);
		if(ret == SOCKET_ERROR)
		{
			// TODO: lookup error and report it
			ret = WSAGetLastError();
			TYCHO_ASSERT(!"Error opening for listen");
			WSACleanup();
			return;
		}
		debug::write_ln("Connection open for listening.");
	}

	//------------------------------------------------------------------------------------

	void debuggee::disconnect()
	{
		int ret;

		if(m_listening_socket)
		{
			ret = ::closesocket(m_listening_socket);
			if(ret == SOCKET_ERROR)
			{
				// TODO: lookup error and report it
				ret = WSAGetLastError();
				TYCHO_ASSERT(!"Error closing listening socket");
			}
		}

		if(m_connected)
		{
			ret = ::closesocket(m_client_socket);
			if(ret == SOCKET_ERROR)
			{
				// TODO: lookup error and report it
				ret = WSAGetLastError();
				TYCHO_ASSERT(!"Error closing connection socket");
			}
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

	bool debuggee::accept()
	{
		int ret;
		
		// check if a client is trying to connect
		m_client_socket = ::accept(m_listening_socket,
								   NULL,					// NOTE: may need these parameters
								   NULL);

		if(m_client_socket == INVALID_SOCKET)
		{
			// TODO: lookup error and report it
			ret = WSAGetLastError();

			// only report an error if it has occurred
			if(ret != WSAEWOULDBLOCK)
			{
				TYCHO_ASSERT(!"Error creating listening socket");
			}
			return false;
		}

		m_connected = true;
		debug::write_ln("Connection found!");
		return true;
	}

	//------------------------------------------------------------------------------------

	bool debuggee::send(const char* data)
	{
		size_t ret = ::send(m_client_socket, data, static_cast<int>(strlen(data)), 0);
		if(ret == SOCKET_ERROR)
		{
			return false;
		}

		debug::write_ln("Bytes sent: %i", ret);
		return true;
	}

	//------------------------------------------------------------------------------------

	bool debuggee::recv(char* data)
	{
		char buff[msg_size] = "";
		int ret = ::recv(m_client_socket, buff, msg_size, 0);

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
