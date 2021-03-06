//////////////////////////////////////////////////////////////////////////////
// Copyright 2006  Martin Slater
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER

#ifndef TYPES_H_MS_2006_8_1_20_57_18_
#define TYPES_H_MS_2006_8_1_20_57_18_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#if TYCHO_GC
#include <stddef.h> // for size_t
#endif

#include <cstdint>

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

        typedef std::int8_t int8;
        typedef std::uint8_t unint8;
        typedef std::int16_t int16;
        typedef std::uint16_t uint16;
        typedef std::int32_t int32;
        typedef std::uint32_t uint32;
        typedef std::int64_t int64;
        typedef std::uint64_t uint64;

        typedef void *ptr_type;
        typedef float float32;
        typedef double float64;
        typedef unsigned int uint;

    } // namespace core
} // namespace tycho

#endif // TYPES_H_MS_2006_8_1_20_57_18_
