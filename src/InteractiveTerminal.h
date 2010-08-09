/*
 * This file is part of qtagger.
 *
 * © 2010 Fernando Tarlá Cardoso Lemos
 *
 * Refer to the LICENSE file for licensing information.
 *
 */

#ifndef INTERACTIVE_TERMINAL_H
#define INTERACTIVE_TERMINAL_H

#include <boost/optional.hpp>
#include <string>

class InteractiveTerminal
{
    public:
        template<typename T> class Validator
        {
            public:
                virtual bool validate(const T &, boost::optional<std::string> &error_message) const = 0;
        };

        virtual bool ask_yes_no_question(const std::string &question,
                const boost::optional<bool> &default_answer = boost::optional<bool>()) = 0;
        virtual std::string ask_string_question(const std::string &question,
                const boost::optional<std::string> &default_answer = boost::optional<std::string>(),
                const Validator<std::string> *validator = NULL) = 0;
        virtual int ask_number_question(const std::string &question,
                const boost::optional<int> &default_answer = boost::optional<int>(),
                const Validator<int> *validator = NULL) = 0;

        virtual void display_info_message(const std::string &message) = 0;
        virtual void display_warning_message(const std::string &message) = 0;
};

#endif
