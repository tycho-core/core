//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Tuesday, 8 April 2008 10:22:15 PM
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "perfect_hash.h"
#include "perfect_hash/perfect.h"
#include "perfect_hash/lookupa.h"
#include "perfect_hash/recycle.h"
#include "core/debug/assert.h"
#include "core/string.h"


//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////
namespace tycho
{
namespace core
{
	namespace ph = core::detail::perfect_hash;
	
	//------------------------------------------------------------------------------------
	
	perfect_hash::hash_function* perfect_hash::create_function(const std::vector<const char*>& inputs, int create_flags)
	{
		ph::hashform form;
		form.mode = ph::hashform::NORMAL_HM;
		form.hashtype = ph::hashform::STRING_HT;
		form.speed = ph::hashform::SLOW_HS;
		
		if((create_flags & cf_minimal) > 0)
		{
			form.perfect = ph::hashform::MINIMAL_HP;
		}
		else
		{
			TYCHO_ASSERT(create_flags & cf_perfect);
			form.perfect = ph::hashform::NORMAL_HP;			
		}

		// adapted from the driver
		using namespace ph;
		
		ub4       nkeys;                                         /* number of keys */
		key      *keys;                                    /* head of list of keys */
		bstuff   *tab;                                       /* table indexed by b */
		ub4       smax;            /* scramble[] values in 0..smax-1, a power of 2 */
		ub4       alen;                            /* a in 0..alen-1, a power of 2 */
		ub4       blen;                            /* b in 0..blen-1, a power of 2 */
		ub4       salt;                       /* a parameter to the hash function */
		reroot   *keyroot;                                       /* source of keys */
		gencode   final;                                    /* code for final hash */
		core::mem_zero(final);
		ub4       i;
		ub4       scramble[SCRAMBLE_LEN];           /* used in final hash function */
		char      buf[10][80];                        /* buffer for generated code */
		char     *buf2[10];                             /* also for generated code */

		// set up memory sources
		keyroot  = ph::remkroot(sizeof(key));

		// set up code for final hash
		final.line = buf2;
		final.used = 0;
		final.len  = 10;

		for(i = 0; i < 10; ++i) 
			final.line[i] = buf[i];

		// read in the list of keywords
		keys = 0;
		nkeys = 0;
		key  *mykey;
		for(size_t i = 0; i < inputs.size(); ++i)
		{		
		    mykey = (key *)renew(keyroot);		
			mykey->name_k = (ub1*)inputs[i];
			mykey->len_k  = (ub4)(core::strlen((char *)mykey->name_k));
			mykey->next_k = keys;
			keys = mykey;
			++nkeys;
		}

		// find the hash
		ph::findhash(&tab, &alen, &blen, &salt, &final, scramble, &smax, keys, nkeys, &form);

		// now make the hash function object
		// calculate how big our tab and scramble tables need to be
		int num_tabs = blen;
		int num_scrambles = 0;
		if (blen >= USE_SCRAMBLE)
		{
			// A way to make the 1-byte values in tab bigger
			if (smax > UB2MAXVAL+1)			
				num_scrambles = UB1MAXVAL+1;
			else
				num_scrambles = UB1MAXVAL/2+1;
		}

		int func_size = sizeof(hash_function) - 1 + num_tabs + num_scrambles;
		hash_function *new_func = (hash_function *)allocator::malloc(func_size);
		core::mem_zero(new_func, func_size);
		new_func->scramble_offset = static_cast<uint16>(num_tabs - 1);
		core::uint8 *tab_ptr = &new_func->tables[0];
		core::uint8 *scramble_ptr = &new_func->tables[new_func->scramble_offset];
		
		// set up tab and scramble arrays
		if(blen >= USE_SCRAMBLE)
		{
			// A way to make the 1-byte values in tab bigger
			if(smax > UB2MAXVAL+1)
			{
				for(i = 0; i <= UB1MAXVAL; i += 4)
				{
					for(int j = 0; j < 4; ++j)
					{
						TYCHO_ASSERT(scramble[i+j] < 256);
						*scramble_ptr++ = static_cast<core::uint8>(scramble[i+j]);
					}
				}
			}
			else
			{
				for(i = 0; i <= UB1MAXVAL; i += 8)
				{
					for(int j = 0; j < 8; ++j)
					{
						TYCHO_ASSERT(scramble[i+j] < 256);
						*scramble_ptr++ = static_cast<core::uint8>(scramble[i+j]);
					}
				}
			}
		}
		if(blen > 0)
		{
			TYCHO_ASSERT(smax <= UB1MAXVAL+1); // only support ub1 at the moment, if we have more than 0xff items then we need a ub2.
			
			if(blen < 16)
			{
				for(i = 0; i < blen; ++i) 
				{
					int val = scramble[tab[i].val_b];
					TYCHO_ASSERT(val < 256);
					*tab_ptr++ = static_cast<core::uint8>(val);
				}
			}
			else if(blen <= 1024)
			{
				for(i = 0; i < blen; i += 16)
				{
					for(int j = 0; j < 16; ++j)
					{
						int val = scramble[tab[i+j].val_b];
						TYCHO_ASSERT(val < 256);
						*tab_ptr++ = static_cast<core::uint8>(val);
					}
				}
			}
			else if (blen < USE_SCRAMBLE)
			{
				for (i=0; i<blen; i+=8)
				{
					for(int j = 0; j < 8; ++j)				
					{
						int val = scramble[tab[i+j].val_b];
						TYCHO_ASSERT(val < 256);
						*tab_ptr++ = static_cast<core::uint8>(val);
					}
				}
			}
			else 
			{
				for(i = 0; i < blen; i += 16)
				{
					for(int j = 0; j < 16; ++j)
					{
						int val = scramble[tab[i+j].val_b];
						TYCHO_ASSERT(val < 256);
						*tab_ptr++ = static_cast<core::uint8>(val);
					}
				}
			}
		}
		
		// setup correct function and parameters
		new_func->type  = static_cast<uint16>(final.p_type);
		new_func->size  = static_cast<uint8>(func_size);
		new_func->init  = final.p_initlev;
		new_func->shift = static_cast<uint16>(final.p_shift);
		new_func->mask  = static_cast<uint16>(final.p_mask);
		new_func->nkeys = static_cast<core::uint16>(nkeys);		
		
		/* clean up memory sources */
		ph::refree(keyroot);
		core::allocator::free((void *)tab);
		
		return new_func;
	}
	
	//------------------------------------------------------------------------------------

	int perfect_hash::lookup(const hash_function &func, const char *key, int len)
	{
		ub4 res = func.nkeys;
		switch(func.type)
		{
			case ph::gencode::CT_NORM_LE_UB4BITS_SCRAMBLED : res = hash_CT_NORM_LE_UB4BITS_SCRAMBLED(func, key, len); break;
			case ph::gencode::CT_ZERO : res = hash_CT_ZERO(func, key, len); break;
			default: TYCHO_ASSERT(!"not implemented");
		}
		if(res == func.nkeys)
			return -1;
			
		return res;
	}

	//------------------------------------------------------------------------------------

	core::uint32 perfect_hash::hash_CT_NORM_LE_UB4BITS_SCRAMBLED(const hash_function &func, const char *key, int len)
	{	    
		ub4 val = ph::lookup((ub1*)key, len, func.init);		
		val = (val >> func.shift);
		ub4 mix = func.tables[val & func.mask];
		val ^= mix;
		return val;
	}

	//------------------------------------------------------------------------------------

	core::uint32 perfect_hash::hash_CT_ZERO(const hash_function &, const char *, int)
	{	    
		return 0;
	}

	//------------------------------------------------------------------------------------

} // end namespace
} // end namespace
