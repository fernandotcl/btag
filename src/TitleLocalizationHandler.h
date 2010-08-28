/*
 * This file is part of qtagger.
 *
 * © 2010 Fernando Tarlá Cardoso Lemos
 *
 * Refer to the LICENSE file for licensing information.
 *
 */

#ifndef TITLE_LOCALIZATION_HANDLER_H
#define TITLE_LOCALIZATION_HANDLER_H

#include <string>

class TitleLocalizationHandler
{
    public:
        enum word_style {
            WORD_STYLE_LOWER,
            WORD_STYLE_UPPER,
            WORD_STYLE_FIRST_UPPER
        };

        virtual word_style word_style_for_word(const std::wstring &word, size_t index) const = 0;
};

#endif
