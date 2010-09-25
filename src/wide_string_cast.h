/*
 * This file is part of btag.
 *
 * © 2010 Fernando Tarlá Cardoso Lemos
 *
 * Refer to the LICENSE file for licensing information.
 *
 */

#ifndef WIDE_STRING_CAST_H
#define WIDE_STRING_CAST_H

#include <taglib/tstring.h>
#include <string>
#include <locale>

#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

namespace boost
{
    template<>
    inline std::wstring lexical_cast<std::wstring, std::string>(const std::string &arg)
    {
        std::wstring result;
        BOOST_FOREACH(char c, arg)
            result += std::use_facet<std::ctype<wchar_t> >(std::locale()).widen(c);
        return result;
    }
}

#endif
