/*
 * This file is part of btag.
 *
 * © 2010-2011 Fernando Tarlá Cardoso Lemos
 *
 * Refer to the LICENSE file for licensing information.
 *
 */

#ifndef ENGLISH_TITLE_LOCALIZATION_HANDLER_H
#define ENGLISH_TITLE_LOCALIZATION_HANDLER_H

#include "TitleLocalizationHandler.h"

class EnglishTitleLocalizationHandler : public TitleLocalizationHandler
{
    public:
        word_hint word_hint_for_word(const std::wstring &word, size_t index, size_t count, wchar_t after_punctuation) const;
};

#endif
