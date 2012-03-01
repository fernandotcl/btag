/*
 * This file is part of btag.
 *
 * © 2012 Fernando Tarlá Cardoso Lemos
 *
 * Refer to the LICENSE file for licensing information.
 *
 */

#include "SpanishTitleLocalizationHandler.h"
#include "titles_base.h"

int main(int argc, char **argv)
{
    SpanishTitleLocalizationHandler handler;
    return run_title_capitalization_tests(handler, argc, argv);
}
