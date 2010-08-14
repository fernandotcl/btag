/*
 * This file is part of qtagger.
 *
 * © 2010 Fernando Tarlá Cardoso Lemos
 *
 * Refer to the LICENSE file for licensing information.
 *
 */

#ifndef STANDARD_CONSOLE_H
#define STANDARD_CONSOLE_H

#include "InteractiveTerminal.h"

class StandardConsole : public InteractiveTerminal
{
    public:
        bool ask_yes_no_question(const std::wstring &question,
                const boost::optional<bool> &default_answer);
        std::wstring ask_string_question(const std::wstring &question,
                const boost::optional<std::wstring> &default_answer,
                const Validator<std::wstring> *validator = NULL);
        int ask_number_question(const std::wstring &question,
                const boost::optional<int> &default_answer,
                const Validator<int> *validator = NULL);

        void display_info_message(const std::string &message);
        void display_info_message(const std::wstring &message);
        void display_warning_message(const std::string &message);
        void display_warning_message(const std::wstring &message);
};

#endif
