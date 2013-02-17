/*
 * This file is part of btag.
 *
 * © 2011 Fernando Tarlá Cardoso Lemos
 *
 * Refer to the LICENSE file for licensing information.
 *
 */

#ifndef CONFIRMATION_HANDLER_H
#define CONFIRMATION_HANDLER_H

#include <string>

#include "BasicStringFilter.h"
#include "InteractiveTerminal.h"

class ConfirmationHandler
{
    public:
        ConfirmationHandler(InteractiveTerminal &terminal,
                BasicStringFilter *input_filter = NULL, BasicStringFilter *output_filter = NULL);

        void reset();
        void set_local_default(const std::wstring &local);
        bool ask(const std::wstring &question);

        bool complies() const { return m_complies; }
        const std::wstring &answer() const { return m_output; }

    private:
        bool do_ask(const std::wstring &question);

        InteractiveTerminal &m_terminal;
        BasicStringFilter *m_input_filter, *m_output_filter;

        std::wstring m_global_def, m_local_def, m_output;
        bool m_complies;
};

#endif
