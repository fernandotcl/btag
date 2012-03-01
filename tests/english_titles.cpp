/*
 * This file is part of btag.
 *
 * © 2012 Fernando Tarlá Cardoso Lemos
 *
 * Refer to the LICENSE file for licensing information.
 *
 */

#include "EnglishTitleLocalizationHandler.h"
#include "titles_base.h"

int main(int argc, char **argv)
{
    EnglishTitleLocalizationHandler handler;
    return run_title_capitalization_tests(handler, argc, argv);
}
