//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Sunday, 30 March 2008 7:50:24 PM
//
// Endian helper routines.
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __ENDIAN_H_FD2344A3_D3C1_4F48_A760_7526FB4F9F89_
#define __ENDIAN_H_FD2344A3_D3C1_4F48_A760_7526FB4F9F89_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "core/core_abi.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace tycho
{
namespace core
{
	
	inline core::int32 endian_swap(core::int32 i)
	{
		core::int32 res;
		const char* src = (char*)&i;
		char* dst = (char*)&res;
		dst[0] = src[3];
		dst[1] = src[2];
		dst[2] = src[1];
		dst[3] = src[0];
		return res;
	}
	

} // end namespace
} // end namespace

#endif // __ENDIAN_H_FD2344A3_D3C1_4F48_A760_7526FB4F9F89_
