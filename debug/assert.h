//////////////////////////////////////////////////////////////////////////////
// Copyright 2006  Martin Slater
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER

#ifndef ASSERT_H_MS_2006_11_27_12_57_29_
#define ASSERT_H_MS_2006_11_27_12_57_29_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "core/core_abi.h"

//////////////////////////////////////////////////////////////////////////////
// FUNCTIONS
//////////////////////////////////////////////////////////////////////////////

#if TYCHO_ASSERT_ENABLED

namespace tycho
{
	namespace core
	{

		struct assert_exception : std::exception
		{
			const char *expr;
			const char *msg;
			const char *file;
			unsigned line;
		};

		typedef void(assert_handler)(const char *, const char *, const char *, unsigned);

		CORE_ABI assert_handler *set_assert_handler(assert_handler *h);
		CORE_ABI void doassert(const char *expr_txt, const char *msg_txt, const char *file, unsigned line);
		CORE_ABI void default_assert(const char *expr_txt, const char *msg_txt, const char *file, unsigned line);
		CORE_ABI void dopanic(const char *msg_txt, const char *file, unsigned line);
		CORE_ABI void assert_handler_exception(const char *expr, const char *msg, const char *file, unsigned line);
		CORE_ABI void initialise_assert();

	} // namespace core
} // namespace tycho

#endif // TYCHO_ASSERT_ENABLED

//////////////////////////////////////////////////////////////////////////////
// MACROS
//////////////////////////////////////////////////////////////////////////////

#define TYCHO_PANIC(_msg) tycho::core::dopanic(_msg, __FILE__, __LINE__)

#if TYCHO_ASSERT_ENABLED
#define TYCHO_ASSERT(_expr) (void)((!!(_expr)) || (tycho::core::doassert(#_expr, 0, __FILE__, __LINE__), 0))
#define TYCHO_ASSERT_MSG(_expr, _msg) (void)((!!(_expr)) || (tycho::core::doassert(#_expr, _msg, __FILE__, __LINE__), 0))
#define TYCHO_ASSERT_MEM TYCHO_ASSERT(_CrtCheckMemory())
#define TYCHO_VERIFY(_expr) TYCHO_ASSERT(_expr)
#else
#define TYCHO_VERIFY(_expr) (_expr)
#define TYCHO_ASSERT(_expr)
#define TYCHO_ASSERT_MSG(_expr, _msg)
#define TYCHO_ASSERT_MEM
#endif

#define TYCHO_NOT_IMPLEMENTED TYCHO_ASSERT(!"implement me please!");

#endif // ASSERT_H_MS_2006_11_27_12_57_29_
