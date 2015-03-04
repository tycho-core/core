//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Monday, 7 April 2008 7:07:03 PM
//////////////////////////////////////////////////////////////////////////////

inline rgba::rgba()
{
	// no - op
}
			
inline rgba::rgba(int r_, int g_, int b_, int a_)
{
	TYCHO_ASSERT(r_ >= 0 && r_ <= 255);
	TYCHO_ASSERT(g_ >= 0 && g_ <= 255);
	TYCHO_ASSERT(b_ >= 0 && b_ <= 255);
	TYCHO_ASSERT(a_ >= 0 && a_ <= 255);

	r(static_cast<uint8>(r_));
	g(static_cast<uint8>(g_));
	b(static_cast<uint8>(b_));
	a(static_cast<uint8>(a_));
}


inline rgba::rgba(int r_, int g_, int b_)
{
	TYCHO_ASSERT(r_ >= 0 && r_ <= 255);
	TYCHO_ASSERT(g_ >= 0 && g_ <= 255);
	TYCHO_ASSERT(b_ >= 0 && b_ <= 255);

	r(static_cast<uint8>(r_));
	g(static_cast<uint8>(g_));
	b(static_cast<uint8>(b_));
	a(255);
}

inline rgba::rgba(const rgba &clr)
{
	r(clr.r());
	g(clr.g());
	b(clr.b());
	a(clr.a());
}


inline rgba& rgba::operator=(const rgba &other)
{
	r(other.r());
	g(other.g());
	b(other.b());
	a(other.a());
	return *this;
}

inline bool rgba::operator==(const rgba& rhs) const
{
	return r() == rhs.r() && 
		   g() == rhs.g() &&
		   b() == rhs.b() &&
		   a() == rhs.a();
}

inline bool rgba::operator!=(const rgba& rhs) const
{
	return !(*this == rhs);
}


/// access the red component
inline core::uint8 rgba::r() const
{
	return m_r;
}

/// access the green component
inline core::uint8 rgba::g() const
{
	return m_g;
}

/// access the blue component
inline core::uint8 rgba::b() const
{
	return m_b;
}

/// access the alpha component
inline core::uint8 rgba::a() const
{
	return m_a;
}
		
/// set the red component
inline void rgba::r(core::uint8 v)
{
	m_r = v;
}

/// set the green component
inline void rgba::g(core::uint8 v)
{
	m_g = v;
}

/// set the blue component
inline void rgba::b(core::uint8 v)
{
	m_b = v;
}
		
/// set the alpha component
inline void rgba::a(core::uint8 v)
{
	m_a = v;
}

template<class T>
math::vector4<T> to_vector4(core::rgba clr)
{
	return math::vector4<T>((T)clr.r() / 255.0f, (T)clr.g() / 255.0f, (T)clr.b() / 255.0f, (T)clr.a() / 255.0f);
}
