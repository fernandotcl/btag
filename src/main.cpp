/*
 * This file is part of qtagger.
 *
 * © 2010 Fernando Tarlá Cardoso Lemos
 *
 * Refer to the LICENSE file for licensing information.
 *
 */

#include <boost/scoped_ptr.hpp>
#include <cstdlib>
#include <getopt.h>
#include <iostream>
#include <locale>
#include <string>

#include "BasicStringFilter.h"
#include "InteractiveTagger.h"
#include "SimpleCapitalizationFilter.h"
#include "StandardConsole.h"
#include "TitleCapitalizationFilter.h"

static void print_usage(std::ostream &out)
{
    out << "\
Usage: \n\
    qtagger [--dir-rename-format] [--file-rename-format format] \\\n\
            [--filter filter] <path> [path2] [path3] ...\n\
    qtagger [--dir-rename-format] [--file-rename-format format] \\\n\
            [--input-filter filter] [--output-filter filter] \\\n\
            <path> [path2] [path3] ...\n\
    qtagger --help\n\
\n\
Available filters: basic, first_upper, lower, title, upper\n\
\n\
Example:\n\
    qtagger --file-rename-format '%track. %album' \\\n\
        --dir-rename-format '%album (%year)' \\\n\
        --filter title\n\
" << std::endl;
}

static BasicStringFilter *select_string_filter(const std::string &filter)
{
    if (filter == "basic")
        return new BasicStringFilter;
    else if (filter == "title")
        return new TitleCapitalizationFilter;
    else if (filter == "lower")
        return new SimpleCapitalizationFilter(SimpleCapitalizationFilter::CAPITALIZATION_MODE_ALL_LOWER);
    else if (filter == "upper")
        return new SimpleCapitalizationFilter(SimpleCapitalizationFilter::CAPITALIZATION_MODE_ALL_UPPER);
    else if (filter == "first_upper")
        return new SimpleCapitalizationFilter(SimpleCapitalizationFilter::CAPITALIZATION_MODE_FIRST_UPPER);
    else
        return NULL;
}

int main(int argc, char **argv)
{
    // Set the global locale for case conversion purposes
    const char *locale_name = getenv("LANG");
    if (locale_name) {
        std::ios_base::sync_with_stdio(false);
        std::locale locale(std::locale::classic(), locale_name, std::locale::ctype);
        std::locale::global(locale);
        std::wcout.imbue(locale);
        std::wcin.imbue(locale);
    }
    else {
        std::cerr << "WARNING: $LANG is not defined" << std::endl;
    }

    struct option long_options[] = {
        {"dir-rename-format", required_argument, NULL, 'd'},
        {"input-filter", required_argument, NULL, 'i'},
        {"file-rename-format", required_argument, NULL, 'r'},
        {"filter", required_argument, NULL, 'f'},
        {"help", no_argument, NULL, 'h'},
        {"output-filter", required_argument, NULL, 'o'},
        {NULL, 0, NULL, 0}
    };

    // Create the interactive tagger
    InteractiveTagger itag;
    boost::scoped_ptr<BasicStringFilter> input_filter, output_filter;

    // Parse the command line options
    int opt;
    while ((opt = getopt_long(argc, argv, "d:i:f:o:h", long_options, NULL)) != -1) {
        switch (opt) {
            case 'd':
                itag.set_dir_rename_format(optarg);
                break;
            case 'f':
                input_filter.reset(select_string_filter(optarg));
                output_filter.reset(select_string_filter(optarg));
                if (!input_filter.get() || !output_filter.get()) {
                    print_usage(std::cerr);
                    return EXIT_FAILURE;
                }
                break;
            case 'i':
                input_filter.reset(select_string_filter(optarg));
                if (!input_filter.get()) {
                    print_usage(std::cerr);
                    return EXIT_FAILURE;
                }
                break;
            case 'h':
                print_usage(std::cout);
                return EXIT_SUCCESS;
            case 'o':
                output_filter.reset(select_string_filter(optarg));
                if (!output_filter.get()) {
                    print_usage(std::cerr);
                    return EXIT_FAILURE;
                }
                break;
            case 'r':
                itag.set_file_rename_format(optarg);
                break;
            default:
                print_usage(std::cerr);
                return EXIT_FAILURE;
        }
    }

    // We need at least one path
    if (optind >= argc) {
        print_usage(std::cerr);
        return EXIT_FAILURE;
    }

    // Add the filters
    if (!input_filter.get())
        input_filter.reset(new BasicStringFilter);
    itag.set_input_filter(input_filter.get());
    itag.set_output_filter(output_filter.get());

    // Create the interactive terminal
    StandardConsole console;
    itag.set_terminal(&console);

    // Perform the interactive tagging
    itag.tag(argc - optind, (const char **)&argv[optind]);

    return EXIT_SUCCESS;
}
