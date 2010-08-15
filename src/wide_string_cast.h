/*
 * This file is part of qtagger.
 *
 * © 2010 Fernando Tarlá Cardoso Lemos
 *
 * Refer to the LICENSE file for licensing information.
 *
 */

#ifndef WIDE_STRING_CAST_H
#define WIDE_STRING_CAST_H

#include <string>
#include <locale>

#include <boost/lexical_cast.hpp>

namespace boost
{
	template<>
    inline std::wstring lexical_cast<std::wstring, std::string>(const std::string &arg)
    {
		std::wstring result;
		for (std::string::size_type i = 0; i < arg.size(); ++i)
			result += std::use_facet<std::ctype<wchar_t> >(std::locale()).widen(arg[i]);
		return result;
    }

	template<>
    inline std::string lexical_cast<std::string, std::wstring>(const std::wstring &arg)
    {
		std::string result;
		std::locale loc;
		for (std::wstring::size_type i = 0; i < arg.size(); ++i)
			result += std::use_facet<std::ctype<wchar_t> >(std::locale()).narrow(arg[i], '?');
		return result;
    }
}

#endif
