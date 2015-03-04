//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Wednesday, 26 March 2008 11:04:57 PM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __PROGRAM_CONFIG_H_13DB47DD_D170_468F_9086_5F47454FE6C5_
#define __PROGRAM_CONFIG_H_13DB47DD_D170_468F_9086_5F47454FE6C5_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "core/core_abi.h"
#include "core/debug/assert.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace tycho
{
namespace core
{

	/// Program configuration access. Configuration options can be retrieved
	/// from either the command line or a config file. This is intended to 
	/// be able to be used for all library and program configuration, this means
	/// it cannot do any memory allocation for use at all as it may be queried 
	/// before the memory manager is initialised. It also means that options are parsed 
	/// in groups as required rather than in a single hit.
	/// 
	/// <pre>
	/// Format:
	///	    --group.name=foo	- long name with value
	///	    --group.name		- long name with no value, only valid for boolean types
	///		-name=foo			- short name with value. note group name is not specified
	///		-name				- short name with no value. only valid for boolean types
	///		free1 free2			- trailing unnamed free options, this may be quoted to
	/// </pre>	
	class CORE_ABI program_config
    {
    public:
		/// type of config option
		enum option_type
		{
			ot_string,
			ot_float,
			ot_int,
			ot_bool
		};
    
		/// single config option
		struct option
		{
			const char* long_name;		
			char		short_name;	
			const char* description;
			const char* default_val;
			option_type type;
		};
		
		/// value type
		struct value
		{
			union data_
			{
				const char **vstr;
				int		   *vint;
				float	   *vfloat;
				bool	   *vbool;
			} data;
			option_type type;
		};			        
		
		/// option / value pair
		struct option_pair
		{
			option o;
			value  v;
		};
		
					
	public:
		/// constructor
		program_config();
		
		/// parse an options from a command line
		void parse_command_line(const char *cmd_line);
		
		/// process the option group parsed in
		void process_option_group(const char* group_name, const char* group_desc, option_pair *options);
		
		/// print help text
		void print_help();
		
		/// get the number of free params available
		int get_num_free_params()
			{ return m_num_free_params; }
			
		/// \returns the i'th free parameter				
		const char* get_free_param(int i)
			{ TYCHO_ASSERT(i < m_num_free_params); return m_free_params[i]; }

		/// create a terminating null option
		static option_pair null_opt();

		/// create a new integer option
		static option_pair opt(const char *long_name, char short_name, const char *desc, const char *def_value, int *dst);

		/// create a new float option
		static option_pair opt(const char *long_name, char short_name, const char *desc, const char *def_value, float *dst);

		/// create a new bool flag option
		static option_pair opt(const char *long_name, char short_name, const char *desc, const char *def_value, bool *dst);
		
		/// create a new string option
		static option_pair opt(const char *long_name, char short_name, const char *desc, const char *def_value, const char **dst);			

	private:
		///	fills in all common option_pair members
		static option_pair opt_base(const char *long_name, char short_name, const char *desc, const char *def_value);
		
		bool set_value(value &v, const char *str);

	private:		
		/// \returns the space remaining in the string buffer
		int buffer_space_remaining()
		 { return StringBufferSize - (m_strings_tail - m_strings); }
		 
		 /// add a string to the string buffer
		const char* add_string(const char *);
		
    private:
		/// option that has been read from the command line
		struct stored_option
		{
			const char *group;
			const char *long_name;
			char  short_name;
			const char *value;
		};
		
		/// group that has been processed, stored for generating help later.
		struct stored_group
		{
			const char*	 group_name;
			const char*  group_desc;
			option_pair* options;
		};
		
		static const int MaxOptions = 64;
		static const int MaxGroups = 32;
		static const int MaxFreeParams = 32;
		static const int StringBufferSize = 2048; /// 2k buffer for storing strings
					
		stored_option m_options[MaxOptions];
		stored_group  m_groups[MaxGroups];
		char		  m_strings[StringBufferSize];			
		const char*	  m_free_params[MaxFreeParams];
		char*		  m_strings_tail;
		int			  m_num_options;
		int			  m_num_groups;
		int			  m_num_free_params;
    };

} // end namespace
} // end namespace

#endif // __PROGRAM_CONFIG_H_13DB47DD_D170_468F_9086_5F47454FE6C5_
