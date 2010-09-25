/*
 * This file is part of btag.
 *
 * © 2010 Fernando Tarlá Cardoso Lemos
 *
 * Refer to the LICENSE file for licensing information.
 *
 */

#ifndef TITLE_CAPITALIZATION_FILTER_H
#define TITLE_CAPITALIZATION_FILTER_H

#include "CapitalizationFilter.h"
#include "TitleLocalizationHandler.h"

class TitleCapitalizationFilter : public CapitalizationFilter
{
    public:
        TitleCapitalizationFilter() : m_handler(NULL) {}
        void set_localization_handler(TitleLocalizationHandler *handler) { m_handler = handler; }

        std::wstring filter(const std::wstring &input) const;

    private:
        enum element_type {
            ELEMENT_TYPE_WORD,
            ELEMENT_TYPE_PUNCTUATION_LEFT,
            ELEMENT_TYPE_PUNCTUATION_RIGHT,
            ELEMENT_TYPE_PUNCTUATION_HANGING
        };

        struct element {
            element_type type;
            std::wstring text;
            element(element_type t, const std::wstring &x) : type(t), text(x) {}
        };

        bool is_punctuation(wchar_t c) const;

        TitleLocalizationHandler *m_handler;
};

#endif
