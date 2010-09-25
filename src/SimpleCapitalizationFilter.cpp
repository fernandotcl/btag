/*
 * This file is part of btag.
 *
 * © 2010 Fernando Tarlá Cardoso Lemos
 *
 * Refer to the LICENSE file for licensing information.
 *
 */

#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>

#include "SimpleCapitalizationFilter.h"

SimpleCapitalizationFilter::SimpleCapitalizationFilter(capitalization_mode mode)
    : m_mode(mode)
{
}

std::wstring SimpleCapitalizationFilter::filter(const std::wstring &input) const
{
    std::wstring processed = BasicStringFilter::filter(input);

    std::wstring res;
    res.reserve(processed.size());

    boost::char_separator<wchar_t> separator(L" ");
    boost::tokenizer<boost::char_separator<wchar_t>, std::wstring::const_iterator, std::wstring > words(processed, separator);
    BOOST_FOREACH(const std::wstring &word, words) {
        std::wstring new_word;
        new_word.reserve(word.size());
        for (std::wstring::size_type i = 0; i < word.size(); ++i)
            new_word += m_mode == CAPITALIZATION_MODE_ALL_UPPER ? uppercase(word[i]) : lowercase(word[i]);
        if (!res.empty()) res += L' ';
        res += new_word;
    }

    if (m_mode == CAPITALIZATION_MODE_FIRST_UPPER && res.size())
        res[0] = uppercase(res[0]);

    return res;
}
