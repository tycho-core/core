//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Saturday, 12 April 2008 3:24:12 PM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __SHARED_PTR_H_70A439C6_540F_479F_AC9A_EFEC1164D4C1_
#define __SHARED_PTR_H_70A439C6_540F_479F_AC9A_EFEC1164D4C1_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "core/core_abi.h"
#include <memory>

//////////////////////////////////////////////////////////////////////////////
// MACROS
//////////////////////////////////////////////////////////////////////////////


#define TYCHO_DECLARE_SHARED_PTR(abi_, class_) \
	class abi_ class_; \
	typedef std::shared_ptr<class_> class_##_ptr;

#endif // __SHARED_PTR_H_70A439C6_540F_479F_AC9A_EFEC1164D4C1_
