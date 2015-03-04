//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Tuesday, 1 July 2008 10:55:43 PM
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "core/thread/thread.h"
#include "core/memory.h"
#include "core/debug/assert.h"
#include "core/pc/safe_windows.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////
namespace tycho
{
namespace core
{

	struct thread::pimpl
	{
		pimpl() :
			thread_handle(INVALID_HANDLE_VALUE),
			thread_id(0)
		{}
		
		HANDLE thread_handle;
		DWORD  thread_id;
	};

	thread::thread() :
		m_pimpl(new pimpl)
	{			
	}
	
	/// destructor
	thread::~thread()
	{
		core::safe_delete(m_pimpl);
	}
	
	static DWORD WINAPI thread_trampoline(void* fn)
	{
		thread::thread_function* thread_fn = reinterpret_cast<thread::thread_function*>(fn);
		return static_cast<DWORD>((*thread_fn)());
	}
	
	/// start the thread and execute the passed function.
	void thread::run(thread_function thread_fn)
	{
		DWORD thread_id = 0;
		HANDLE h = ::CreateThread(NULL, 0, &thread_trampoline, &thread_fn, 0, &thread_id);
		TYCHO_ASSERT(h != NULL);
		m_pimpl->thread_id = thread_id;
		m_pimpl->thread_handle = h;
	}
	
	/// kill the thread without prejudice.
	void thread::terminate()
	{
		HANDLE& h = m_pimpl->thread_handle;
		if(h != INVALID_HANDLE_VALUE)
		{
			::TerminateThread(h, 0);
			h = INVALID_HANDLE_VALUE;
		}
	}

} // end namespace
} // end namespace
