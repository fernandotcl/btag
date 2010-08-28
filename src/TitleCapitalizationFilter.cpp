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
    std::wstring new_word;
    new_word.reserve(word.size());
    for (std::wstring::size_type i = 0; i < word.size(); ++i)
        new_word += lowercase(word[i]);

    TitleLocalizationHandler::word_style style = TitleLocalizationHandler::WORD_STYLE_FIRST_UPPER;
    if (m_handler) style = m_handler->word_style_for_word(new_word, index);

    if (style == TitleLocalizationHandler::WORD_STYLE_UPPER) {
        for (std::wstring::size_type i = 0; i < word.size(); ++i)
            new_word += uppercase(word[i]);
    }
    else if (style == TitleLocalizationHandler::WORD_STYLE_FIRST_UPPER) {
        new_word[0] = uppercase(word[0]);
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
