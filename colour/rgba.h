//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Monday, 7 April 2008 7:07:03 PM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __RGBA_H_AF8647FA_463A_468B_8A45_2D8C34C167B6_
#define __RGBA_H_AF8647FA_463A_468B_8A45_2D8C34C167B6_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "core/core_abi.h"
#include "core/debug/assert.h"
#include "math/vector4.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace tycho
{
namespace core
{

	/// uint8 x 4 colour.
    class CORE_ABI rgba
    {
    public:
		/// default constructor, does not initialise is components. use rgba(0,0,0,0) to explicitly 
		/// create a zero colour.
		rgba();
					
		/// rgba constructor, takes int so it can validate the input in debug.
		rgba(int r, int g, int b, int a);

		/// rgb constructor, takes int so it can validate the input in debug. 
		/// the alpha component it set to 255.
		rgba(int r, int g, int b);
		
		/// copy constructor, want by value instead of by const ref but class isn't 
		/// fully declared so can't. wonder if the optimiser can get this.
		rgba(const rgba &);
		
		/// assignment operator
		rgba& operator=(const rgba &);
		
		/// equality operator
		bool operator==(const rgba&) const;

		/// inequality operator
		bool operator!=(const rgba&) const;
		
		/// access the red component
		core::uint8 r() const;
		
		/// access the green component
		core::uint8 g() const;
		
		/// access the blue component
		core::uint8 b() const;

		/// access the alpha component
		core::uint8 a() const;
				
		/// set the red component
		void r(core::uint8);
		
		/// set the green component
		void g(core::uint8);

		/// set the blue component
		void b(core::uint8);
				
		/// set the alpha component
		void a(core::uint8);
		
    private:
		core::uint8 m_r;
		core::uint8 m_g;
		core::uint8 m_b;
		core::uint8 m_a;		
    };

	template<class T> math::vector4<T> to_vector4(core::rgba clr);

#include "core/colour/rgba.inl"

} // end namespace
} // end namespace

#endif // __RGBA_H_AF8647FA_463A_468B_8A45_2D8C34C167B6_
