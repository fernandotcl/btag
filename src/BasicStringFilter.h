#ifndef BASIC_STRING_FILTER
#define BASIC_STRING_FILTER

#include <string>

class BasicStringFilter
{
    public:
        virtual std::wstring filter(const std::wstring &input);
};

#endif
