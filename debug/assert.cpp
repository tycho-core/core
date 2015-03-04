//////////////////////////////////////////////////////////////////////////////
// Copyright 2006  Martin Slater
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "assert.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace tycho
{
namespace core
{

   	namespace detail
	{
		static assert_handler *handler = &default_assert;
	}

    //--------------------------------------------------------------------

	CORE_ABI assert_handler* set_assert_handler(assert_handler *h)
	{
		assert_handler *oh = detail::handler;
		detail::handler = h;
		return oh;		
	}

    //--------------------------------------------------------------------

    CORE_ABI void doassert(const char *expr_txt, const char* msg_txt, const char *file, unsigned line)
    {
        if(detail::handler)
            detail::handler(expr_txt, msg_txt, file, line);
    }

    //--------------------------------------------------------------------

    CORE_ABI void assert_handler_exception(const char *expr, const char *msg, const char *file, unsigned line)
	{
		assert_exception ex;
        ex.expr = expr;
		ex.msg = msg;
		ex.file = file;
		ex.line = line;
		throw(ex);
	}

    //--------------------------------------------------------------------

} // end namespace
} // end namespace
