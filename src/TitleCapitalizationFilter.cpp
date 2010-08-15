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

std::wstring TitleCapitalizationFilter::filter(const std::wstring &input) const
{
    // TODO Use language-specific filters for real title casing

    std::wstring processed = BasicStringFilter::filter(input);

    std::wstring res;
    res.reserve(processed.size());

    boost::char_separator<wchar_t> separator(L" ");
    boost::tokenizer<boost::char_separator<wchar_t>, std::wstring::const_iterator, std::wstring > words(processed, separator);
    BOOST_FOREACH(const std::wstring &word, words) {
        std::wstring new_word;
        new_word.reserve(word.size());
        new_word += uppercase(word[0]);
        for (std::wstring::size_type i = 1; i < word.size(); ++i)
            new_word += lowercase(word[i]);
        if (!res.empty()) res += L' ';
        res += new_word;
    }

    return res;
}
