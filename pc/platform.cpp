//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Saturday, 29 March 2008 7:46:49 PM
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "core/platform.h"
#include "core/string.h"
#include "core/memory.h"
#include "core/pc/safe_windows.h"
#include "boost/format.hpp"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////
namespace tycho
{
namespace core
{

	//------------------------------------------------------------------------------------

	CORE_ABI size_t memory_granularity()
	{
		SYSTEM_INFO system_info;
		GetSystemInfo(&system_info);
		return system_info.dwAllocationGranularity;
	}

	//------------------------------------------------------------------------------------

	CORE_ABI size_t memory_page_size()
	{
		SYSTEM_INFO system_info;
		GetSystemInfo(&system_info);
		return system_info.dwPageSize;
	}

	//------------------------------------------------------------------------------------

	CORE_ABI size_t get_max_os_mem_size()
	{
		return 0;
	}

	//------------------------------------------------------------------------------------

	CORE_ABI void* get_os_mem(size_t size, int flags)
	{
		return VirtualAlloc(0, size, flags, PAGE_READWRITE);
	}

	//------------------------------------------------------------------------------------

	CORE_ABI bool free_os_mem(void* memory)
	{
		return VirtualFree(memory, 0, MEM_RELEASE) != 0; 
	}

	//------------------------------------------------------------------------------------

	CORE_ABI bool get_os_alloc_info(void* memory, alloc_info& info)
	{
		MEMORY_BASIC_INFORMATION minfo;
		char* cptr = reinterpret_cast<char*>(memory);
	
		if (::VirtualQuery(cptr, &minfo, sizeof(minfo)) != 0)
		{
			info.allocation_base = minfo.AllocationBase;
			info.base_address	 = minfo.BaseAddress;
			info.region_size	 = minfo.RegionSize;
			info.state			 = minfo.State;
			return true;
		}

		return false;
	}
	
	//------------------------------------------------------------------------------------

	static bool is_valid_path_char(char c)
	{
		// invalid characters in a path segment
		static const char invalid_path_chars[] =
			"\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x0E\x0F"
			"\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1A\x1B\x1C\x1D\x1E\x1F"
			"<>:\"/\\|";

		return core::strchr(invalid_path_chars, c) == 0;
	}

	//------------------------------------------------------------------------------------

	/// split a path int the sections <volume>:/<path>/<path>/...
	/// this modifies the passed path to insert null terminators 
	/// after each section and returns pointers into the string
	/// to the different sections.
	static int splitpath_modify(char *path, char *segments[], int max_segments, bool& path_is_dir)
	{
		int num_segments = 0;

		// find volume
		char*colon = strchr(path, ':');

		if(!colon)
			return 0;

		*colon  = '\0';
		segments[num_segments++] = path;
		path = colon+1;

		if(*path == '\0')
			return num_segments;

		if(*path != '/')
			return 0;

		++path;
		
		// walk path to separate path from basename.
		// if path ends in / then basename will be null
		// if the path is
		path_is_dir = false;
		while(*path)
		{
			segments[num_segments++] = path;
			char *end = path;
			
			while(*end && *end != L'/')
			{
				if(!is_valid_path_char(*end))
					return 0;
				++end;
			}
			
			if(*end)
			{
				if(*(end+1))
					*end = '\0';
				else
				{
					// path ends in slash so explicitly identifies a directory
					path_is_dir = true;
					*end = '\0';
					break;
				}
			}
			else
				break;

			path = end+1;
		}

		return num_segments;
	}		  

	//------------------------------------------------------------------------------------

	CORE_ABI bool create_directory(const char* p)
	{
		if(directory_exists(p))
			return false;

		return ::CreateDirectory(p, NULL) == TRUE;
	}	

	//------------------------------------------------------------------------------------

	CORE_ABI bool create_directories(const char* p)
	{
		// make sure path is not too long 
		int plen = core::strlen(p);
		if(plen >= MAX_PATH)
			return false;

		// check we contain a volume identifier
		if(core::strchr(p, ':') == 0)
			return false;

		// copy path as we will modify it
		char buf[MAX_PATH] = { 0 };
		core::strcpy(buf, p);

		// split path into components
		bool is_dir;
		const int MaxSegments = 32;
		char* segments[MaxSegments] = { 0 };
		int num_segments = splitpath_modify(buf, segments, MaxSegments, is_dir);
		if(num_segments == 0)
			return false;

		// create all directories
		char cur_dir[MAX_PATH] = { 0 };
		core::strcpy(cur_dir, segments[0]);
		core::strcat(cur_dir, ":");
		for(int i = 1; i < num_segments; ++i)
		{
			core::strcat(cur_dir, "/");
			core::strcat(cur_dir, segments[i]);

			// check to see if the directory already exists
			if(!directory_exists(cur_dir))
			{
				if(!create_directory(cur_dir))
					return false;
			}
		}

		return true;
	}

	//------------------------------------------------------------------------------------

	CORE_ABI bool directory_exists(const char* dir)
	{
		WIN32_FIND_DATAA find_info;
		core::mem_zero(find_info);
		HANDLE h = ::FindFirstFileA(dir, &find_info);
		if(h == INVALID_HANDLE_VALUE)
			return false;
		::FindClose(h);
		return (find_info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) > 0;
	}

	//------------------------------------------------------------------------------------

	CORE_ABI std::string get_last_error_string()
	{
        LPTSTR buffer = 0;
        DWORD last_error = ::GetLastError();

		::FormatMessage(
            FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM,
            NULL,
            last_error,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPTSTR)&buffer,
            0, NULL);

		if(buffer) 
		{
			std::string result = (boost::format("%d : %s") % last_error % buffer).str();
			::LocalFree(buffer);			
			return result;
		}
		
		return "";			
	} 

    //------------------------------------------------------------------------------------

	CORE_ABI void name_thread(size_t thread_id, const char *name)
	{
#if TYCHO_EXCEPTIONS
		TYCHO_ASSERT(name);

		const unsigned int MS_VC_EXCEPTION = 0x406D1388;

		struct THREADNAME_INFO
		{
			DWORD dwType; // Must be 0x1000.
			LPCSTR szName; // Pointer to name (in user addr space).
			DWORD dwThreadID; // Thread ID (-1=caller thread).
			DWORD dwFlags; // Reserved for future use, must be zero.
		};

	   THREADNAME_INFO info;
	   info.dwType = 0x1000;
	   info.szName = name;
	   info.dwThreadID = static_cast<DWORD>(thread_id);
	   info.dwFlags = 0;

	   __try
	   {
			RaiseException(MS_VC_EXCEPTION, 0, sizeof(info)/sizeof(DWORD), (DWORD*)&info);
	   }
	   __except(EXCEPTION_CONTINUE_EXECUTION)
	   {
	   }
#endif // TYCHO_EXCEPTIONS
	}

    //------------------------------------------------------------------------------------

	CORE_ABI void name_current_thread(const char *name)
	{
		name_thread(static_cast<size_t>(::GetCurrentThreadId()), name);
	} 

    //------------------------------------------------------------------------------------

} // end namespace
} // end namespace
