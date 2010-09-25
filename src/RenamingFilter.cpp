/*
 * This file is part of btag.
 *
 * © 2010 Fernando Tarlá Cardoso Lemos
 *
 * Refer to the LICENSE file for licensing information.
 *
 */

#include <boost/foreach.hpp>

#include "RenamingFilter.h"

std::wstring RenamingFilter::filter(const std::wstring &path_component) const
{
    std::wstring new_component;
    new_component.reserve(path_component.size());

    BOOST_FOREACH(wchar_t c, path_component)
        new_component += is_character_allowed(c) ? c : replacement_character(c);

    if (new_component.empty())
        new_component += replacement_character(L'A');

    return new_component;
}
