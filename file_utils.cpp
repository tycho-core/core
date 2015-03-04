//////////////////////////////////////////////////////////////////////////////
// Copyright 2006  Martin Slater
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "file_utils.h"
#include <stdio.h>

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace tycho
{
namespace core
{

#ifdef TYCHO_PC

	namespace detail
	{
		template<class T>
		bool read_entire_file(FILE *f, T &cont)
		{
			if(!f)			
				return false;
			
			fseek(f, 0L, SEEK_END);
			unsigned long len = ftell(f);
			fseek(f, 0L, SEEK_SET);
			
			cont.resize(len);
			fread(&cont[0], 1, len, f);
			
			fclose(f);
			
			return true;	
		}
	}

	CORE_ABI bool read_entire_file(const char *filename, std::vector<unsigned char> &data)
	{
		return detail::read_entire_file(fopen(filename, "r+b"), data);		
	}

	CORE_ABI bool read_entire_file(const char *filename, std::string &data)
	{
		return detail::read_entire_file(fopen(filename, "r+b"), data);
	}

	CORE_ABI bool is_relative_path(const char *p)
	{
		return !p[1] || p[1] != ':';
	}

#endif // TYCHO_PC

} // end namespace
} // end namespace
