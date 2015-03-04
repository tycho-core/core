//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Saturday, 17 May 2008 10:23:44 AM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __DLMALLOC_H_FC02AC52_F2BB_43F2_8405_C3A788E5CFFE_
#define __DLMALLOC_H_FC02AC52_F2BB_43F2_8405_C3A788E5CFFE_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "core/core_abi.h"

//////////////////////////////////////////////////////////////////////////////
// Refactored from Doug Lea's malloc.c v2.8.3 
// - only brought over code that deals with MSPACEs (preallocated blocks)
//////////////////////////////////////////////////////////////////////////////

namespace tycho
{
namespace core
{
namespace dlmalloc
{
	struct mallinfo 
	{
		size_t arena;    /* non-mmapped space allocated from system */
		size_t ordblks;  /* number of free chunks */
		size_t smblks;   /* always 0 */
		size_t hblks;    /* always 0 */
		size_t hblkhd;   /* space in mmapped regions */
		size_t usmblks;  /* maximum total allocated space */
		size_t fsmblks;  /* always 0 */
		size_t uordblks; /* total allocated space */
		size_t fordblks; /* total free space */
		size_t keepcost; /* releasable (via malloc_trim) space */
	};

	// allocation and free	
	void* mspace_malloc(void* mspace, size_t bytes);
	void* mspace_memalign(void* mspace, size_t alignment, size_t bytes);
	void* mspace_realloc(void* mspace, void* memory, size_t newsize);
	void mspace_free(void* mspace, void* memory);

	// mspace control	
	void* create_mspace_with_base(void* base, size_t capacity, int locked);
	size_t destroy_mspace(void* msp);

	// info
	mallinfo mspace_mallinfo(void* mspace);
	size_t memsize(void* memory);
	bool dl_only_mspaces();

} // end namespace
} // end namespace
} // end namespace

#endif // __DLMALLOC_H_FC02AC52_F2BB_43F2_8405_C3A788E5CFFE_
