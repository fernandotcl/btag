/*
 * This file is part of btag.
 *
 * © 2011 Fernando Tarlá Cardoso Lemos
 *
 * Refer to the LICENSE file for licensing information.
 *
 */

#include <boost/algorithm/string.hpp>
#include <cstdlib>
#include <iostream>

#include "EnglishTitleLocalizationHandler.h"
#include "TitleCapitalizationFilter.h"

/* This was initialized with a list of best-sellers found on
 * Wikipedia (though only those that we should handle well).
 * Feel free to extend it with whatever content. */
static const wchar_t *correct[] = {
    L"A Tale of Two Cities",
    L"The Lord of the Rings",
    L"The Hobbit",
    L"Dream of the Red Chamber",
    L"And Then There Were None",
    L"The Lion, the Witch and the Wardrobe",
    L"Think and Grow Rich",
    L"The Catcher in the Rye",
    L"The Alchemist",
    L"Steps to Christ",
    /* 10 */
    L"Heidi's Years of Wandering and Learning",
    L"The Common Sense Book of Baby and Child Care",
    L"Anne of Green Gables",
    L"The Name of the Rose",
    L"The Hite Report Shere Hite",
    L"The Tale of Peter Rabbit",
    L"The Ginger Man",
    L"Harry Potter and the Deathly Hallows",
    L"Jonathan Livingston Seagull",
    L"A Message to Garcia",
    /* 20 */
    L"Angels and Demons",
    L"How the Steel Was Tempered",
    L"War and Peace",
    L"You Can Heal Your Life",
    L"Kane and Abel",
    L"Sophie's World",
    L"The Very Hungry Caterpillar",
    L"The Girl with the Dragon Tattoo",
    L"The Thorn Birds",
    L"The Purpose Driven Life",
    /* 30 */
    L"One Hundred Years of Solitude",
    L"The Diary of a Young Girl",
    L"Valley of the Dolls",
    L"In His Steps: What Would Jesus Do?",
    L"The Revolt of Mamie Stover",
    L"Gone with the Wind",
    L"To Kill a Mockingbird"
};

int main(int argc, char **argv) {
    // Force an UTF-8 locale
    std::ios_base::sync_with_stdio(false);
    std::locale locale(std::locale::classic(), "C.UTF-8", std::locale::ctype);
    std::locale::global(locale);

    // Set up the capitalization filter
    EnglishTitleLocalizationHandler handler;
    TitleCapitalizationFilter filter;
    filter.set_localization_handler(&handler);

    int errors = 0;
    for (size_t i = 0; i < sizeof(correct) / sizeof(const wchar_t *); ++i) {
        // Get lowercase and uppercase versions of the string
        std::wstring correctStr(correct[i]);
        std::wstring lowerStr(boost::algorithm::to_lower_copy(correctStr));
        std::wstring upperStr(boost::algorithm::to_upper_copy(correctStr));

        // Apply the localization handler
        std::wstring lowerFiltered(filter.filter(lowerStr));
        std::wstring upperFiltered(filter.filter(upperStr));

        // Make sure they match
        if (lowerFiltered != correctStr) {
            std::cerr << "Incorrect conversion from lowercase for index " << i << std::endl;
            ++errors;
        }
        if (upperFiltered != correctStr) {
            std::cerr << "Incorrect conversion from uppercase for index " << i << std::endl;
            ++errors;
        }
    }

    return errors == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
