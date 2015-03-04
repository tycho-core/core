//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Wednesday, 5 March 2008 7:30:39 PM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __CORE_ABI_H_8B5DED01_1931_4953_9E06_58E760DF081B_
#define __CORE_ABI_H_8B5DED01_1931_4953_9E06_58E760DF081B_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "core_abi.h"
#ifdef __cplusplus
#include "core/types.h"
#endif 

#if TYCHO_PC

// configuration macros
#define TYCHO_BIG_ENDIAN    0
#define TYCHO_LITTLE_ENDIAN 1

#ifndef TYCHO_GC
#define TYCHO_GC 0
#endif 

// DLL interface
#ifdef TYCHO_CORE_EXPORTS
#define CORE_ABI __declspec(dllexport)
#define ZLIB_INTERNAL
#define ZLIB_DLL

#else
#define CORE_ABI __declspec(dllimport)
#endif 

// disable a few warnings globally. should move these into specific cpp's to avoid polluting
// user header files
#pragma warning(disable : 4251) // class needs to have dll-interface to be used by clients of class ''
#pragma warning(disable : 4355) // 'this' : used in base member initializer list

#else // TYCHO_PC


// configuration macros
#define TYCHO_BIG_ENDIAN    1
#define TYCHO_LITTLE_ENDIAN 0

#ifndef TYCHO_PC
#define TYCHO_PC 0
#endif 

#define CORE_ABI

#endif // TYCHO_GC

#ifndef TYCHO_NO_EXCEPTIONS
#define TYCHO_NO_EXCEPTIONS 0
#endif

#ifdef __cplusplus
#include "core/memory/new.h"
#include "core/forward_decls.h"
#endif 

#endif // __CORE_ABI_H_8B5DED01_1931_4953_9E06_58E760DF081B_
