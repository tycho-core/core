//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Wednesday, 26 March 2008 12:13:44 AM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __THREAD_H_4825B807_AEA4_426B_9CA8_0229821D964A_
#define __THREAD_H_4825B807_AEA4_426B_9CA8_0229821D964A_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "core/core_abi.h"
#include "core/noncopyable.h"
#include <functional>

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace tycho
{
namespace core
{

    class CORE_ABI thread : noncopyable
    {
    public:
		typedef std::function<int()> thread_function;
		
		/// constructor
		thread();
		
		/// destructor
		~thread();
		
		/// start the thread and execute the passed function.
		void run(thread_function);
		
		/// kill the thread without prejudice.
		/// \warning this will not release any resources associated with the thread or call destructor's. 		
		void terminate();
				
    private:
		struct pimpl;
		pimpl* m_pimpl;    
    };

} // end namespace
} // end namespace

#endif // __THREAD_H_4825B807_AEA4_426B_9CA8_0229821D964A_
