/*
 * This file is part of qtagger.
 *
 * © 2010 Fernando Tarlá Cardoso Lemos
 *
 * Refer to the LICENSE file for licensing information.
 *
 */

#ifndef ENGLISH_TITLE_LOCALIZATION_HANDLER
#define ENGLISH_TITLE_LOCALIZATION_HANDLER

#include "TitleLocalizationHandler.h"

class EnglishTitleLocalizationHandler : public TitleLocalizationHandler
{
    public:
        word_style word_style_for_word(const std::wstring &word, size_t index) const
        {
            if (index == 0)
                return WORD_STYLE_FIRST_UPPER;

            static const wchar_t *lowercase_words[] = {
                L"and", L"but", L"or", L"nor", L"to", L"over", L"an", L"a",
                L"the", L"at", L"by", L"for", L"in", L"of", L"on", L"be", L"is", NULL
            };

            for (int i = 0; lowercase_words[i]; ++i) {
                if (word == lowercase_words[i])
                    return WORD_STYLE_LOWER;
            }

            return WORD_STYLE_FIRST_UPPER;
        }
};

#endif
