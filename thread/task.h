//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Wednesday, 5 November 2008 11:44:05 PM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __TASK_H_CC36E306_E374_4381_8D00_80613531B345_
#define __TASK_H_CC36E306_E374_4381_8D00_80613531B345_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "core/core_abi.h"
#include <vector>
#include <functional>

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace tycho
{
namespace core
{

    class CORE_ABI task
    {
    public:
		typedef std::function<void()> run_function_t;

	public:
		/// constructor
		task(uint32 task_id, run_function_t f);

		/// Called to run the task on the assigned thread
		void run();
		
    private:
		uint32				m_task_id;
		std::vector<uint32> m_dependent_tasks;		
		run_function_t		m_run_function;
    };

} // end namespace
} // end namespace

#endif // __TASK_H_CC36E306_E374_4381_8D00_80613531B345_
