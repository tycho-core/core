//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Tuesday, 22 April 2008 9:56:51 PM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __SCOPED_HASH_TABLE_H_DA6FFA85_3F19_4D49_94D8_85A818F946F9_
#define __SCOPED_HASH_TABLE_H_DA6FFA85_3F19_4D49_94D8_85A818F946F9_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "core/core_abi.h"
#include "core/shared_ptr.h"
#include "core/hash.h"
#include "core/string.h"
#include "core/containers/hash_table.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace tycho
{
namespace core
{ 
	template<class T>
	inline bool scoped_hash_table_default_equals_fn(const T& lhs, const T& rhs)
	{
		return lhs == rhs;
	}

	inline bool char_array_equals_fn(const char* const & lhs, const char* const & rhs)
	{
		return core::strcmp(lhs, rhs) == 0;
	}

	/// implements a hash table that maintains a stack of values for each key that can be pushed
	/// and popped from the table. lookup's always return the top value.
	template<class Key, 
			 class Value, 
			 int NumBuckets,  
			 bool (*KeyCmpFunc)(const Key&, const Key&) = scoped_hash_table_default_equals_fn<Key>, 
			 class allocator = default_allocator>
	class scoped_hash_table
	{
	public:
		scoped_hash_table()
		{
			core::mem_zero(m_buckets, sizeof(hash_bucket*) * NumBuckets);
		}
		
		~scoped_hash_table()
		{
			clear();
		}
		
				
		void push(Key key, const Value &value)
		{
			int bucket_idx = hash(key) % NumBuckets;
			hash_bucket* bucket = m_buckets[bucket_idx];
			bool create_new_bucket = true;
			if(bucket)
			{
				// find bucket for this key
				while(bucket)
				{
					if(KeyCmpFunc(bucket->key, key))
					{	
						// have a match so just add our binding to the head of the value chain
						hash_value* val = new hash_value;
						val->value = value;
						val->next  = bucket->value;
						bucket->value = val;
						create_new_bucket = false;
						break;		
					}
					bucket = bucket->next;
				}
			}			

			// create a new bucket
			if(create_new_bucket)
			{
				hash_bucket* new_bucket = new hash_bucket;
				new_bucket->key = key;
				new_bucket->value = new hash_value;
				new_bucket->value->next = 0;
				new_bucket->value->value = value; // wot a lot o values
				new_bucket->next = m_buckets[bucket_idx];
				m_buckets[bucket_idx] = new_bucket;
			}

		}		
		
		/// remove the current binding for the specified key from the top of the list
		bool pop(Key key)
		{
			int bucket_idx = hash(key) % NumBuckets;
			hash_bucket* bucket = m_buckets[bucket_idx];
			if(bucket)
			{
				// find bucket for this key
				while(bucket)
				{
					if(KeyCmpFunc(bucket->key, key))
					{	
						// chop its head off, this may leave the bucket empty (no value) but we don't free it
						// as theres only a fixed number of these and they are tiny so we can have so have 
						// empty ones waiting for a value is fine
						TYCHO_ASSERT(bucket->value);
						hash_value* next = bucket->value->next;
						delete bucket->value;
						bucket->value = next;
						return true;		
					}
					bucket = bucket->next;
				}
			}					
			return false;
		}

		Value* find(Key key)
		{		
			int bucket_idx = hash(key) % NumBuckets;
			hash_bucket* bucket = m_buckets[bucket_idx];
			if(bucket)
			{
				// find bucket for this key
				while(bucket)
				{
					if(KeyCmpFunc(bucket->key, key))
					{	
						return &bucket->value->value;
					}
					bucket = bucket->next;
				}
			}					
			return 0;
		}
						
		void clear()
		{
			for(int i = 0; i < NumBuckets; ++i)
				free_bucket_chain(m_buckets[i]);
			core::mem_zero(m_buckets, sizeof(hash_bucket*) * NumBuckets);
		}
		
	private:	
	
		/// non copyable for now
		void operator=(const scoped_hash_table&);
		
		// single entry in the hash table, stores a link to the next entry with the same name
		struct hash_value
		{
			Value		value;			
			hash_value* next;
		};
		
		// hash bucket, stores a linked list of all hash collisions.
		struct hash_bucket
		{
			Key			 key;
			hash_value*	 value;
			hash_bucket* next;
		};

		void free_value_chain(hash_value *v)
		{
			while(v)
			{
				hash_value* c = v;
				v = v->next;
				delete c;
			}
		}
		
		void free_bucket_chain(hash_bucket* b)
		{
			while(b)
			{
				hash_bucket* c = b;
				b = b->next;
				if(c->value)
					free_value_chain(c->value);
				delete c;
			}
		}
		
		hash_bucket* m_buckets[NumBuckets];
	};
	
} // end namespace
} // end namespace

#endif // __SCOPED_HASH_TABLE_H_DA6FFA85_3F19_4D49_94D8_85A818F946F9_
