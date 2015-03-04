//////////////////////////////////////////////////////////////////////////////
// Copyright 2006  Martin Slater
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef TYPES_H_MS_2006_8_1_20_57_18_
#define TYPES_H_MS_2006_8_1_20_57_18_


//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#if TYCHO_GC
#include <stddef.h> // for size_t
#endif

//////////////////////////////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace tycho
{

namespace core
{

#if TYCHO_GC || TYCHO_LINUX
	typedef char int8;
	typedef unsigned char uint8;
	typedef short int16;

	typedef unsigned short uint16;
	typedef int int32;
	typedef unsigned int uint32;
	typedef long long int64;
	typedef unsigned long long uint64;
			
	typedef float  float32;
	typedef double float64;
	typedef void* ptr_type;

#elif TYCHO_PC
	typedef __int8 int8;
	typedef unsigned __int8 uint8;
	typedef __int16 int16;
	typedef unsigned __int16 uint16;
	typedef __int32 int32;
	typedef unsigned __int32 uint32;
	typedef __int64 int64;
	typedef unsigned __int64 uint64;
	/// \todo  fix for win64
	typedef void* ptr_type;
			
	typedef float  float32;
	typedef double float64;
#endif 

typedef unsigned int uint;
	
} // namespace
} // namespace

#endif  // TYPES_H_MS_2006_8_1_20_57_18_

