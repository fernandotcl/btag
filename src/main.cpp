/*
 * This file is part of btag.
 *
 * © 2010-2012 Fernando Tarlá Cardoso Lemos
 *
 * Refer to the LICENSE file for licensing information.
 *
 */

#include <boost/optional.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/locale.hpp>
#include <cstdlib>
#include <cstring>
#include <getopt.h>
#include <iostream>
#include <locale>
#include <string>

#include "BasicStringFilter.h"
#include "config.h"
#include "ConservativeRenamingFilter.h"
#include "EnglishTitleLocalizationHandler.h"
#include "InteractiveTagger.h"
#include "RenamingFilter.h"
#include "SimpleCapitalizationFilter.h"
#include "SpanishTitleLocalizationHandler.h"
#include "StandardConsole.h"
#include "TitleCapitalizationFilter.h"
#include "TitleLocalizationHandler.h"
#include "UnixRenamingFilter.h"

static void print_usage(std::ostream &out)
{
    out << "\
Usage: \n\
    " PROGRAM_NAME " [options] path1 [path2] [path3] ...\n\
    " PROGRAM_NAME " --help\
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
    else if (filter == "first-upper")
        return new SimpleCapitalizationFilter(SimpleCapitalizationFilter::CAPITALIZATION_MODE_FIRST_UPPER);
    else
        return NULL;
}

static TitleLocalizationHandler *select_title_localization_handler(const std::string &locale)
{
    if (locale == "en")
        return new EnglishTitleLocalizationHandler;
    else if (locale == "es")
        return new SpanishTitleLocalizationHandler;
    else
        return NULL;
}

static RenamingFilter *select_renaming_filter(const std::string &filter)
{
    if (filter == "unix")
        return new UnixRenamingFilter;
    else if (filter == "conservative")
        return new ConservativeRenamingFilter;
    else
        return NULL;
}

int main(int argc, char **argv)
{
    // Set the global locale for case conversion purposes
    std::ios_base::sync_with_stdio(false);
    boost::locale::generator gen;
    std::locale locale(gen(""));
    std::locale::global(locale);
    std::wcout.imbue(locale);
    std::wcin.imbue(locale);

    struct option long_options[] = {
        {"always-ask-track", no_argument, NULL, 0},
#ifdef CUESHEET_SUPPORT
        {"cue-sheet", required_argument, NULL, 'C'},
        {"cue-sheet-encoding", required_argument, NULL, 0},
#endif
        {"dir-rename-format", required_argument, NULL, 'd'},
        {"dry-run", no_argument, NULL, 'D'},
        {"input-filter", required_argument, NULL, 'i'},
        {"file-rename-format", required_argument, NULL, 'r'},
        {"filter", required_argument, NULL, 'f'},
        {"help", no_argument, NULL, 'h'},
        {"output-filter", required_argument, NULL, 'o'},
        {"renaming-filter", required_argument, NULL, 'n'},
        {"title-locale", required_argument, NULL, 't'},
        {NULL, 0, NULL, 0}
    };

    // Create the interactive tagger
    InteractiveTagger itag;
    boost::scoped_ptr<BasicStringFilter> input_filter, output_filter;
    boost::scoped_ptr<TitleLocalizationHandler> title_localization_handler;
    boost::scoped_ptr<RenamingFilter> renaming_filter;

    // Create the interactive terminal
    StandardConsole console;
    itag.set_terminal(&console);

    // Cue sheet filename and encoding
    boost::optional<std::string> cue_filename;
    std::string cue_encoding("ISO-8859-1");

    // Parse the command line options
    int opt, option_index;
#ifdef CUESHEET_SUPPORT
    while ((opt = getopt_long(argc, argv, "C:Dd:i:f:o:hn:r:t:", long_options, &option_index)) != -1) {
#else
    while ((opt = getopt_long(argc, argv, "Dd:i:f:o:hn:r:t:", long_options, &option_index)) != -1) {
#endif
        switch (opt) {
#ifdef CUESHEET_SUPPORT
            case 'C':
                cue_filename = optarg;
                break;
#endif
            case 'D':
                itag.set_dry_run();
                break;
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
            case 'n':
                renaming_filter.reset(select_renaming_filter(optarg));
                if (!renaming_filter.get()) {
                    print_usage(std::cerr);
                    return EXIT_FAILURE;
                }
                break;
            case 'r':
                itag.set_file_rename_format(optarg);
                break;
            case 't':
                title_localization_handler.reset(select_title_localization_handler(optarg));
                if (!title_localization_handler.get()) {
                    print_usage(std::cerr);
                    return EXIT_FAILURE;
                }
                break;
            case 0:
                if (!strcmp(long_options[option_index].name, "always-ask-track"))
                    itag.set_ask_track();
#ifdef CUESHEET_SUPPORT
                else if (!strcmp(long_options[option_index].name, "cue-sheet-encoding"))
                    cue_encoding = optarg;
#endif
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

#ifdef CUESHEET_SUPPORT
    // Load the cue sheet if needed
    if (cue_filename)
        itag.load_cue_sheet(*cue_filename, cue_encoding);
#endif

    // Add the title localization handler
    if (!title_localization_handler.get())
        title_localization_handler.reset(new EnglishTitleLocalizationHandler);
    TitleCapitalizationFilter *filter = dynamic_cast<TitleCapitalizationFilter *>(input_filter.get());
    if (filter) filter->set_localization_handler(title_localization_handler.get());
    filter = dynamic_cast<TitleCapitalizationFilter *>(output_filter.get());
    if (filter) filter->set_localization_handler(title_localization_handler.get());

    // Add the filters
    if (!input_filter.get())
        input_filter.reset(new BasicStringFilter);
    itag.set_input_filter(input_filter.get());
    itag.set_output_filter(output_filter.get());

    // Add the renaming filter
    if (!renaming_filter.get())
        renaming_filter.reset(new UnixRenamingFilter);
    itag.set_renaming_filter(renaming_filter.get());

    // Perform the interactive tagging
    itag.tag(argc - optind, (const char **)&argv[optind]);

    return EXIT_SUCCESS;
}
