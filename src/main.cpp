#include <cstdlib>
#include <getopt.h>
#include <iostream>
#include <string>

#include "InteractiveTagger.h"
#include "StandardConsole.h"

static void print_usage(std::ostream &out)
{
    out << "\
Usage: \n\
    qtagger <path> [path2] [path3] ...\n\
    qtagger --help" << std::endl;
}

int main(int argc, char **argv)
{
    struct option long_options[] = {
        {"help", no_argument, NULL, 'h'},
        {NULL, 0, NULL, 0}
    };

    // Create the interactive tagger
    InteractiveTagger itag;

    // Parse the command line options
    int opt;
    while ((opt = getopt_long(argc, argv, "c:h", long_options, NULL)) != -1) {
        switch (opt) {
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

    // Create the interactive terminal
    StandardConsole console;
    itag.set_terminal(&console);

    // Perform the interactive tagging
    itag.tag(argc - optind, (const char **)&argv[optind]);

    return EXIT_SUCCESS;
}
