/*
 * This file is part of btag.
 *
 * © 2010 Fernando Tarlá Cardoso Lemos
 *
 * Refer to the LICENSE file for licensing information.
 *
 */

#ifndef SIMPLE_CAPITALIZATION_FILTER_H
#define SIMPLE_CAPITALIZATION_FILTER_H

#include "CapitalizationFilter.h"

class SimpleCapitalizationFilter : public CapitalizationFilter
{
    public:
        enum capitalization_mode {
            CAPITALIZATION_MODE_ALL_LOWER,
            CAPITALIZATION_MODE_ALL_UPPER,
            CAPITALIZATION_MODE_FIRST_UPPER
        };

        SimpleCapitalizationFilter(capitalization_mode mode);
        std::wstring filter(const std::wstring &input) const;

    private:
        capitalization_mode m_mode;
};

#endif
