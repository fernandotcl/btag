/*
 * This file is part of btag.
 *
 * © 2011 Fernando Tarlá Cardoso Lemos
 *
 * Refer to the LICENSE file for licensing information.
 *
 */

#include <boost/optional.hpp>
#include <string>

#include "BasicStringFilter.h"
#include "ConfirmationHandler.h"
#include "InteractiveTerminal.h"

ConfirmationHandler::ConfirmationHandler(InteractiveTerminal &terminal,
        BasicStringFilter *input_filter, BasicStringFilter *output_filter)
    : m_terminal(terminal), m_input_filter(input_filter), m_output_filter(output_filter)
{
}

void ConfirmationHandler::set_local_default(const std::wstring &local)
{
    // Set the filtered local default
    m_local_def = m_input_filter ? m_input_filter->filter(local) : local;
}

void ConfirmationHandler::reset()
{
    // Erase the local default and the previous answer
    m_local_def.erase();
    m_output.erase();
}

bool ConfirmationHandler::do_ask(const std::wstring &question)
{
    // Ask the question
    boost::optional<std::wstring> default_answer;
    if (!m_global_def.empty())
        default_answer = m_global_def;
    else if (!m_local_def.empty())
        default_answer = m_local_def;
    std::wstring answer = m_terminal.ask_string_question(question, default_answer);

    // The answer complies if it's the same as the previously entered answer
    // (the user confirmed it) or if it's the same as the default answer
    m_complies = answer == default_answer || answer == m_output;
    m_output = answer;
    if (m_complies)
        return true;

    // Check if the output filter rejects it
    if (m_output_filter) {
        std::wstring filtered = m_output_filter->filter(answer);
        if (m_output_filter->requires_confirmation_as_output_filter() && answer != filtered) {
            m_local_def = filtered;
            return (m_complies = false);
        }
    }

    // Nope, so it's good
    return (m_complies = true);
}

bool ConfirmationHandler::ask(const std::wstring &question)
{
    // If the answer complies, we got a global default
    if (do_ask(question)) {
        m_global_def = m_output;
        return true;
    }
    return false;
}
