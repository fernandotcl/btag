/*
 * This file is part of qtagger.
 *
 * © 2010 Fernando Tarlá Cardoso Lemos
 *
 * Refer to the LICENSE file for licensing information.
 *
 */

#ifndef BASIC_STRING_FILTER
#define BASIC_STRING_FILTER

#include <string>

class BasicStringFilter
{
    public:
        virtual std::wstring filter(const std::wstring &input) const;
        virtual bool requires_confirmation_as_output_filter() const { return false; }
};

#endif
