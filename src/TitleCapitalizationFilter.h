/*
 * This file is part of qtagger.
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
        std::wstring filtered_word(const std::wstring &word, size_t index) const;

        TitleLocalizationHandler *m_handler;
};

#endif
