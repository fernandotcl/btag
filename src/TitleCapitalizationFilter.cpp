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
#include <boost/shared_ptr.hpp>
#include <list>

#include "TitleCapitalizationFilter.h"
#include "TitleLocalizationHandler.h"

bool TitleCapitalizationFilter::is_punctuation(wchar_t c) const
{
    wchar_t *punctuation_list = m_handler->punctuation_list();
    for (int i = 0; punctuation_list[i]; ++i) {
        if (c == punctuation_list[i])
            return true;
    }
    return false;
}

std::wstring TitleCapitalizationFilter::filter(const std::wstring &input) const
{
    std::wstring processed = BasicStringFilter::filter(input);

    boost::char_separator<wchar_t> separator(L" ");
    boost::tokenizer<boost::char_separator<wchar_t>, std::wstring::const_iterator, std::wstring > tokens(processed, separator);

    int num_words = 0;
    std::list<boost::shared_ptr<element> > elements;
    BOOST_FOREACH(const std::wstring &token, tokens) {
        if (token.empty()) continue;

        // Look for all punctuation to the left of the token
        std::wstring punctuation_left;
        BOOST_FOREACH(wchar_t c, token) {
            if (!is_punctuation(c)) break;
            punctuation_left += c;
        }

        // Handle hanging punctuation
        if (punctuation_left.size() == token.size()) {
            boost::shared_ptr<element> e(new element(ELEMENT_TYPE_PUNCTUATION_HANGING, token));
            elements.push_back(e);
            continue;
        }

        // Add the punctuation to the left to the list
        if (!punctuation_left.empty()) {
            boost::shared_ptr<element> e(new element(ELEMENT_TYPE_PUNCTUATION_LEFT, punctuation_left));
            elements.push_back(e);
        }

        // Check if this could be an acronym
        std::wstring acronym(token.substr(punctuation_left.size()));
        bool is_acronym = m_handler->is_acronym(acronym);

        // Look for all punctuation to the right of the token
        std::wstring punctuation_right;
        if (!is_acronym) {
            BOOST_REVERSE_FOREACH(wchar_t c, token) {
                if (!is_punctuation(c)) break;
                punctuation_right = c + punctuation_right;
            }
        }

        // Find the word between the punctuation and add it to the list
        size_t word_size = token.size() - punctuation_left.size() - punctuation_right.size();
        std::wstring word(token.substr(punctuation_left.size(), word_size));
        elements.push_back(boost::shared_ptr<element>(new element(ELEMENT_TYPE_WORD, word)));
        ++num_words;

        // Add the punctuation to the right to the list
        if (!punctuation_right.empty()) {
            boost::shared_ptr<element> e(new element(ELEMENT_TYPE_PUNCTUATION_RIGHT, punctuation_right));
            elements.push_back(e);
        }
    }

    std::wstring res;
    res.reserve(processed.size());

    bool last_was_punctuation_left = false;
    int word_index = 0;
    boost::shared_ptr<element> last_e;
    BOOST_FOREACH(boost::shared_ptr<element> e, elements) {
        if (e->type == ELEMENT_TYPE_WORD) {
            // Lowercase the string
            BOOST_FOREACH(wchar_t &c, e->text) c = lowercase(c);

            // Get a hint
            wchar_t after_punctuation = last_e.get() && last_e->type != ELEMENT_TYPE_WORD ? last_e->text[last_e->text.size() - 1] : 0;
            TitleLocalizationHandler::word_hint hint = m_handler->word_hint_for_word(e->text, word_index++, num_words, after_punctuation);

            // Apply it
            switch (hint.style) {
                case TitleLocalizationHandler::WORD_STYLE_LOWER:
                    break;
                case TitleLocalizationHandler::WORD_STYLE_UPPER:
                    BOOST_FOREACH(wchar_t &c, e->text) c = uppercase(c);
                    break;
                case TitleLocalizationHandler::WORD_STYLE_FIRST_UPPER:
                    e->text[0] = uppercase(e->text[0]);
                    break;
                case TitleLocalizationHandler::WORD_STYLE_CUSTOM:
                    for (size_t i = 0; i < hint.uppercase->size(); ++i) {
                        if ((*hint.uppercase)[i])
                            e->text[i] = uppercase(e->text[i]);
                    }
                    break;
            }
        }

        // Append to the result
        switch (e->type) {
            case ELEMENT_TYPE_WORD:
                if (!last_was_punctuation_left && !res.empty())
                    res += L' ';
                res += e->text;
                last_was_punctuation_left = false;
                break;
            case ELEMENT_TYPE_PUNCTUATION_RIGHT:
                res += e->text;
                last_was_punctuation_left = false;
                break;
            case ELEMENT_TYPE_PUNCTUATION_LEFT:
                if (!res.empty())
                    res += L' ';
                res += e->text;
                last_was_punctuation_left = true;
                break;
            case ELEMENT_TYPE_PUNCTUATION_HANGING:
                if (!res.empty())
                    res += L' ';
                res += e->text;
                last_was_punctuation_left = false;
                break;
        }

        last_e = e;
    }

    return res;
}
