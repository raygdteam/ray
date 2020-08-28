#pragma once
#include <eastl/string.h>

class String : public eastl::basic_string<char>
{
public:
	String() = default;

	explicit String(const allocator_type& allocator)
		: basic_string<char>(allocator)
	{
	}

	String(const this_type& x, size_type position, size_type n)
		: basic_string<char>(x, position, n)
	{
	}

	String(const value_type* p, size_type n, const allocator_type& allocator)
		: basic_string<char>(p, n, allocator)
	{
	}

	String(const value_type* p, const allocator_type& allocator)
		: basic_string<char>(p, allocator)
	{
	}

	String(size_type n, value_type c, const allocator_type& allocator)
		: basic_string<char>(n, c, allocator)
	{
	}

	explicit String(const this_type& x)
		: basic_string<char>(x)
	{
	}

	String(const this_type& x, const allocator_type& allocator)
		: basic_string<char>(x, allocator)
	{
	}

	String(const value_type* pBegin, const value_type* pEnd, const allocator_type& allocator)
		: basic_string<char>(pBegin, pEnd, allocator)
	{
	}

	String(const CtorDoNotInitialize& ctor_do_not_initialize, size_type n, const allocator_type& allocator)
		: basic_string<char>(ctor_do_not_initialize, n, allocator)
	{
	}

	String(const CtorSprintf& ctor_sprintf, const value_type* pFormat)
		: basic_string<char>(ctor_sprintf, pFormat)
	{
	}

	String(const std::initializer_list<char>& init, const allocator_type& allocator)
		: basic_string<char>(init, allocator)
	{
	}

	explicit String(this_type&& x)
		: basic_string<char>(x)
	{
	}

	String(this_type&& x, const allocator_type& allocator)
		: basic_string<char>(x, allocator)
	{
	}

	String(const view_type& sv, const allocator_type& allocator)
		: basic_string<char>(sv, allocator)
	{
	}

	String(const view_type& sv, size_type position, size_type n, const allocator_type& allocator)
		: basic_string<char>(sv, position, n, allocator)
	{
	}
	
};

using string = String;
