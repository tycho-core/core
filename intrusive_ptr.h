//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Thursday, 10 April 2008 8:26:28 PM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __INTRUSIVE_PTR_H_A9382B6A_A007_48E6_AD53_D8D88214F23B_
#define __INTRUSIVE_PTR_H_A9382B6A_A007_48E6_AD53_D8D88214F23B_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "core/core_abi.h"
#include "boost/intrusive_ptr.hpp"

//////////////////////////////////////////////////////////////////////////////
// MACROS
//////////////////////////////////////////////////////////////////////////////

/// \todo investigate using a cleanup pass over entities / resources which just looks for any
///       with a ref count of 0 and deletes them. this would make the add_ref() and dec_ref() functions
///       just inc/dec and not have to check for zero and delete which pulls in a lot of code to everywhere
///       that take an intrusive pointer parameter, it also gives possibly the better benefit of letting us
///       delay actual deletion of classes of objects until a well defined time.



/// Helper macro to declare boost::intrusive_ptr functions for adding and removing references.
/// this will just try to call add_ref() and dec_ref() on the passed pointer so it better implement these.
#define TYCHO_INTRUSIVE_PTR_FUNC_DECLS(abi_, class_) \
	abi_ void intrusive_ptr_add_ref(class_ *r); \
	abi_ void intrusive_ptr_release(class_ *r);

/// Helper macro to declare boost::intrusive_ptr functions for adding and removing references.
/// this will just try to call add_ref() and dec_ref() on the passed pointer so it better implement these.
/// This needs to be defined in the cpp file
#define TYCHO_INTRUSIVE_PTR_FUNC_IMPL(abi_, class_) \
	abi_ void intrusive_ptr_add_ref(class_ *r) { r->add_ref(); } \
	abi_ void intrusive_ptr_release(class_ *r) { r->dec_ref(); }

/// Helper macro to forward declare the boost::intrusive_ptr functions for adding and removing references.
#define TYCHO_INTRUSIVE_PTR_FORWARD_DECLS(abi_, class_) \
	abi_ void intrusive_ptr_add_ref(class_ *r);  \
	abi_ void intrusive_ptr_release(class_ *r);
	

/// Helper macro to insert friend definitions for the intrusive_ptr management functions.
#define TYCHO_INTRUSIVE_PTR_FRIEND_DECLS(abi_, class_) \
		friend abi_ void intrusive_ptr_add_ref(class_*); \
		friend abi_ void intrusive_ptr_release(class_*);

/// Use this to declare an intrusive pointer inline (i.e not a forward declaration)
#define TYCHO_DECLARE_INTRUSIVE_PTR_INLINE(abi_, class_) \
	typedef boost::intrusive_ptr<class_> class_##_ptr;
				
/// Use this to declare an intrusive pointer, this is normally declared in the appropriate forward_decl 
/// file for the library the class is declared in.
#define TYCHO_DECLARE_INTRUSIVE_PTR(abi_, class_) \
	class abi_ class_; \
	typedef boost::intrusive_ptr<class_> class_##_ptr; \
	TYCHO_INTRUSIVE_PTR_FORWARD_DECLS(abi_, class_)
		
#endif // __INTRUSIVE_PTR_H_A9382B6A_A007_48E6_AD53_D8D88214F23B_
