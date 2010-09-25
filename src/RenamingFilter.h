/*
 * This file is part of btag.
 *
 * © 2010 Fernando Tarlá Cardoso Lemos
 *
 * Refer to the LICENSE file for licensing information.
 *
 */

#ifndef RENAMING_FILTER_H
#define RENAMING_FILTER_H

#include <string>

class RenamingFilter
{
    public:
        std::wstring filter(const std::wstring &path_component) const;

    private:
        virtual bool is_character_allowed(wchar_t c) const = 0;
        virtual wchar_t replacement_character(wchar_t c) const = 0;
};

#endif
