//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Saturday, 12 April 2008 1:30:57 PM
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "core/memory/ansi_allocator.h"


//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////
namespace tycho
{
namespace core
{

	//------------------------------------------------------------------------------------

	void* ansi_allocator::malloc(size_t size, int /*flags*/, allocator_layer *)
	{
		return _aligned_malloc(size, 1);
	}
	
	//------------------------------------------------------------------------------------

	void* ansi_allocator::malloc_aligned(size_t size, size_t alignment, int /*flags*/, allocator_layer *)
	{
		return _aligned_malloc(size, alignment);
	}
	
	//------------------------------------------------------------------------------------

	void* ansi_allocator::realloc(void* memory, size_t newsize, int /*flags*/, allocator_layer*)
	{
		return _aligned_realloc(memory, newsize, 0);
	}

	//------------------------------------------------------------------------------------

	void ansi_allocator::free(void *p, int /*flags*/, allocator_layer *)
	{
		_aligned_free(p);
	}

	//------------------------------------------------------------------------------------

	size_t ansi_allocator::get_allocation_size(void *ptr, allocator_layer * /*next*/)
	{
		// from dbgint.h
		const int nNoMansLandSize = 4;
		struct _CrtMemBlockHeader
		{
			struct _CrtMemBlockHeader * pBlockHeaderNext;
			struct _CrtMemBlockHeader * pBlockHeaderPrev;
			char *                      szFileName;
			int                         nLine;
	#ifdef _WIN64
			/* These items are reversed on Win64 to eliminate gaps in the struct
			 * and ensure that sizeof(struct)%16 == 0, so 16-byte alignment is
			 * maintained in the debug heap.
			 */
			int                         nBlockUse;
			size_t                      nDataSize;
	#else  /* _WIN64 */
			size_t                      nDataSize;
	        int                         nBlockUse;
	#endif  /* _WIN64 */
			long                        lRequest;
			unsigned char               gap[nNoMansLandSize];
        };
        
        _CrtMemBlockHeader *hdr = ((_CrtMemBlockHeader*)ptr)-1;
        return hdr->nDataSize;
	}	

	//------------------------------------------------------------------------------------

} // end namespace
} // end namespace
