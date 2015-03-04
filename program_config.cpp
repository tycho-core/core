//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Wednesday, 26 March 2008 11:04:57 PM
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "program_config.h"
#include "string.h"
#include "core/debug/assert.h"
#include "core/debug/utilities.h"
#include "core/console.h"
#include "core/string_builder.h"
#include "core/memory.h"
#include <string>
#include <stdlib.h>

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////
namespace tycho
{
namespace core
{

	//------------------------------------------------------------------------------------

	program_config::program_config() :
		m_strings_tail(m_strings),
		m_num_options(0),
		m_num_groups(0),
		m_num_free_params(0)
	{							
	}
				
	//------------------------------------------------------------------------------------

	bool is_long_opt(const char *p)
	{
		if(!*p || !*(p+1))
			return false;

		return (*p == '-' && *(p+1)=='-');
	}
	
	//------------------------------------------------------------------------------------

	bool is_short_opt(const char *p)
	{
		if(!*p)
			return false;
		return (*p == '-');
	}
	
	//------------------------------------------------------------------------------------

	bool read_opt_name(const char *&in_p, char *out_buf, int buf_len)
	{	
		TYCHO_ASSERT(out_buf);
		TYCHO_ASSERT(buf_len);

		
		if(!*in_p)
			return false;
		char *buf = out_buf;		
		const char *p = in_p;			
		while(*p)
		{
			if(core::is_whitespace(*p) || core::is_key(p, '='))
			{
				// all done
				*buf = 0;
				
				break;
			}
			
			if(buf - out_buf >= (buf_len-1))
			{
				// out of buffer space
				*buf = 0;
				break;
			}
			
			*buf++ = *p++;
		}
		
		// check we read at least one valid character
		bool ok = (p > in_p);
		
		// update callers pointer
		*buf = 0;
		in_p = p;					
		return ok;
	}
	
	//------------------------------------------------------------------------------------

	bool read_value(const char *&in_p, char *out_buf, int buf_len)
	{	
		TYCHO_ASSERT(out_buf);
		TYCHO_ASSERT(buf_len);
		
		if(!*in_p)
			return false;
		char *buf = out_buf;		
		const char *p = in_p;			
		while(*p)
		{
			if(core::is_whitespace(*p))
			{
				// all done
				*buf = 0;
				
				break;
			}
			
			if(buf - out_buf >= (buf_len-1))
			{
				// out of buffer space
				*buf = 0;
				break;
			}
			
			*buf++ = *p++;
		}
		
		// check we read at least one valid character
		bool ok = (p > in_p);
		
		// update callers pointer
		*buf = 0;
		in_p = p;					
		return ok;
	}			


	
	//------------------------------------------------------------------------------------
	
	/// reads a free parameter from the passed buffer. if the parameter
	/// is quoted then all spaces in the quote are treated as part of the parameter,
	/// the quotes are removed.
	bool read_free(const char *&in_p, char *out_buf, int buf_len)
	{	
		TYCHO_ASSERT(out_buf);
		TYCHO_ASSERT(buf_len);
		
		if(!*in_p)
			return false;
		char *buf = out_buf;		
		const char *p = in_p;
		bool quoted = *p == '"';
		if(quoted)
			++p;
		while(*p)
		{
			if(quoted && *p == '"')			
			{
				// reached the end
				++p;
				*buf = 0;
				eat_whitespace(p);
				break;
			}
			else if(!quoted && core::is_whitespace(*p))
			{
				*buf = 0;
				eat_whitespace(p);
				break;
			}
			else
			{
				*buf++ = *p++;
			}
		}

		// check we read at least one valid character
		bool ok = (p > in_p);
		
		// update callers pointer
		*buf = 0;
		in_p = p;					
		return ok;
	}
			
	//------------------------------------------------------------------------------------
			
	bool validate_int(const char *p)
	{
		if(!p || !*p)
			return false;
			
		if(*p == '-' || *p == '+')
			++p;
			
		while(*p)
		{
			if(!core::is_numeric(*p))
			{
				return false;
			}
			++p;
		}
		
		return true;
	}
	
	//------------------------------------------------------------------------------------

	const char* program_config::add_string(const char *str)
	{
		if(str == 0 || *str == 0 || m_strings_tail == nullptr)
			return 0;
			
		const char *cpy = m_strings_tail;
		int len = core::strlen(str);
		if((m_strings_tail - m_strings) + len >= buffer_space_remaining())
			return 0;
			
		core::strncpy(m_strings_tail, str, buffer_space_remaining());
		m_strings_tail += len+1;			
		return cpy;
	}
	
	//------------------------------------------------------------------------------------

	void program_config::parse_command_line(const char *cmd_line)
	{
		TYCHO_ASSERT(cmd_line);
		
		// parse all options from the command line
		const int BufLen = 256;
		const char *p = cmd_line;
		char long_opt_buf[BufLen] = { 0 };
		char value_buf[BufLen] = { 0 };
		bool parsing_free = false;

		// first argument is the program name, just consume it
		if(!read_free(p, long_opt_buf, BufLen))
		{
			// error
			return;
		}

		while(*p)
		{
			*long_opt_buf = 0;
			*value_buf = 0;
			
			// consume whitespace
			while(core::is_whitespace(*p))
				++p;
				
			if(!*p)
				break;
				
			// see what we got, long opt, short opt or free opt
			char short_opt = 0;
			if(is_long_opt(p))											
			{
				p += 2; // step over '--'
				if(!read_opt_name(p, long_opt_buf, BufLen))
				{
					return; // error
				}
			}
			else if(is_short_opt(p))
			{
				++p; // step over '-'
				short_opt = *p++;
			}
			else 
			{
				// free option
			}
			
			if(long_opt_buf[0] != 0 || short_opt)
			{
				if(parsing_free)
				{
					// error, no options allowed after first free parameter
					return;
				}
				
				// check for assignment
				if(core::is_key(p, '='))
				{
					++p;
					if(!read_value(p, value_buf, BufLen))
					{
						return; // error
					}
				}
				else
				{
					// swallow whitespace
					eat_whitespace(p);
				}
				

				// store the option
				m_options[m_num_options].short_name = short_opt;
				m_options[m_num_options].long_name  = add_string(long_opt_buf);
				m_options[m_num_options].value      = add_string(value_buf);
				++m_num_options;
			}
			else
			{
				// start of free parameters, any options after here are flagged as errors
				const int BufLen = 256;
				char buf[BufLen] = { 0 };
				if(!read_free(p, buf, BufLen))
				{
					return; // error
				}
				
				if(m_num_free_params == MaxFreeParams)
					return; // error
					
				m_free_params[m_num_free_params++] = add_string(buf);
			}
		}
	}
	
	//------------------------------------------------------------------------------------
	
	bool program_config::set_value(value &v, const char *str)
	{						
		// we got a winner, make sure its value is correct for its type
		if(v.type == ot_bool)
		{
			// if no value the implicitly true
			if(!str)
			{
				*v.data.vbool = true;
				return true;
			}
			
			// should be "1" or "0" if specified
			if(core::strcmp(str, "1") == 0)
			{
				/// \todo this allows "1" but also "1asa" etc
				*v.data.vbool = true;
				return true;
			}
			if(core::strcmp(str, "0") == 0)
			{
				/// \todo this allows "0" but also "0asa" etc
				*v.data.vbool = false;
				return true;
			}
			
			return false;
		}
		if(v.type == ot_int)
		{
			// error if no value specified
			if(!str)
			{										
				return false;
			}
			if(!validate_int(str))
			{
				return false;
			}
			
			*v.data.vint = ::
atoi(str);
			return true;
		}					
		if(v.type == ot_string)			
		{
			if(!str)
			{
				*v.data.vstr = 0;	
				return false;
			}
			
			*v.data.vstr = str;
			return true;
		}
		if(v.type == ot_float)			
		{
			if(!str)
			{
				return false;
			}
			
			*v.data.vfloat = (float)atof(str);
			return true;
		}					
		
		return false;
	}

	//------------------------------------------------------------------------------------

	void program_config::process_option_group(const char* group_name, const char* group_desc, option_pair *options)
	{
		TYCHO_ASSERT(group_name);
		TYCHO_ASSERT(options);
		
		int group_name_len = core::strlen(group_name);
		// remember this group
		m_groups[m_num_groups].group_name = group_name;
		m_groups[m_num_groups].group_desc = group_desc;
		m_groups[m_num_groups].options = options;
		++m_num_groups;
					
		// scan for any options in this group
		while(options->o.long_name != 0 || options->o.short_name != 0)
		{
			// search for an option of this name by short or long name
			bool found = false;
			for(int i = 0; i < m_num_options; ++i)
			{
				bool long_match = false;
				if(options->o.long_name && m_options[i].long_name)
				{
					// check its the correct group
					if(core::strncmp(m_options[i].long_name, group_name, group_name_len) == 0)
					{
						const char *p = m_options[i].long_name;
						p += group_name_len;
						if(!*p || *p != '.')
							break; //error
						++p;								
						long_match = core::strcmp(options->o.long_name, p) == 0;
					}
				}
				
				bool short_match = false;							
				if(options->o.short_name && m_options[i].short_name)
					short_match = options->o.short_name == m_options[i].short_name;
										
				const char *value = 0;
				if(long_match || short_match)
					value = m_options[i].value;
				else if(options->o.default_val)
					value = options->o.default_val;
				
				if(long_match || short_match)
				{
					set_value(options->v, value);					
					found = true;
					break;					
				}					
			}
			
			// option wasn't set by user so set a default if it has one
			if(!found && options->o.default_val)
				set_value(options->v, options->o.default_val);
			
			// next one
			++options;
		}
	}

	//------------------------------------------------------------------------------------

	program_config::option_pair program_config::null_opt()
	{
		static option_pair new_pair = { { 0 }, { { 0 } } };
		return new_pair;
	}

	//------------------------------------------------------------------------------------

	program_config::option_pair program_config::opt_base(const char *long_name, char short_name, const char *desc, const char *def_value)		
	{
		option_pair new_pair;
		new_pair.o.long_name = long_name;
		new_pair.o.short_name = short_name;
		new_pair.o.description = desc;
		new_pair.o.default_val = def_value;
		return new_pair;
	}

	//------------------------------------------------------------------------------------

	program_config::option_pair program_config::opt(const char *long_name, char short_name, const char *desc, const char *def_value, int *dst)
	{
		option_pair new_pair = opt_base(long_name, short_name, desc, def_value);
		new_pair.o.type = ot_int;
		new_pair.v.type = ot_int;
		new_pair.v.data.vint = dst;
		return new_pair;
	}
	
	//------------------------------------------------------------------------------------	

	program_config::option_pair program_config::opt(const char *long_name, char short_name, const char *desc, const char *def_value, float *dst)
	{
		option_pair new_pair = opt_base(long_name, short_name, desc, def_value);
		new_pair.o.type = ot_float;
		new_pair.v.type = ot_float;
		new_pair.v.data.vfloat = dst;
		return new_pair;
	}

	//------------------------------------------------------------------------------------

	program_config::option_pair program_config::opt(const char *long_name, char short_name, const char *desc, const char *def_value, bool *dst)
	{
		option_pair new_pair = opt_base(long_name, short_name, desc, def_value);
		new_pair.o.type = ot_bool;
		new_pair.v.type = ot_bool;
		new_pair.v.data.vbool = dst;
		return new_pair;
	}

	//------------------------------------------------------------------------------------

	program_config::option_pair program_config::opt(const char *long_name, char short_name, const char *desc, const char *def_value, const char **dst)
	{
		option_pair new_pair = opt_base(long_name, short_name, desc, def_value);
		new_pair.o.type = ot_string;
		new_pair.v.type = ot_string;
		new_pair.v.data.vstr = dst;
		return new_pair;
	}

	//------------------------------------------------------------------------------------

	void program_config::print_help()
	{
		const int BufLen = 512;
		core::string_builder builder(reinterpret_cast<char*>(TYCHO_ALLOCA(BufLen)), BufLen);
		for(int i = 0; i < m_num_groups; ++i)
		{	
			const stored_group &g = m_groups[i];

			// 2 passes to print each group, first finds the longest option string
			// so we can use that to format the descriptions cleanly to the right, then
			// the second prints the information.
			int max_name = 0;
			for(const option_pair *o = g.options; o->o.long_name || o->o.short_name; ++o)
			{
				if(o->o.long_name)					
					builder.append("\t--%s.%s", g.group_name, o->o.long_name);						
				if(o->o.short_name)
					builder.append(" (-%c)", o->o.short_name);
				if(o->o.default_val)
					builder.append(" = %s", o->o.default_val);	
				if(builder.length() > max_name)
					max_name = builder.length();		
				builder.clear();
			}
			
			// if no max name then no options
			if(max_name)
			{			
				console::write_ln(g.group_desc);
				console::write_ln("------------------------------------------");
				for(const option_pair *o = g.options; o->o.long_name || o->o.short_name; ++o)
				{
					if(o->o.long_name)					
						builder.append("\t--%s.%s", g.group_name, o->o.long_name);						
					if(o->o.short_name)
						builder.append(" (-%c)", o->o.short_name);
					if(o->o.default_val)
						builder.append(" = %s", o->o.default_val);	
					if(builder.length() > max_name)
						max_name = builder.length();		
					console::write(builder.string());
					for(int i = builder.length(); i < max_name; ++i)
						console::write(" ");
					console::write(" : %s\n", o->o.description);
					builder.clear();
				}
				console::write_ln("");
			}
		}
	}

	//------------------------------------------------------------------------------------

} // end namespace
} // end namespace
