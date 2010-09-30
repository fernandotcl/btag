/*
 * This file is part of btag.
 *
 * © 2010 Fernando Tarlá Cardoso Lemos
 *
 * Refer to the LICENSE file for licensing information.
 *
 */

#include "SpanishTitleLocalizationHandler.h"

TitleLocalizationHandler::word_hint SpanishTitleLocalizationHandler::word_hint_for_word(const std::wstring &word,
        size_t index, size_t count, wchar_t after_punctuation) const
{
    // Uppercase acronyms
    if (is_acronym(word))
        return WORD_STYLE_UPPER;

    // Check for hyphens
    if (word.size() > 2 && word[0] != L'-' && word[word.size() - 1] != L'-') {
        boost::optional<std::vector<bool> > uppercase;
        for (size_t i = 1; i < word.size() - 1; ++i) {
            if (word[i] == L'-') {
                if (!uppercase) {
                    uppercase = std::vector<bool>(word.size(), false);
                    (*uppercase)[0] = true;
                }
                (*uppercase)[i + 1] = true;
            }
        }
        if (uppercase)
            return *uppercase;
    }

    // Uppercase the first letter of the first and last words
    if (index == 0 || index == count - 1)
        return WORD_STYLE_FIRST_UPPER;

    // Uppercase the first letter of a word after major punctuation
    static const wchar_t major_punctuation[] = {
        L'.', L':', L'?', L'!', L'—', L')', L'(', L'"', L'\'', L'“', L'”', L'‘', L'’', 0
    };
    if (after_punctuation != 0) {
        for (int i = 0; major_punctuation[i]; ++i) {
            if (after_punctuation == major_punctuation[i])
                return WORD_STYLE_FIRST_UPPER;
        }
    }

    // Lowercase some specific words (context insensitive)
    static const wchar_t *lowercase_words[] = {
        L"el", L"la", L"los", L"las", L"una", L"unas", L"uno", L"unos", L"al", L"del", // articles
        L"a", L"con", L"de", L"en", L"para", L"por", L"sin",                           // common prepositions
        L"y", L"e", L"o", L"u", L"que", L"ni",                                         // common conjunctions
        L"me", L"te", L"se", L"nos", L"os", L"se",                                     // reflexive pronouns
        L"vs.", L"etc.", NULL                                                          // some exceptions
    };
    for (int i = 0; lowercase_words[i]; ++i) {
        if (word == lowercase_words[i])
            return WORD_STYLE_LOWER;
    }

    return WORD_STYLE_FIRST_UPPER;
}
