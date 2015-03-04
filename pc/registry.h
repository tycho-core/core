//////////////////////////////////////////////////////////////////////////////
// Copyright 2006  Martin Slater
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef REGISTRY_H_MS_2007_1_7_21_17_57_
#define REGISTRY_H_MS_2007_1_7_21_17_57_


//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "core/core_abi.h"
#include "assert.h"
#include "safe_windows.h"
#include "core/types.h"
#include <string>
#include <vector>

//////////////////////////////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace tycho
{
namespace core
{
namespace pc
{
namespace registry
{

	/// exception type thrown for all registry errors
	struct exception : virtual public std::exception
	{
		exception(const char *what) :
			std::exception(what) {}
	};

	/// registry key handle wrapper, closes the handle on destruction
	class key_holder
	{
	public:
		key_holder(HKEY key, bool no_close);
		~key_holder();

		const HKEY handle() const 
			{ return m_key; }

	private:
		HKEY m_key;
		bool m_no_close;
	};
	typedef std::shared_ptr<key_holder> key_holder_ptr;

	/// registry key. These cannot be created directly only through
	/// the \ref open_key , \ref create_key free functions.
	class key
	{
	public:
		typedef std::vector<std::wstring> wstring_list;

		// static functions
		static key  create(const key &parent, const std::wstring &path);
		static key  open(const key &parent, const std::wstring &path);
		static void erase(const key &parent, const std::wstring &path);
		static bool exists(const key &parent, const std::wstring &path);

		void erase(const std::wstring &subkey) const;
		
		bool get_value(const std::wstring &value, core::uint32 &out);
		bool get_value(const std::wstring &value, core::uint64 &out);
		bool get_value(const std::wstring &value, std::wstring &out);
		bool get_value(const std::wstring &value, wstring_list &out);

		bool set_value(const std::wstring &value, core::uint32 out);
		bool set_value(const std::wstring &value, core::uint64 out);
		bool set_value(const std::wstring &value, const std::wstring &out);
		bool set_value(const std::wstring &value, const wstring_list &out);

	private:
		friend key create_predefined_key(HKEY);

	private:
		key(const key &parent, const std::wstring &path, bool create);
		key(HKEY);

	private:
		key_holder_ptr m_key;
	};

	// predefined keys
	const key& classes_root_key();
	const key& current_config_key();
	const key& current_user_key();
	const key& local_machine_key();
	const key& users_key();

} // namespace
} // namespace
} // namespace
} // namespace


#endif  // REGISTRY_H_MS_2007_1_7_21_17_57_

