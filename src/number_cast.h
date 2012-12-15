/*
 * This file is part of btag.
 *
 * © 2012 Fernando Tarlá Cardoso Lemos
 *
 * Refer to the LICENSE file for licensing information.
 *
 */

#ifndef NUMBER_CAST_H
#define NUMBER_CAST_H

#include <locale>
#include <sstream>
#include <string>

template<typename T>
inline std::wstring number_cast(T arg)
{
    std::wostringstream oss;
    oss.imbue(std::locale::classic());
    oss << arg;
    return oss.str();
}

#endif
