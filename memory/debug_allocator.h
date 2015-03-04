//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Monday, 24 March 2008 5:08:39 AM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __DEBUG_ALLOCATOR_H_13D36C7A_56A3_47E8_8A30_DCDC6A963966_
#define __DEBUG_ALLOCATOR_H_13D36C7A_56A3_47E8_8A30_DCDC6A963966_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "core/core_abi.h"
#include "core/memory/allocator_layer.h"
#include "core/containers/hash_table.h"
#include "core/hash.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace tycho
{
namespace core
{

	/// The debug allocator layer adds memory debugging support to its underlying
	/// raw allocator layer. This allows differing underlying allocators while still
	/// implementing a consistent set of debugging aids on top of it. This may possibly
	/// expand the memory requests to add its own per allocation information.
    class CORE_ABI debug_allocator : public allocator_layer
    {
    public:
		/// feature flags used to enable specific debugging aids.
		enum features
		{
			/// nothing enabled
			f_none = 0,
			
			/// add markers at beginning and end of allocation to check for memory corruption
			f_overrun_detect = (1 << 0),
			
			/// fill newly allocated memory with a specific pattern
			f_fill_allocated = (1 << 1),

			/// fill newly freed memory with a specific pattern
			f_fill_freed = (1 << 2),
			
			/// add memory leak tracking
			f_memleaks = (1 << 3),
			
			/// all debugging features
			f_all = f_overrun_detect | f_fill_allocated | f_fill_freed | f_memleaks
		};
		
    public:
		/// constructor
		debug_allocator(int feature_flags = f_all);

		///  destructor
		~debug_allocator();
		
		/// set the features enabled. see \ref debug_allocator::features.
		void set_features(int feature_flags)
			{ TYCHO_ASSERT(m_mem_stats.get_used() == 0); m_features = feature_flags; }
			
		/// \name allocator_layer interface
		//@{
		virtual void*  malloc(size_t size, int flags, allocator_layer *next);
		virtual void*  malloc_aligned(size_t size, size_t alignment, int flags, allocator_layer *next);
		virtual void*  realloc(void* memory, size_t newsize, int flags, allocator_layer *next);
		virtual void   free(void *, int flags, allocator_layer *next);        		
		virtual size_t get_allocation_size(void *ptr, allocator_layer *next);
		//@}
		
		/// check that there have been no illegal modifications due to stray writes or buffer over-runs / under-runs
		bool check_consistency();
					
		/// print out all currently allocated memory 
		void print_allocated();
		
	private:
		struct stats
		{
		public:
			stats() : m_peak(0), m_used(0) {}
			
			void inc(int amount) { m_used += amount; if(m_used > m_peak) m_peak = m_used; }
			void dec(int amount) { m_used -= amount; TYCHO_ASSERT(m_used >= 0); }
			int get_used() const { return m_used; }
			int get_peak() const { return m_peak; }
		
		private:
			int m_peak; ///< peak amount of memory allocated 
			int m_used; ///< current amount of memory allocated
		};
		
		int m_features;				///< features currently enabled
		stats m_mem_stats;			///< statistics for normal allocations
		stats m_internal_mem_stats;	//< statistics for internal allocations (made by the debug allocator itself and so cannot be tracked currently)

		///< hash table of all currently allocated pointers
		/// \todo  store stack in its own hash table to remove duplicates
		//		 and reduce the size of the per allocation debug info.
		//		 currently uses (MaxStackDepth + 2) * sizeof(ptr_type) + 2 * sizeof(ptr_type)
		//       per node (including hash table linked list pointers).
		//		 this is 80 bytes per allocation on a 32 bit ptr system, which is a crazy amount.
		struct debug_info
		{
			static const int MaxStackDepth = 16;
			core::ptr_type address;
			size_t		   padding;
			size_t		   size;
			core::ptr_type stack[MaxStackDepth];				
		};
		
		/// all memory allocated inside the memory manager must use this allocator
		/// to avoid recursion.
		class internal_debug_allocator
		{				
		public:
			void* allocate(size_t n, int flags = 0)
			{
				return allocator::malloc(n, flags | allocator::f_internal);
			}
			
			void* allocate(size_t n, size_t alignment, int flags = 0)
			{
				return allocator::malloc_aligned(n, alignment, flags | allocator::f_internal);			  
			}
			
			void deallocate(void* p, size_t /*n*/)
			{
				allocator::free(p, allocator::f_internal);
			}
		};
		
		/// hash table for storing allocation debug information in						
		/// \todo  all allocations in a hash table are of the same size
		//       so would be good to use a free list allocator here for
		//       speed and to reduce fragmentation.
		typedef hash_table<core::ptr_type, 
						   debug_info, 
						   1021, 
						   compare_equals<core::ptr_type>,
						   internal_debug_allocator
						   > di_hash_table;
		
		struct allocation_info
		{
			ptr_type real_ptr;			///< pointer to the real allocation, may be different to the users area depending on debug options.
			ptr_type user_ptr;			///< pointer to start of the users area of memory
			int*	 overwrite_front;	///< pointer to front overwrite protection area or 0 if none in use for this allocations
			int*	 overwrite_back;	///< pointer to back overwrite protection area or 0 if none in use for this allocations
			size_t   real_size;			///< total size of allocation
			size_t   user_size;			///< size the user requested
			size_t   padding;			///< number of padding bytes for alignment at the front of allocation
			bool	 internal;			///< true if this is an internal allocation.
			di_hash_table::iterator debug_it;
			debug_info* dbg_info;
		};
		
		struct size_info
		{
			size_t real_size;	///< total allocation size
			size_t user_size;	///< size user requested
			size_t padding;		///< size that is alignment padding
		};
		
// 		ptr_info get_real_ptr(ptr_type ptr, int flags, allocator_layer *next_layer);
// 		void check_for_overrun(ptr_info);
		size_info calculate_size(size_t user_size, size_t alignment, int flags);
// 		ptr_type set_debug_info(ptr_info ptr, const size_info& size, int flags);
		di_hash_table::iterator get_debug_info(ptr_type ptr);
		
		/// Fill an allocation info structure for an existing allocation. This takes the user pointer.
		void get_allocation_info(allocation_info& info, void* user_ptr, int flags, allocator_layer* next_layer);
		
		/// Fill out an allocation_info structure for a new allocation. This takes the real pointer.
		void make_allocation_info(allocation_info& info, void* real_ptr, size_t real_size, size_t user_size, size_t padding, int flags);
		
		/// Add a debug entry if needed for the allocation
		void add_debug_info(allocation_info& info, int fill_offset, int fill_size, size_t real_size);
		
		/// Remove debug info for this allocation and update memory stats.
		void free_debug_info(allocation_info& info, allocator_layer* next_layer);
		
		/// Fill an allocation info structure for an existing allocation. This takes the user pointer.
		void get_allocation_info(allocation_info& info, void* user_ptr, int flags);
		
		/// calculates the number of bytes needed for the debug allocator at the front and
		/// back of the user allocation
		void get_num_debug_bytes(int flags, size_t& leading_bytes, size_t& trailing_bytes);
		
		/// Asserts if the overrun protection bytes have changed
		void check_for_overrun(allocation_info& pinfo);
		
								   
		di_hash_table m_debug_info;
    };

} // end namespace
} // end namespace

#endif // __DEBUG_ALLOCATOR_H_13D36C7A_56A3_47E8_8A30_DCDC6A963966_
