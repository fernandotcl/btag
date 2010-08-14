/*
 * This file is part of qtagger.
 *
 * © 2010 Fernando Tarlá Cardoso Lemos
 *
 * Refer to the LICENSE file for licensing information.
 *
 */

#include <boost/lexical_cast.hpp>
#include <iostream>

#include "StandardConsole.h"

bool StandardConsole::ask_yes_no_question(const std::wstring &question,
        const boost::optional<bool> &default_answer)
{
    for (;;) {
        std::wcout << question;
        if (default_answer)
            std::wcout << (*default_answer ? L" [Y/n] " : L" [y/N] ");
        else
            std::wcout << L" [y/n] ";

        std::wstring a;
        std::getline(std::wcin, a);
        if (default_answer && a.empty()) {
            return *default_answer;
        }
        else if (a.size() == 1) {
            if (a[0] == 'y' || a[0] == 'Y')
                return true;
            else if (a[0] == 'n' || a[0] == 'y')
                return false;
        }
        else if (a.size() == 2 && (a[0] == 'n' || a[0] == 'N')
                && (a[1] == 'o' || a[1] == 'O')) {
            return false;
        }
        else if (a.size() == 3 && (a[0] == 'y' || a[0] == 'Y')
                && (a[1] == 'e' || a[1] == 'E')
                && (a[2] == 's' || a[2] == 'S')) {
            return true;
        }

        std::wcout << L"Please answer \"y\" or \"n\"" << std::endl;
    }
}

std::wstring StandardConsole::ask_string_question(const std::wstring &question,
        const boost::optional<std::wstring> &default_answer,
        const Validator<std::wstring> *validator)
{
    for (;;) {
        std::wcout << question << (default_answer ? L" [" + *default_answer + L"] " : L" ");

        std::wstring answer;
        std::getline(std::wcin, answer);
        if (answer.empty()) {
            if (default_answer)
                return *default_answer;
        }
        else {
            if (validator) {
                boost::optional<std::wstring> error_message;
                if (validator->validate(answer, error_message))
                    return answer;
                std::wcout << (error_message ? *error_message : L"Unknown validation error") << std::endl;
                continue;
            }
            else {
                return answer;
            }
        }

        std::wcout << L"Please answer the question" << std::endl;
    }
}

int StandardConsole::ask_number_question(const std::wstring &question,
        const boost::optional<int> &default_answer,
        const Validator<int> *validator)
{
    for (;;) {
        std::wcout << question;
        if (default_answer)
            std::wcout << L" [" << *default_answer << L"] ";
        else
            std::wcout << L" ";

        std::wstring answer;
        std::getline(std::wcin, answer);
        if (answer.empty()) {
            if (default_answer)
                return *default_answer;
        }
        else {
            int res;
            try {
                res = boost::lexical_cast<int>(answer);
            }
            catch (boost::bad_lexical_cast &) {
                std::wcout << L"Please answer the question with a valid number" << std::endl;
                continue;
            }
            if (validator) {
                boost::optional<std::wstring> error_message;
                if (validator->validate(res, error_message))
                    return res;
                std::wcout << (error_message ? *error_message : L"Unknown validation error") << std::endl;
                continue;
            }
            else {
                return res;
            }
        }

        std::wcout << L"Please answer the question" << std::endl;
    }
}

void StandardConsole::display_info_message(const std::string &message)
{
    std::cout << message << std::endl;
}

void StandardConsole::display_info_message(const std::wstring &message)
{
    std::wcout << message << std::endl;
}

void StandardConsole::display_warning_message(const std::string &message)
{
    std::cerr << L"WARNING: " << message << std::endl;
}

void StandardConsole::display_warning_message(const std::wstring &message)
{
    std::wcerr << L"WARNING: " << message << std::endl;
}
