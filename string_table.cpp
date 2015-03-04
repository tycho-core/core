//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Tuesday, 6 October 2009 2:45:44 PM
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "string_table.h"
#include "core/debug/assert.h"
#include "core/string.h"
#include "core/memory.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////
namespace tycho
{
namespace core
{

    //--------------------------------------------------------------------

	string_table::index_t string_table::add(const char* str)
	{
		index_t i = find(str);
		if(i != invalid_index)
			return i;
		string_hdr* hdr = new string_hdr();
		hdr->ref_count = 1;
		hdr->string = str;
		m_strings.push_back(hdr);
		TYCHO_ASSERT((int)m_strings.size() < (int)max_index);
		return (index_t)m_strings.size();
	}

    //--------------------------------------------------------------------

	const char* string_table::get(index_t i) const
	{
		TYCHO_ASSERT((int)i < (int)m_strings.size());
		TYCHO_ASSERT(m_strings[i]);
		return m_strings[i]->string.c_str();
	}

    //--------------------------------------------------------------------

	string_table::index_t string_table::find(const char* str) const
	{
		//TODO : optimise
		for(size_t i = 0; i < m_strings.size(); ++i)
		{
			if(m_strings[i] && core::strcmp(str, m_strings[i]->string.c_str()) == 0)
				return (index_t)i;
		}
		return invalid_index;
	}

    //--------------------------------------------------------------------

	void string_table::add_ref(index_t i)
	{
		TYCHO_ASSERT((int)i < (int)m_strings.size());
		TYCHO_ASSERT(m_strings[i]);
		++m_strings[i]->ref_count;
	}

    //--------------------------------------------------------------------

	void string_table::dec_ref(index_t i)
	{
		TYCHO_ASSERT((int)i < (int)m_strings.size());
		TYCHO_ASSERT(m_strings[i]);
		--m_strings[i]->ref_count;
		if(m_strings[i]->ref_count == 1)
		{
			core::safe_delete(m_strings[i]);

			// add to free list
		}
	}

    //--------------------------------------------------------------------

} // end namespace
} // end namespace
