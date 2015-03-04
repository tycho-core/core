//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2013 Martin Slater
// Created : Wednesday, 3 September 2014 11:58:02 PM
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "timer.h"
#include "core/clock.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////
namespace tycho
{
namespace core
{

    //--------------------------------------------------------------------

	void timer::start()
    {
        m_start_time = core::clock::get_time();
    }

    //--------------------------------------------------------------------
    
    double timer::elapsed_time() const
    {
        return core::clock::get_time() - m_start_time;
    }

    //--------------------------------------------------------------------


} // end namespace
} // end namespace
