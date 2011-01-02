/*
 * This file is part of btag.
 *
 * © 2010 Fernando Tarlá Cardoso Lemos
 *
 * Refer to the LICENSE file for licensing information.
 *
 */

#ifndef TITLE_LOCALIZATION_HANDLER_H
#define TITLE_LOCALIZATION_HANDLER_H

#include <boost/optional.hpp>
#include <string>
#include <vector>

class TitleLocalizationHandler
{
    public:
        enum word_style {
            WORD_STYLE_LOWER,
            WORD_STYLE_UPPER,
            WORD_STYLE_FIRST_UPPER,
            WORD_STYLE_CUSTOM
        };

        struct word_hint {
            word_style style;
            boost::optional<std::vector<bool> > uppercase;
            word_hint(word_style s) : style(s) {}
            word_hint(const std::vector<bool> &u) : style(WORD_STYLE_CUSTOM), uppercase(u) {}
        };

        virtual word_hint word_hint_for_word(const std::wstring &word, size_t index, size_t count, wchar_t after_punctuation) const = 0;
        virtual bool is_acronym(const std::wstring &word) const;
        virtual wchar_t *punctuation_list() const;
};

#endif
