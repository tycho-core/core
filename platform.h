//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Saturday, 29 March 2008 7:47:14 PM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __PLATFORM_H_A6C707AA_17B5_4860_9E87_63723AD54033_
#define __PLATFORM_H_A6C707AA_17B5_4860_9E87_63723AD54033_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "core/core_abi.h"
#include <string>

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace tycho
{
namespace core
{
	enum os_allocation_flags
	{
		mem_commit		= 0x1000,	///< Allocates physical storage in memory or in the paging file on disk 
		mem_reserve     = 0x2000,	///< Reserves a range of the process's virtual address space
		mem_top_down    = 0x100000	///< Allocates memory at the highest possible address. 
	};


	/// information about a os memory allocation
	struct alloc_info
	{
		void*	base_address;
		void*	allocation_base;
		uint64	state;
		uint64  region_size;
	};

	/// memory allocation
	/// \returns 0 to signify growable memory (ala win32) or number of 
	/// bytes available in restricted memory cases (on consoles)
	CORE_ABI size_t get_max_os_mem_size();

	/// allocate size bytes from the os
	CORE_ABI void* get_os_mem(size_t size, int flags = mem_commit);

	/// free an os allocation
	CORE_ABI bool free_os_mem(void* memory);

	/// fills out an alloc info structure for the provided pointer
	CORE_ABI bool get_os_alloc_info(void* memory, alloc_info& info);

	/// memory init info
	CORE_ABI size_t memory_granularity();
	CORE_ABI size_t memory_page_size();

	/// create a directory
	CORE_ABI bool create_directory(const char*);

	/// create directory and all its parents
	CORE_ABI bool create_directories(const char*);

	/// makes sure the directory exists, creating if it does not
	CORE_ABI bool ensure_directory_exists(const char*);

	/// \returns true if the passed path points to an existing directory
	CORE_ABI bool directory_exists(const char*);
    
    /// \returns The absolute path to the user's temporary directory
	CORE_ABI std::string temp_dir();
	
	/// \returns The absolute path to the users application data directory
	CORE_ABI std::string app_data_dir(const char* app_name);
	
	/// \returns The absolute path to the directory to put save game date
	CORE_ABI std::string save_game_dir(const char* app_name);
    
	/// \returns the current working directory
	CORE_ABI std::string current_working_directory();

	/// \returns A string representation of the last os error
	CORE_ABI std::string get_last_error_string();

    /// set the name of thread
	CORE_ABI void name_thread(size_t thread_id, const char *name); 

	/// set the name of the current thread
	CORE_ABI void name_current_thread(const char *name); 

} // end namespace
} // end namespace

#endif // __PLATFORM_H_A6C707AA_17B5_4860_9E87_63723AD54033_
