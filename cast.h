//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Monday, 8 December 2008 6:48:37 AM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __CAST_H_3613F653_D053_445F_8B23_02B6417229B0_
#define __CAST_H_3613F653_D053_445F_8B23_02B6417229B0_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "core/core_abi.h"
#include "core/debug/assert.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace tycho
{
namespace core
{

	/// Cast that only checks in debug if its being cast to a derived type. In all other
	/// builds it just uses a static cast so this can only be used when you 'know' what the 
	/// destination type is known and you want to sanity check it.
	template<class DstType, class SrcType>	DstType debug_checked_cast(SrcType src)
	{
#if TYCHO_DEBUG
		DstType r = dynamic_cast<DstType>(src);
		TYCHO_ASSERT(r);
		return r;
#else 
		return static_cast<DstType>(src);
#endif
	}

} // end namespace
} // end namespace

#endif // __CAST_H_3613F653_D053_445F_8B23_02B6417229B0_
