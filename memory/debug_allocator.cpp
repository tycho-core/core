//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Monday, 24 March 2008 5:08:39 AM
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "core/types.h"
#include "core/debug/assert.h"
#include "core/debug/utilities.h"
#include "core/string.h"
#include "core/memory/debug_allocator.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////
namespace tycho
{
namespace core
{
		
	//------------------------------------------------------------------------------------
		
	/// pattern to initialise newly allocated ram to
	const int AllocatedRamFillPattern = 0x0DF0DEC0; // c0def00d
	
	/// pattern to set ram to after deallocation
    //const int UnallocatedRamFillPattern = 0x0DF0ADDE; // deadf00d
	
	/// pattern to write at end of allocation to detect overruns
	const int OverwriteDetectPattern = 0x0DD0ADDE; // deadd00d
				
	//------------------------------------------------------------------------------------
				
	debug_allocator::debug_allocator(int features) :
		m_features(features)
	{			
	}
	
	//------------------------------------------------------------------------------------

	debug_allocator::~debug_allocator()
	{
		m_features = 0;
	}

	//------------------------------------------------------------------------------------

	void* debug_allocator::malloc(size_t size, int flags, allocator_layer *next_layer)
	{			
		return malloc_aligned(size, allocator::default_alignment, flags, next_layer);
	}
	
	//------------------------------------------------------------------------------------

	void* debug_allocator::malloc_aligned(size_t size, size_t alignment, int flags, allocator_layer *next_layer)
	{			
		// [align padding][our header][user data]
		size_info sinfo = calculate_size(size, alignment, flags);
		void*  new_ptr = next_layer->malloc_aligned(sinfo.real_size, alignment, flags, 0);
		TYCHO_ASSERT(new_ptr);
		allocation_info new_alloc_info;
		make_allocation_info(new_alloc_info, (void*)new_ptr, sinfo.real_size, size, sinfo.padding, flags);
		add_debug_info(new_alloc_info, 0, size, next_layer->get_allocation_size(new_ptr, 0));
		return (void*)new_alloc_info.user_ptr;
	}

	//------------------------------------------------------------------------------------

	void* debug_allocator::realloc(void* ptr, size_t new_size, int flags, allocator_layer* next_layer)
	{
		// this will need moving debug's footer, and if that fails we'll need to 
		// realloc from the next_layer but maintain alignment
		allocation_info cur_alloc_info;
		get_allocation_info(cur_alloc_info, ptr, flags, next_layer);		
		check_for_overrun(cur_alloc_info);
		size_t old_size = cur_alloc_info.user_size;		
		free_debug_info(cur_alloc_info, next_layer);		
		size_info sinfo = calculate_size(new_size, allocator::default_alignment, flags);
		void* new_ptr = next_layer->realloc((void*)cur_alloc_info.real_ptr, sinfo.real_size, flags, 0);		
		TYCHO_ASSERT(new_ptr);
		allocation_info new_alloc_info;
		make_allocation_info(new_alloc_info, (void*)new_ptr, sinfo.real_size, new_size, 0, flags);
		add_debug_info(new_alloc_info, (new_size < old_size) ? -1 : old_size, new_size - old_size, next_layer->get_allocation_size(new_ptr, 0));
		return (void*)new_alloc_info.user_ptr;
	}
	
	//------------------------------------------------------------------------------------

	void debug_allocator::free(void *ptr, int flags, allocator_layer *next_layer)
	{
		allocation_info alloc_info;
		get_allocation_info(alloc_info, ptr, flags, next_layer);		
		check_for_overrun(alloc_info);
		free_debug_info(alloc_info, next_layer);		
		next_layer->free((void*)alloc_info.real_ptr, flags, 0);
	}

	//------------------------------------------------------------------------------------

	debug_allocator::di_hash_table::iterator debug_allocator::get_debug_info(ptr_type ptr)
	{
		return m_debug_info.find(ptr);
	}
	
	//------------------------------------------------------------------------------------

	void debug_allocator::check_for_overrun(allocation_info& pinfo)	
	{
		if(!pinfo.internal)
		{
			TYCHO_ASSERT(*pinfo.overwrite_front == OverwriteDetectPattern);
			TYCHO_ASSERT(*pinfo.overwrite_back == OverwriteDetectPattern);
		}
	}
	
	//------------------------------------------------------------------------------------

	debug_allocator::size_info debug_allocator::calculate_size(size_t user_size, size_t alignment, int flags)
	{
		size_t alloc_size = user_size;
		const bool internal_alloc = (flags & allocator::f_internal) != 0;

		// [align padding][our header][user data]
		size_t padding = 0;

		// don't add any debug info if its an internal allocation to avoid recursion
		// means we can't detect leaks in the leak checker
		if(!internal_alloc)
		{
			size_t leading_bytes, trailing_bytes;
			get_num_debug_bytes(flags, leading_bytes, trailing_bytes);
			if(leading_bytes)
			{
				// alignment padding, if larger than header size
				padding = (alignment > leading_bytes ? alignment - leading_bytes : 0);
			}	
			alloc_size += padding + leading_bytes + trailing_bytes;
		}
		size_info sinfo;
		sinfo.real_size = alloc_size;
		sinfo.user_size = user_size;
		sinfo.padding = padding;
		
		return sinfo;	
	}
	

	//------------------------------------------------------------------------------------

	/// calculates the number of bytes needed for the debug allocator at the front and
	/// back of the user allocation
	void debug_allocator::get_num_debug_bytes(int flags, size_t& leading_bytes, size_t& trailing_bytes)
	{
		leading_bytes = 0;
		trailing_bytes = 0;
		if((flags & allocator::f_internal) > 0)
			return;
		
		if(m_features & f_overrun_detect)
		{
			leading_bytes += 4;		
			trailing_bytes += 4;
		}		
	}
	
	//------------------------------------------------------------------------------------

	void debug_allocator::get_allocation_info(allocation_info& info, void* user_ptr, int flags, allocator_layer* next_layer)
	{
		const bool internal_alloc = (flags & allocator::f_internal) != 0;
		info.internal = internal_alloc;
		if(!internal_alloc)
		{
			di_hash_table::iterator it = get_debug_info((ptr_type)user_ptr);
			TYCHO_ASSERT_MSG(it != m_debug_info.end(), "Trying to free a pointer that is not heap allocated.\nThis is possibly caused by passing a pointer to a static or stack allocated object.");
			debug_info* di = &it->second;
			size_t leading_bytes, trailing_bytes;
			get_num_debug_bytes(flags, leading_bytes, trailing_bytes);
			info.real_ptr  = (ptr_type)((char*)user_ptr - leading_bytes - di->padding);
			info.user_ptr  = user_ptr;
			info.real_size = di->size + leading_bytes + trailing_bytes + di->padding;
			info.user_size = info.real_size - leading_bytes - trailing_bytes - di->padding;
			info.padding   = di->padding;
			info.dbg_info = di;
			info.debug_it   = it;
			if(m_features & f_overrun_detect)
			{
				info.overwrite_front = (core::int32*)((char*)info.user_ptr - 4);
				info.overwrite_back  = (core::int32*)((char*)info.user_ptr + info.user_size);
			}
		}
		else
		{
			info.real_ptr = info.user_ptr = (ptr_type)user_ptr;
			info.real_size = info.user_size = next_layer->get_allocation_size(user_ptr, 0);
			info.overwrite_front = info.overwrite_back = 0;
			info.padding = 0;
		}
	}
	
	//------------------------------------------------------------------------------------
	
	void debug_allocator::make_allocation_info(allocation_info& info, void* real_ptr, size_t real_size, size_t user_size, size_t padding, int flags)
	{
		const bool internal_alloc = (flags & allocator::f_internal) != 0;
		info.internal = internal_alloc;
		info.debug_it = m_debug_info.end();
		info.dbg_info = 0;
		if(!internal_alloc)
		{
			size_t leading_bytes, trailing_bytes;
			get_num_debug_bytes(flags, leading_bytes, trailing_bytes);
			info.padding = padding;
			info.real_ptr = (ptr_type)real_ptr;
			info.user_ptr = (ptr_type)((char*)real_ptr + padding + leading_bytes);
			info.real_size = real_size;
			info.user_size = user_size;
			if(m_features & f_overrun_detect)
			{
				info.overwrite_front = (core::int32*)((char*)info.user_ptr - 4);
				info.overwrite_back  = (core::int32*)((char*)info.user_ptr + info.user_size);
			}
		}
		else
		{
			info.real_size = real_size;
			info.user_size = user_size;
			info.padding = 0;
			info.real_ptr = info.user_ptr = (ptr_type)real_ptr;
			info.overwrite_front = info.overwrite_back = 0;
		}
	}
	
	//------------------------------------------------------------------------------------
	
	void debug_allocator::add_debug_info(allocation_info& info, size_t fill_offset, size_t fill_size, size_t real_size)
	{
		//if((m_mem_stats.get_peak()+real_size) > 1024 * 1024)
		//{
		//	core::debug::write_ln("peak > 1mg!");
		//}

		if(!info.internal)
		{
			// avoid the need for this unless leak detection is enabled.
			debug_info di;
			core::mem_zero(di);					
			di.size = info.user_size;
			di.address = info.user_ptr;
			di.padding = info.padding;

			if(info.overwrite_front)
				*info.overwrite_front = OverwriteDetectPattern;
			if(info.overwrite_back)
				*info.overwrite_back = OverwriteDetectPattern;
			if(m_features & f_memleaks)
			{
				// capture caller stack trace for this allocation and pass it on to the 
				// allocation tracker via the debuggee interface. this allows the data to
				// be recorded by the host debugger rather than consuming ram from the 
				// running process which may be on a limited memory system (i.e. gamecube)
				debug::stacktrace(di.stack, debug_info::MaxStackDepth);
				/// \todo need to get the address of the new operator and the __tmainCRTStartup
				///       functions and use these to top n tail the stack track so its only
				///       within user code
			}
			if(m_features & f_fill_allocated)
			{
				if(fill_offset != -1)
					core::mem_set32((char*)info.user_ptr + fill_offset, AllocatedRamFillPattern, fill_size);
			}	
			m_debug_info.insert(std::make_pair((ptr_type)info.user_ptr, di));								
			m_mem_stats.inc(real_size);	
		}	
		else
		{
			m_internal_mem_stats.inc(real_size);			
		}
		
	}	
	
	//------------------------------------------------------------------------------------
	
	void debug_allocator::free_debug_info(allocation_info& info, allocator_layer* next_layer)
	{
		// release any debug info we have for this allocation
		size_t actual_size = next_layer->get_allocation_size(info.real_ptr, 0);
		if(!info.internal)
		{
			TYCHO_ASSERT(info.dbg_info);
			m_mem_stats.dec(actual_size);
			m_debug_info.erase(info.debug_it);
		}
		else
		{
			m_internal_mem_stats.dec(actual_size);
		}
	}
	
	//------------------------------------------------------------------------------------
	
	/// attempt to find the size of this chunk
	/// may or may not be internal allocation, may not even be allocated by this allocator
	size_t debug_allocator::get_allocation_size(void* ptr, allocator_layer *next)
	{		
		di_hash_table::const_iterator it = m_debug_info.find((ptr_type)ptr);
		if (it != m_debug_info.end())
		{
			return it->second.size;
		}

		return next->get_allocation_size(ptr, 0);
	}

	//------------------------------------------------------------------------------------

	void debug_allocator::print_allocated()
	{
		const int BufLen = 256;
		char buf[BufLen];
		di_hash_table::const_iterator it = m_debug_info.begin();
		di_hash_table::const_iterator end = m_debug_info.end();
		size_t num_leaks = 0;
		size_t num_bytes = 0;
		for(; it != end; ++it)
		{
			const debug_info &info = it->second;
			core::snprintf(buf, BufLen, "%d byte leak @ 0x%x\n", info.size, info.address);
			debug::write(buf);
			++num_leaks;
			num_bytes += info.size;
			for(int i = 0; i < debug_info::MaxStackDepth && info.stack[i]; ++i)
			{
				char name_buf[debug::SymbolStringMaxLength];
				char file_buf[debug::SymbolStringMaxLength];
				int line = 0;
				if(debug::convert_address_to_function(info.stack[i], name_buf, file_buf, &line))
				{
					core::snprintf(buf, BufLen, "\t\t%s(%d) : %s\n", file_buf, line, name_buf);
					debug::write(buf);
				}
			}				
		}
		debug::write_ln("----------------------------------------------------------------");
		debug::write_ln("| Memory Report                                                |");
		debug::write_ln("----------------------------------------------------------------");
		debug::write_ln("| Num leaks             : %6d                               |", num_leaks);		
		debug::write_ln("| Bytes still allocated : %6dkb (%5dmb)                   |", num_bytes / 1024, num_bytes / 1024 / 1024);		
		debug::write_ln("| Peak user bytes       : %6dkb (%5dmb)                   |", m_mem_stats.get_peak() / 1024,  m_mem_stats.get_peak() / 1024 / 1024);
		debug::write_ln("| Peak internal bytes   : %6dkb (%5dmb)                   |", m_internal_mem_stats.get_peak() / 1024, m_internal_mem_stats.get_peak() / 1024 / 1024);
		debug::write_ln("----------------------------------------------------------------");
	}

} // end namespace
} // end namespace
