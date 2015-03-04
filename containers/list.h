//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Wednesday, 26 March 2008 12:19:04 AM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __LIST_H_A30E0115_117D_473B_96E8_798246F640C5_
#define __LIST_H_A30E0115_117D_473B_96E8_798246F640C5_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "core/core_abi.h"
#include "core/containers/allocator.h"
#include <utility>

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace tycho
{
namespace core
{

	/// doubly linked list
	template<class T, class Allocator = tycho::core::default_allocator>
	class list
	{
	public:
		typedef list<T, Allocator>	this_type;
		typedef Allocator	allocator_type;
		typedef T			value_type;			
		typedef T*			pointer;
		typedef const T*	const_pointer;
		typedef T&          reference;
		typedef const T&    const_reference;
				
				
		/// internal node
		struct node
		{
			node() : prev(0), next(0) {}
			value_type value;
			node *prev;
			node *next;
		
		private:
			void operator=(const node &);
		};	 
		
		typedef node  node_type;
		typedef node* node_ptr;
		typedef const node* const_node_ptr;
		typedef node*& node_pref;
		typedef node* const & const_node_pref;
		
	public:
		struct const_iterator;
		struct iterator
		{
			iterator() : m_t(0) {}
			iterator(node_type *t) : m_t(t) {}
			iterator(const iterator &it) : m_t(it.m_t) {}
			
			T& operator*() 
			{ 
				return m_t->value; 
			}
			
			T* operator->()
			{
				return &m_t->value;
			}
			
			iterator operator++(int)
			{
				iterator tmp = *this;
				m_t = m_t->next;
				return tmp;
			}
			
			iterator operator++()
			{
				m_t = m_t->next;
				return *this;
			}
			
			/// post decrement
			iterator operator--(int)
			{
				iterator tmp = *this;
				--*this;
				return tmp;
				
			}

			/// pre decrement
			iterator operator--()
			{
				m_t = prev_node(m_t);
				return *this;					
			}
			
			bool operator==(iterator lhs)
			{
				return m_t == lhs.m_t;
			}
			
			bool operator!=(iterator lhs)
			{
				return !(*this == lhs);
			}

			iterator operator=(iterator rhs)
			{
				m_t = rhs.m_t;
				return *this;
			}
							
		private:
			friend struct const_iterator;
			friend class list<T, Allocator>;
			node_type* m_t;
		};
	
		struct const_iterator
		{
			const_iterator() : m_t(0) {}
			const_iterator(const node_type *t) : m_t(t) {}
			const_iterator(iterator it) : m_t(it.m_t) {}
			const_iterator(const const_iterator &it) : m_t(it.m_t) {}
			
			const_iterator& operator=(const iterator &it)
			{
				m_t = it->m_t;
				return *this;
			}
			
			const T& operator*() 
			{ 
				return m_t->value; 
			}

			const T* operator->()
			{
				return &m_t->value;
			}

			/// post increment				
			const_iterator operator++(int)
			{
				iterator tmp = *this;
				++*this;
				return tmp;
			}
			
			/// pre increment
			const_iterator operator++()
			{
				m_t = next_node(m_t);
				return *this;
			}
			
			/// post decrement
			const_iterator operator--(int)
			{
				const_iterator tmp = *this;
				--*this;
				return tmp;
				
			}

			/// pre decrement
			const_iterator operator--()
			{
				m_t = prev_node();
				return *this;					
			}
			
			bool operator==(const_iterator rhs)
			{
				return m_t == rhs.m_t;
			}
			
			bool operator!=(const_iterator rhs)
			{
				return !(*this == rhs);
			}
			
			const_iterator operator=(const_iterator rhs)
			{
				m_t = rhs->m_t;
				return *this;
			}
							
		private:
			friend class list<T, Allocator>;
			const_node_ptr m_t;
		};
		
		
		list(const allocator_type &a = allocator_type())
		{
			m_allocator = a;
			m_head = buynode();
			m_size = 0;
		}
		
		/// destructor
		~list()
			{ clear(); killnode(m_head);}
		
		static node_pref next_node(node_ptr n)
			{ return n->next; }

		static node_pref prev_node(node_ptr n)
			{ return n->prev;}

		static const_node_pref next_node(const_node_ptr n)
			{ return n->next; }

		static const_node_pref prev_node(const_node_ptr n)
			{ return n->prev;}

		
		iterator push_front(const value_type &v)
		{
			return insert(begin(), v);
		}
		
		iterator push_back(const value_type &v)
		{
			return insert(end(), v);
		}
		
		iterator insert(iterator where, const value_type &v)
		{
			node_ptr node = where.m_t;
			node_ptr new_node = buynode(node, prev_node(node), v);
			++m_size;
			prev_node(node) = new_node;
			next_node(prev_node(new_node)) = new_node;				
			return (--where);
		}
		
		iterator erase(iterator where)
		{
			node_ptr n = (where++).m_t;
			if(n != m_head)
			{	
				next_node(prev_node(n)) = next_node(n);
				prev_node(next_node(n)) = prev_node(n);
				tycho::core::destroy(n, m_allocator);
				--m_size;
			}
			return (where);			
		}
		
		//void insert(iterator _Where,size_type _Count, const Type& _Val);
		//template<class InputIterator>   void insert(iterator _Where,InputIterator _First,InputIterator _Last);			
		
		/// O(N)
		void clear()
		{
			node *next;
			node *n = next_node(m_head);
			next_node(m_head) = m_head;
			prev_node(m_head) = m_head;
			while(n != m_head)
			{
				next = next_node(n);
				killnode(n);
				n = next;
			}
			m_size = 0;
		}
		
		/// O(1) 
		bool empty() const
		{
			return m_size == 0;
		}
		
		/// O(1)
		size_t size() const
		{
			return m_size;
		}
		
		iterator begin()
			{ return iterator(next_node(m_head)); }
		
		iterator end()
			{ return iterator(m_head); }

		const_iterator begin() const
			{ return const_iterator(next_node(m_head)); }
		
		const_iterator end() const
			{ return const_iterator(m_head); }

	private:
		node_ptr buynode()
		{
			// allocate a node and links
			node *n = tycho::core::default_construct<node>(m_allocator);
			n->next = n;
			n->prev = n;
			return n;
		}
		
		node_ptr buynode(node_ptr next, node_ptr prev, const value_type &val)
		{	
			node_ptr n = tycho::core::allocate<node>(m_allocator);								
			tycho::core::construct(&n->value, val);
			tycho::core::construct(&n->next, next);
			tycho::core::construct(&n->prev, prev);
			return n;
		}			
		void killnode(node_ptr p)
		{
			tycho::core::destroy(p, m_allocator);
		}
		
	private:
		node* m_head;
		size_t m_size;
		allocator_type m_allocator;
	};


#include "list.inl"

} // end namespace
} // end namespace

#endif // __LIST_H_A30E0115_117D_473B_96E8_798246F640C5_
