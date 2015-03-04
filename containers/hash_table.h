//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Monday, 24 March 2008 6:47:16 PM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __HASH_TABLE_H_9DC674F6_AC57_4470_8CB6_B62F47930AAB_
#define __HASH_TABLE_H_9DC674F6_AC57_4470_8CB6_B62F47930AAB_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "core/core_abi.h"
#include "core/memory.h"
#include "core/memory/allocator.h"
#include "core/memory/new.h"
#include "core/debug/assert.h"
#include "core/containers/list.h"
#include "core/string.h"
#include "core/hash.h"
#include <utility>

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace tycho
{
namespace core
{
	
	template<class T>
	struct compare_equals
	{
		bool operator()(const T &lhs, const T &rhs) const
		{
			return lhs == rhs;
		}
	};

	struct compare_cstr
	{
		bool operator()(const char* lhs, const char* rhs)
		{
			return core::strcmp(lhs, rhs) == 0;
		}
	};

	
	template<class KeyType, 
			 class ValueType,
			 int NumBuckets, 
			 typename Comparer = compare_equals<KeyType>,
			 class Allocator = tycho::core::default_allocator>
    class hash_table
    {
    public:
		typedef hash_table<ValueType, KeyType, NumBuckets, Allocator> this_type;
		typedef Allocator				allocator_type;
		typedef ValueType*				pointer;
		typedef const ValueType*		const_pointer;
		typedef ValueType&              reference;
		typedef const ValueType&        const_reference;

		typedef KeyType                 key_type;
					
		typedef std::pair<const key_type, ValueType>	value_type;

		/// The type of a non-const iterator over the hash map.
		typedef typename core::list<value_type, allocator_type>::iterator iterator;

		/// The type of a const iterator over the hash map.
		typedef typename core::list<value_type, allocator_type>::const_iterator const_iterator;

		/// Constructor.
		hash_table(allocator_type allocator = allocator_type()) :
			m_allocator(allocator)
		{
			// Initialise all buckets to empty.
			for (size_t i = 0; i < NumBuckets; ++i)
			{
				m_buckets[i].first = m_values.end();
				m_buckets[i].last = m_values.end();
			}
		}

		/// Get an iterator for the beginning of the map.
		iterator begin()
		{
			return m_values.begin();
		}

		/// Get an iterator for the beginning of the map.
		const_iterator begin() const
		{
			return m_values.begin();
		}

		/// Get an iterator for the end of the map.
		iterator end()
		{
			return m_values.end();
		}

		/// Get an iterator for the end of the map.
		const_iterator end() const
		{
			return m_values.end();
		}

		/// Check whether the map is empty.
		bool empty() const
		{
			return m_values.empty();
		}

		/// Find an entry in the map.
		iterator find(const key_type& k)
		{
			size_t bucket = hash(k) % NumBuckets;
			iterator it = m_buckets[bucket].first;
			if(it == m_values.end())
				return m_values.end();
			iterator end = m_buckets[bucket].last;
			++end;
			while(it != end)
			{
				Comparer cmp;
				if (cmp(it->first, k))
					return it;
				++it;
			}
			return m_values.end();
		}

		/// Find an entry in the map.
		const_iterator find(const key_type& k) const
		{
			size_t bucket = hash(k) % NumBuckets;
			const_iterator it = m_buckets[bucket].first;
			if (it == m_values.end())
				return it;
			const_iterator end = m_buckets[bucket].last;
			++end;
			while (it != end)
			{
				Comparer cmp;
				if (cmp(it->first, k))
				return it;
				++it;
			}
			return m_values.end();
		}

		/// Insert a new entry into the map.
		std::pair<iterator, bool> insert(const value_type& v)
		{
			size_t bucket = hash(v.first) % NumBuckets;
			iterator it = m_buckets[bucket].first;
			if(it == m_values.end())
			{
				m_buckets[bucket].first = m_buckets[bucket].last = m_values.insert(m_values.end(), v);
				return std::pair<iterator, bool>(m_buckets[bucket].last, true);
			}
			iterator end = m_buckets[bucket].last;
			++end;
			while(it != end)
			{
				if(it->first == v.first)
					return std::pair<iterator, bool>(it, false);
				++it;
			}
			m_buckets[bucket].last = m_values.insert(end, v);
			return std::pair<iterator, bool>(m_buckets[bucket].last, true);
		}

		/// Erase an entry from the map.
		void erase(iterator it)
		{
			TYCHO_ASSERT(it != m_values.end());

			size_t bucket = hash(it->first) % NumBuckets;
			bool is_first = (it == m_buckets[bucket].first);
			bool is_last = (it == m_buckets[bucket].last);
			if (is_first && is_last)
				m_buckets[bucket].first = m_buckets[bucket].last = m_values.end();
			else if (is_first)
				++m_buckets[bucket].first;
			else if (is_last)
				--m_buckets[bucket].last;

			m_values.erase(it);
		}

		/// Remove all entries from the map.
		void clear()
		{
			// Clear the values.
			m_values.clear();

			// Initialise all buckets to empty.
			for(size_t i = 0; i < NumBuckets; ++i)
				m_buckets[i].first = m_buckets[i].last = m_values.end();
		}

	private:
		// The list of all values in the hash map.
		core::list<value_type, allocator_type> m_values;
		
		/// allocator for this container
		allocator_type m_allocator;

		// The type for a bucket in the hash table.
		struct bucket_type
		{
			iterator first;
			iterator last;
		};

		// The buckets in the hash.
		bucket_type m_buckets[NumBuckets];
	};

} // end namespace
} // end namespace

#endif // __HASH_TABLE_H_9DC674F6_AC57_4470_8CB6_B62F47930AAB_
