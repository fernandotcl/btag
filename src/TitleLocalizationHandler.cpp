/*
 * This file is part of btag.
 *
 * © 2010 Fernando Tarlá Cardoso Lemos
 *
 * Refer to the LICENSE file for licensing information.
 *
 */

#include <boost/foreach.hpp>

#include "TitleLocalizationHandler.h"

bool TitleLocalizationHandler::is_acronym(const std::wstring &word) const
{
    if (word.size() < 4 || word[word.size() - 1] != L'.')
        return false;

    bool should_be_dot = false;
    BOOST_FOREACH(wchar_t c, word) {
        if (should_be_dot && c != L'.')
            return false;
        else if (!should_be_dot && c == L'.')
            return false;
        else
            should_be_dot = !should_be_dot;
    }

    return true;
}

wchar_t *TitleLocalizationHandler::punctuation_list() const
{
    static wchar_t punctuation[] = {
        L',', L'.', L':', L'-', L';', L'?', L'¿', L'!', L'¡', L'-', L'—', L')', L'(', L'"', L'\'', L'“', L'”', L'‘', L'’'
    };
    return punctuation;
}
