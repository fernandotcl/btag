/*
 * This file is part of btag.
 *
 * © 2010 Fernando Tarlá Cardoso Lemos
 *
 * Refer to the LICENSE file for licensing information.
 *
 */

#ifndef VALIDATORS_H
#define VALIDATORS_H

#include "InteractiveTerminal.h"

class YearValidator : public InteractiveTerminal::Validator<int>
{
    public:
        bool validate(const int &year, boost::optional<std::wstring> &error_message) const
        {
            if (year > 2500 || year < 1000) {
                error_message = L"The year should be a number between 1000 and 2500";
                return false;
            }
            else {
                return true;
            }
        }
};

class TrackValidator : public InteractiveTerminal::Validator<int>
{
    public:
        bool validate(const int &track, boost::optional<std::wstring> &error_message) const
        {
            if (track > 1000 || track < 1) {
                error_message = L"The track should be a number between 1 and 1000";
                return false;
            }
            else {
                return true;
            }
        }
};

#endif
