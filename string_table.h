//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Tuesday, 6 October 2009 2:45:44 PM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __STRING_TABLE_H_68722096_6619_475D_BF39_D0DFB34B75E9_
#define __STRING_TABLE_H_68722096_6619_475D_BF39_D0DFB34B75E9_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "core/core_abi.h"
#include <vector>
#include <string>

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace tycho
{
namespace core
{

	/// maintains unique list of strings
    class CORE_ABI string_table
    {
    public:
		typedef core::uint16 index_t;
		static const int invalid_index = 0xffff;
		static const int max_index = 0xfffe;

		/// add a string to the table
		index_t add(const char*);

		/// get the string stored at index i.
		const char* get(index_t) const;

		/// find the string 
		index_t find(const char*) const;

		/// increase the ref count on the passed string
		void add_ref(index_t);

		/// decrease the ref count on the passed string. will delete when
		/// it reaches zero.
		void dec_ref(index_t);

    private:
		struct string_hdr
		{
			core::uint16 ref_count;
			std::string string;
		};

		std::vector<string_hdr*> m_strings;
    };

} // end namespace
} // end namespace

#endif // __STRING_TABLE_H_68722096_6619_475D_BF39_D0DFB34B75E9_
