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

class TitleCapitalizationFilter : public CapitalizationFilter
{
    public:
        std::wstring filter(const std::wstring &input) const;
};

#endif
