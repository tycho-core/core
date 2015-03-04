//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Tuesday, 27 May 2008 9:49:47 PM
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "core/console.h"
#include <string>
#include <stdlib.h>

#if TYCHO_OSX
#include <unistd.h>
#endif

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////
namespace tycho
{
namespace core
{

	//------------------------------------------------------------------------------------

	static const char* get_home_dir()
	{
		return ::getenv("HOME");
	}

	//------------------------------------------------------------------------------------

	CORE_ABI std::string temp_dir()
	{
		std::string res(get_home_dir());
		res.append("/.tycho/temp/");
		return res;
	}
	
	//------------------------------------------------------------------------------------

	CORE_ABI std::string app_data_dir(const char* app_name)
	{
		std::string res(get_home_dir());
		res.append("/.tycho/");
	    res.append(app_name);
		res.append("/");
	    return res;
	}

	//------------------------------------------------------------------------------------

	CORE_ABI std::string save_game_dir(const char* app_name)
	{
	    std::string res(get_home_dir());
		res.append("/.tycho/");
	    res.append(app_name);
	    res.append("/save_games/");
	    return res;
	}
		
	//------------------------------------------------------------------------------------
		
	CORE_ABI std::string current_working_directory()
	{
	    char buffer[1024];        
	    ::getcwd(buffer, 1024);
	    return std::string(buffer) + "/"; 	
	}

	//------------------------------------------------------------------------------------
	
} // end namespace
} // end namespace
