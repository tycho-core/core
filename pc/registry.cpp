//////////////////////////////////////////////////////////////////////////////
// Copyright 2006  Martin Slater
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "core/types.h"
#include "core/platform.h"
#include "registry.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////
using namespace tycho::core;

namespace tycho
{
namespace core
{
namespace pc
{
namespace registry
{
	namespace detail
	{
		// registry types
		// 	REG_BINARY  Binary data in any form. 
		// 	REG_DWORD   32-bit number. 		
		// 	REG_QWORD   64-bit number. 
		// 	REG_DWORD_LITTLE_ENDIAN  32-bit number in little-endian format. This is equivalent to REG_DWORD.
		// 	In little-endian format, a multibyte value is stored in memory from the lowest byte (the "little end") to the highest byte. For example, the value 0x12345678 is stored as (0x78 0x56 0x34 0x12) in little-endian format. 
		// 	Microsoft Windows NT and Microsoft Windows 95 are designed to run on little-endian computer architectures. A user may connect to computers that have big-endian architectures, such as some UNIX systems. 
		// 	REG_QWORD_LITTLE_ENDIAN  A 64-bit number in little-endian format. This is equivalent to REG_QWORD. 
		// 	REG_DWORD_BIG_ENDIAN   	32-bit number in big-endian format.
		// 	In big-endian format, a multibyte value is stored in memory from the highest byte (the "big end") to the lowest byte. For example, the value 0x12345678 is stored as (0x12 0x34 0x56 0x78) in big-endian format. 
		// 	REG_EXPAND_SZ Null-terminated string that contains unexpanded references to environment variables (for example, "%PATH%"). It will be a Unicode or ANSI string, depending on whether you use the Unicode or ANSI functions. 
		// 	REG_LINK Unicode symbolic link. 
		// 	REG_MULTI_SZ Array of null-terminated strings that are terminated by two null characters. 
		// 	REG_NONE  No defined value type. 
		// 	REG_RESOURCE_LIST Device-driver resource list. 
		// 	REG_SZ Null-terminated string. It will be a Unicode or ANSI string, depending on whether you use the Unicode or ANSI functions. 

		/// REG_DWORD
		bool reg_get_value(HKEY key, const std::wstring &value, uint32 &out)
		{
			DWORD len = sizeof(uint32);
			DWORD type = REG_DWORD;
			LONG result = ::RegQueryValueExW(key, value.c_str(), NULL, &type, (LPBYTE)&out, &len);
			TYCHO_ASSERT(len == 4);
			if(result != ERROR_SUCCESS || type != REG_DWORD)
				return false;
			return true;
		}

		/// REG QWORD
		bool reg_get_value(HKEY key, const std::wstring &value, uint64 &out)
		{
			DWORD len = sizeof(uint64);
			DWORD type = REG_QWORD;
			LONG result = ::RegQueryValueExW(key, value.c_str(), NULL, &type, (LPBYTE)&out, &len);
			TYCHO_ASSERT(len == 8);
			if(result != ERROR_SUCCESS || type != REG_QWORD)
				return false;
			return true;
		}

		/// REG_SZ - wide character
		bool reg_get_value(HKEY key, const std::wstring &value, std::wstring &out)
		{
 			DWORD len = 0;
			DWORD type;
 			LONG result = ::RegQueryValueExW(key, value.c_str(), NULL, &type, NULL, &len);
			if(result != ERROR_SUCCESS || len == 0 || type != REG_SZ)
				return false;
			out.resize(len/2);
			result = ::RegQueryValueExW(key, value.c_str(), NULL, &type, (LPBYTE)&out[0], &len);				
			out.erase(--out.end()); // remove extra null terminator 
			return (result == ERROR_SUCCESS);
		}			

		/// REG_MULTI_SZ - wide character
		bool reg_get_value(HKEY key, const std::wstring &value, std::vector<std::wstring> &result)
		{
			return false;
		}

		/// REG_BINARY
		bool reg_get_value(HKEY key, const std::wstring &value, std::vector<uint8> &result)
		{
			return false;
		}

		/// REG_DWORD
		bool reg_set_value(HKEY key, const std::wstring &name, uint32 value)
		{
			LONG result = ::RegSetValueExW(key, name.c_str(), 0, REG_DWORD, (LPBYTE)&value, sizeof(value));
			if(result != ERROR_SUCCESS)
				return false;
			return true;
		}

		/// REG QWORD
		bool reg_set_value(HKEY key, const std::wstring &name, uint64 value)
		{
			LONG result = ::RegSetValueExW(key, name.c_str(), 0, REG_QWORD, (LPBYTE)&value, sizeof(value));
			if(result != ERROR_SUCCESS)
				return false;
			return true;
		}

		/// REG_SZ - wide character
		bool reg_set_value(HKEY key, const std::wstring &name, const std::wstring &value)
		{
			LONG result = ::RegSetValueExW(key, name.c_str(), 0, REG_SZ, (LPBYTE)&value[0], value.length()*2+2);
			if(result != ERROR_SUCCESS)
				return false;
			return true;
		}

		/// REG_MULTI_SZ - wide character
		bool reg_set_value(HKEY key, const std::wstring &name, const std::vector<std::wstring> &value)
		{
			return false;
		}

		/// REG_BINARY
		bool reg_set_value(HKEY key, const std::wstring &name, const std::vector<uint8> &value)
		{
			return false;
		}

	} // end detail namespace

	//////////////////////////////////////////////////////////////////////////////
	// key_holder
	//////////////////////////////////////////////////////////////////////////////

	key_holder::key_holder(HKEY key, bool no_close)
		: m_key(key), m_no_close(no_close) {}

	key_holder::~key_holder()
		{ if(!m_no_close) ::RegCloseKey(m_key); m_key = 0; }

	//////////////////////////////////////////////////////////////////////////////
	// key
	//////////////////////////////////////////////////////////////////////////////

	key::key(const key &parent, const std::wstring &path, bool create)
	{
		if(create)
		{
			HKEY key = 0;
			LONG result = ::RegCreateKeyW(parent.m_key->handle(), path.c_str(), &key);
			if(result != ERROR_SUCCESS)
			{
				std::string err = tycho::core::get_last_error_string();
				std::string msg("Unable to create key : ");
				msg.append(tycho::core::get_last_error_string());
#if TYCHO_NO_EXCEPTIONS
				TYCHO_PANIC(msg.c_str());
#else
				throw exception(msg.c_str());
#endif
			}
			m_key = key_holder_ptr(new key_holder(key, false));
		}
		else
		{
			HKEY key = 0;
			LONG result = ::RegOpenKeyW(parent.m_key->handle(), path.c_str(), &key);
			if(result != ERROR_SUCCESS)
			{
				std::string msg("Unable to open key : ");
				msg.append(tycho::core::get_last_error_string());
#if TYCHO_NO_EXCEPTIONS
				TYCHO_PANIC(msg.c_str());
#else
				throw exception(msg.c_str());
#endif
			}
			m_key = key_holder_ptr(new key_holder(key, false));
		}
	}

	key::key(HKEY key) :
		m_key(new key_holder(key, true))
	{
	}

	// static member functions

	/// create a new registry key. throws registry::exception on failure.
	key key::create(const key &parent, const std::wstring &path)
	{
		return key(parent, path, true);
	}

	/// open an existing registry key. throws registry::exception on failure.
	key key::open(const key &parent, const std::wstring &path)
	{
		return key(parent, path, false);
	}

	/// delete an existing registry key. throws register::exception on failure
	void key::erase(const key &parent, const std::wstring &path)
	{
		parent.erase(path);
	}

	bool key::exists(const key &parent, const std::wstring &path)			
	{
#if TYCHO_EXCEPTIONS
		try 
		{
			open(parent, path);
		}
		catch(const exception &)
		{
			return false;
		}
#else
		open(parent, path);
#endif // TYCHO_EXCEPTIONS
		return true;
	}

	// member functions
	void key::erase(const std::wstring &subkey) const
	{
		if(::RegDeleteKeyW(m_key->handle(), subkey.c_str()) != ERROR_SUCCESS)
		{
#if TYCHO_NO_EXCEPTIONS
			TYCHO_PANIC("Failed to delete key");
#else
			throw exception("Failed to delete key");
#endif
		}
	}

	bool key::get_value(const std::wstring &value, uint32 &out)
	{
		return detail::reg_get_value(m_key->handle(), value, out);
	}

	bool key::get_value(const std::wstring &value, uint64 &out)
	{
		return detail::reg_get_value(m_key->handle(), value, out);
	}

 	bool key::get_value(const std::wstring &value, std::wstring &out)
	{
		return detail::reg_get_value(m_key->handle(), value, out);
	}


 	bool key::get_value(const std::wstring &value, wstring_list &out)
	{
		return detail::reg_get_value(m_key->handle(), value, out);
	}


	bool key::set_value(const std::wstring &value, uint32 out)
	{
		return detail::reg_set_value(m_key->handle(), value, out);
	}

	bool key::set_value(const std::wstring &value, uint64 out)
	{
		return detail::reg_set_value(m_key->handle(), value, out);
	}

 	bool key::set_value(const std::wstring &value, const std::wstring &out)
	{
		return detail::reg_set_value(m_key->handle(), value, out);
	}

 	bool key::set_value(const std::wstring &value, const wstring_list &out)
	{
		return detail::reg_set_value(m_key->handle(), value, out);
	}

	//////////////////////////////////////////////////////////////////////////////
	// free functions
	//////////////////////////////////////////////////////////////////////////////

	key create_predefined_key(HKEY hkey)
	{
		return key(hkey);
	}

	const key& classes_root_key()
	{
		static key crk = create_predefined_key(HKEY_CLASSES_ROOT);
		return crk;			
	}

	const key& current_config_key()
	{
		static key crk = create_predefined_key(HKEY_CURRENT_CONFIG);
		return crk;			
	}

	const key& current_user_key()
	{
		static key crk = create_predefined_key(HKEY_CURRENT_USER);
		return crk;			
	}

	const key& local_machine_key()
	{
		static key crk = create_predefined_key(HKEY_LOCAL_MACHINE);
		return crk;			
	}

	const key& users_key()
	{
		static key crk = create_predefined_key(HKEY_USERS);
		return crk;			
	}

} // namespace
} // namespace
} // namespace
} // namespace
