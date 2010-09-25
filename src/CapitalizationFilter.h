/*
 * This file is part of btag.
 *
 * © 2010 Fernando Tarlá Cardoso Lemos
 *
 * Refer to the LICENSE file for licensing information.
 *
 */

#ifndef CAPITALIZATION_FILTER_H
#define CAPITALIZATION_FILTER_H

#include "BasicStringFilter.h"

class CapitalizationFilter : public BasicStringFilter
{
    public:
        bool requires_confirmation_as_output_filter() const { return true; }

    protected:
        wchar_t lowercase(wchar_t c) const
        {
            return std::use_facet<std::ctype<wchar_t> >(std::locale()).tolower(c);
        }

        wchar_t uppercase(wchar_t c) const
        {
            return std::use_facet<std::ctype<wchar_t> >(std::locale()).toupper(c);
        }
};

#endif
