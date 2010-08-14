/*
 * This file is part of qtagger.
 *
 * © 2010 Fernando Tarlá Cardoso Lemos
 *
 * Refer to the LICENSE file for licensing information.
 *
 */

#include <cstdlib>
#include <getopt.h>
#include <iostream>
#include <locale>
#include <string>

#include "BasicStringFilter.h"
#include "InteractiveTagger.h"
#include "StandardConsole.h"

static void print_usage(std::ostream &out)
{
    out << "\
Usage: \n\
    qtagger [--dir-rename-format] [--file-rename-format format] \\\n\
            <path> [path2] [path3] ...\n\
    qtagger --help" << std::endl;
}

int main(int argc, char **argv)
{
    // Set the global locale for case conversion purposes
    std::locale::global(std::locale(std::locale::classic(), "en_US.UTF-8", std::locale::ctype));

    struct option long_options[] = {
        {"dir-rename-format", required_argument, NULL, 'd'},
        {"file-rename-format", required_argument, NULL, 'f'},
        {"help", no_argument, NULL, 'h'},
        {NULL, 0, NULL, 0}
    };

    // Create the interactive tagger
    InteractiveTagger itag;

    // Parse the command line options
    int opt;
    while ((opt = getopt_long(argc, argv, "d:f:h", long_options, NULL)) != -1) {
        switch (opt) {
            case 'd':
                itag.set_dir_rename_format(optarg);
                break;
            case 'f':
                itag.set_file_rename_format(optarg);
                break;
            case 'h':
                print_usage(std::cout);
                return EXIT_SUCCESS;
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

    // Add the input filter
    BasicStringFilter input_filter;
    itag.set_input_filter(&input_filter);

    // Create the interactive terminal
    StandardConsole console;
    itag.set_terminal(&console);

    // Perform the interactive tagging
    itag.tag(argc - optind, (const char **)&argv[optind]);

    return EXIT_SUCCESS;
}
