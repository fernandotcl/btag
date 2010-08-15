/*
 * This file is part of qtagger.
 *
 * © 2010 Fernando Tarlá Cardoso Lemos
 *
 * Refer to the LICENSE file for licensing information.
 *
 */

#include <boost/foreach.hpp>

#include "BasicStringFilter.h"

std::wstring BasicStringFilter::filter(const std::wstring &input) const
{
    std::wstring res;
    res.reserve(input.size());

    bool started = false, in_whitespace = false;
    BOOST_FOREACH(wchar_t c, input) {
        if (started) {
            if (c == '\n' || c == '\r' || c == '\t' || c == ' ' || c == '\v') {
                in_whitespace = true;
            }
            else {
                if (in_whitespace) {
                    res += ' ';
                    in_whitespace = false;
                }
                res += c;
            }
        }
        else if (c != '\n' && c != '\r' && c != '\t' && c != ' ' && c != '\v') {
            res += c;
            started = true;
        }
    }

    return res;
}
