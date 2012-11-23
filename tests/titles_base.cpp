/*
 * This file is part of btag.
 *
 * © 2011-2012 Fernando Tarlá Cardoso Lemos
 *
 * Refer to the LICENSE file for licensing information.
 *
 */

#include <boost/algorithm/string.hpp>
#include <cstdlib>
#include <fstream>
#include <iostream>

#include "titles_base.h"

int run_title_capitalization_tests(TitleLocalizationHandler &handler, int argc, char **argv)
{
    // Find the path to the test data
    if (argc != 2) {
        std::cerr << "You need to supply the path to test data file in the command line" << std::endl;
        return EXIT_FAILURE;
    }

    // Open the test data file
    std::wifstream f(argv[1]);
    if (!f.is_open()) {
        std::cerr << "Failed to open the test data file" << std::endl;
        return EXIT_FAILURE;
    }

    // Force an UTF-8 locale
    std::ios_base::sync_with_stdio(false);
    std::locale locale(std::locale::classic(), "C.UTF-8", std::locale::ctype);
    f.imbue(locale);
    std::wcerr.imbue(locale);

    // Set up the capitalization filter
    TitleCapitalizationFilter filter;
    filter.set_localization_handler(&handler);

    int errors = 0, correct_lines = 0, lineno = 1;
    std::wstring line;
    for (;;) {
        // Get a line
        std::getline(f, line);
        if (f.eof())
            break;
        else if (f.fail()) {
            std::wcerr << L"Error reading line from file" << std::endl;
            return EXIT_FAILURE;
        }

        // Check if we can skip it
        std::wstring::size_type first = line.find_first_not_of(L" \t\n\r");
        if (first == std::wstring::npos || line[first] == L'#') {
            ++lineno;
            continue;
        }

        // Trim it
        std::wstring::size_type last = line.find_last_not_of(L" \t\n\r");
        std::wstring correctStr(line.substr(first, last - first + 1));

        // Get lowercase and uppercase versions of the string
        std::wstring lowerStr(boost::algorithm::to_lower_copy(correctStr));
        std::wstring upperStr(boost::algorithm::to_upper_copy(correctStr));

        // Apply the localization handler
        std::wstring lowerFiltered(filter.filter(lowerStr));
        std::wstring upperFiltered(filter.filter(upperStr));

        // Make sure they match
        bool has_error = false;
        if (lowerFiltered != correctStr) {
            std::wcerr << lineno << L": Incorrect conversion from lowercase" << std::endl;
            has_error = true;
            ++errors;
        }
        if (upperFiltered != correctStr) {
            std::wcerr << lineno << L": Incorrect conversion from uppercase" << std::endl;
            has_error = true;
            ++errors;
        }

        // Increment the counters
        ++lineno;
        if (!has_error)
            ++correct_lines;
    }

    return correct_lines > 10 && errors == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
