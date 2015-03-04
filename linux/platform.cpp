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
#include "core/console.h"
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

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
	    //TODO
	    return 0;
	}

	//------------------------------------------------------------------------------------

	CORE_ABI size_t memory_page_size()
	{
	    //TODO
	    return 0;
	}

	//------------------------------------------------------------------------------------

	CORE_ABI size_t get_max_os_mem_size()
	{
		return 0;
	}

	//------------------------------------------------------------------------------------

	CORE_ABI void* get_os_mem(size_t size, int flags)
	{
	    return 0; 
	}

	//------------------------------------------------------------------------------------

	CORE_ABI bool free_os_mem(void* memory)
	{
	    return false; 
	}

	//------------------------------------------------------------------------------------

	CORE_ABI bool get_os_alloc_info(void* memory, alloc_info& info)
	{
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

	/// split a path int the sections /<path>/<path>/...
	/// this modifies the passed path to insert null terminators 
	/// after each section and returns pointers into the string
	/// to the different sections.
	static int splitpath_modify(char *path, char *segments[], int max_segments, bool& path_is_dir)
	{
		int num_segments = 0;

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

	CORE_ABI bool create_directory(const char* dir)
	{
		return ::mkdir(dir, 0755) == 0;
	}

	//------------------------------------------------------------------------------------

	CORE_ABI bool create_directories(const char* p)
	{
		// make sure path is not too long 
		int plen = core::strlen(p);
		if(plen >= FILENAME_MAX)
			return false;

		// copy path as we will modify it
		char buf[FILENAME_MAX] = { 0 };
		core::strcpy(buf, p);

		// split path into components
		bool is_dir;
		const int MaxSegments = 32;
		char* segments[MaxSegments] = { 0 };
		int num_segments = splitpath_modify(buf, segments, MaxSegments, is_dir);
		if(num_segments == 0)
			return false;

		// create all directories
		char cur_dir[FILENAME_MAX] = { 0 };
		for(int i = 0; i < num_segments; ++i)
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
		struct stat s;
		if(::stat(dir, &s) == -1)
			return false;
		return S_ISDIR(s.st_mode) != 0;
	}

	//------------------------------------------------------------------------------------

    CORE_ABI std::string get_last_error_string()
    {
        return "Linux get_last_error_string() not implemented";
    }

	//------------------------------------------------------------------------------------

	CORE_ABI void name_thread(size_t thread_id, const char *name)
    {
       //TODO : implement
    }

	//------------------------------------------------------------------------------------

    CORE_ABI void name_current_thread(const char *name)
    {
       //TODO : implement
    }


	//------------------------------------------------------------------------------------

} // end namespace
} // end namespace
