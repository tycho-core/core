//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Monday, 24 March 2008 5:15:16 AM
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "core/memory/new.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////
using namespace tycho::core;

#if 0

/// override global new operator
void* __CRTDECL operator new(size_t size)
{
	return tycho::core::allocator::malloc(static_cast<tycho::core::uint32>(size));
}

/// override global new operator
void* __CRTDECL operator new(size_t size, const std::nothrow_t & ) throw()
{
	return tycho::core::allocator::malloc(static_cast<tycho::core::uint32>(size));
}

/// override global array new operator
void* __CRTDECL operator new[](size_t size)
{
	return tycho::core::allocator::malloc(static_cast<tycho::core::uint32>(size));
}

/// override global array new operator
void* __CRTDECL operator new[](size_t size, const std::nothrow_t & ) throw()
{
	return tycho::core::allocator::malloc(static_cast<tycho::core::uint32>(size));
}

/// override global delete operator
void __CRTDECL operator delete(void *ptr)
{
	tycho::core::allocator::free(ptr);
}

/// override global delete operator
void __CRTDECL operator delete(void *ptr, const std::nothrow_t & ) throw()
{
	tycho::core::allocator::free(ptr);
}

/// override global array delete operator
void __CRTDECL operator delete[](void *ptr)
{
	tycho::core::allocator::free(ptr);
}

/// override global array delete operator
void __CRTDECL operator delete[](void *ptr, const std::nothrow_t & ) throw()
{
	tycho::core::allocator::free(ptr);
}

#endif // 0