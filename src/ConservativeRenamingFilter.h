/*
 * This file is part of btag.
 *
 * © 2010-2011 Fernando Tarlá Cardoso Lemos
 *
 * Refer to the LICENSE file for licensing information.
 *
 */

#ifndef CONSERVATIVE_RENAMING_FILTER_H
#define CONSERVATIVE_RENAMING_FILTER_H

#include "RenamingFilter.h"

class ConservativeRenamingFilter : public RenamingFilter
{
    private:
        bool is_character_allowed(wchar_t c) const
        {
            if (c >= L'a' && c <= L'z') return true;
            if (c >= L'A' && c <= L'Z') return true;
            if (c >= L'0' && c <= L'9') return true;

            static wchar_t allowed_chars[15] = {
                L' ', L'%', L'-', L'_', L'@', L'~', L'`', L'!',
                L'(', L')', L'{', L'}', L'^', L'#', L'&',
            };
            for (int i = 0; i < 15; ++i) {
                if (c == allowed_chars[i])
                    return true;
            }

            return false;
        }

        wchar_t replacement_character(wchar_t c) const
        {
            return L'_';
        }
};

#endif
