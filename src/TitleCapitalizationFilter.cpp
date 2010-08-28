/*
 * This file is part of qtagger.
 *
 * © 2010 Fernando Tarlá Cardoso Lemos
 *
 * Refer to the LICENSE file for licensing information.
 *
 */

#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>

#include "TitleCapitalizationFilter.h"
#include "TitleLocalizationHandler.h"

std::wstring TitleCapitalizationFilter::filtered_word(const std::wstring &word, size_t index) const
{
    std::wstring check_word;
    check_word.reserve(word.size());
    BOOST_FOREACH(wchar_t c, word) {
        if (c != L'(' && c != L')')
            check_word += lowercase(c);
    }

    TitleLocalizationHandler::word_style style = TitleLocalizationHandler::WORD_STYLE_FIRST_UPPER;
    if (m_handler && check_word.size()) style = m_handler->word_style_for_word(check_word, index);

    std::wstring new_word;
    new_word.reserve(word.size());

    switch (style) {
        case TitleLocalizationHandler::WORD_STYLE_LOWER:
            BOOST_FOREACH(wchar_t c, word) new_word += lowercase(c);
            break;
        case TitleLocalizationHandler::WORD_STYLE_UPPER:
            BOOST_FOREACH(wchar_t c, word) new_word += uppercase(c);
            break;
        case TitleLocalizationHandler::WORD_STYLE_FIRST_UPPER:
            {
                bool upped_first = false;
                BOOST_FOREACH(wchar_t c, word) {
                    if (!upped_first && c != L'(') {
                        new_word += uppercase(c);
                        upped_first = true;
                    }
                    else {
                        new_word += lowercase(c);
                    }
                }
                break;
            }
    }

    return new_word;
}

std::wstring TitleCapitalizationFilter::filter(const std::wstring &input) const
{
    std::wstring processed = BasicStringFilter::filter(input);

    std::wstring res;
    res.reserve(processed.size());

    size_t word_index = 0;
    boost::char_separator<wchar_t> separator(L" ");
    boost::tokenizer<boost::char_separator<wchar_t>, std::wstring::const_iterator, std::wstring > words(processed, separator);
    BOOST_FOREACH(const std::wstring &word, words) {
        if (!res.empty()) res += L' ';
        res += filtered_word(word, word_index++);
    }

    return res;
}
