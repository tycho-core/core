//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Tuesday, 27 May 2008 9:49:47 PM
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "core/pc/safe_windows.h"
#include "utilities.h"
#include "core/string.h"
#include <direct.h>

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////
namespace tycho
{
namespace core
{

	//------------------------------------------------------------------------------------

	CORE_ABI std::string temp_dir()
	{
		char buffer[MAX_PATH+1];
		if(FAILED(::GetTempPath(MAX_PATH, buffer)))
			return "";
		return buffer;
	}

	//------------------------------------------------------------------------------------

	CORE_ABI std::string app_data_dir(const char* app_name)
	{
		char buffer[MAX_PATH+1];
		if(FAILED(::SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, SHGFP_TYPE_CURRENT, &buffer[0])))
			return "";
		core::strrep(buffer, '\\', '/');
		return std::string(buffer) + "/" + app_name + "/";	
	}

	//------------------------------------------------------------------------------------

	CORE_ABI std::string save_game_dir(const char* app_name)
	{
		return app_data_dir(app_name) + "save_games/";
	}
		
	//------------------------------------------------------------------------------------
		
	CORE_ABI std::string current_working_directory()
	{
        char buffer[MAX_PATH];        
        _getcwd(buffer, MAX_PATH);    
		core::strrep(buffer, '\\', '/');
        return std::string(buffer) + "/"; 	
	}

	//------------------------------------------------------------------------------------
	
} // end namespace
} // end namespace
