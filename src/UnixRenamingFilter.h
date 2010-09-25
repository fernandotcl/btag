/*
 * This file is part of btag.
 *
 * © 2010 Fernando Tarlá Cardoso Lemos
 *
 * Refer to the LICENSE file for licensing information.
 *
 */

#ifndef UNIX_RENAMING_FILTER_H
#define UNIX_RENAMING_FILTER_H

#include "RenamingFilter.h"

class UnixRenamingFilter : public RenamingFilter
{
    private:
        bool is_character_allowed(wchar_t c) const
        {
            return c != L'/' && c != L'\0';
        }

        wchar_t replacement_character(wchar_t c) const
        {
            return L'_';
        }
};

#endif
