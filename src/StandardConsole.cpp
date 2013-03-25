/*
 * This file is part of btag.
 *
 * © 2010,2013 Fernando Tarlá Cardoso Lemos
 *
 * Refer to the LICENSE file for licensing information.
 *
 */

#include <boost/lexical_cast.hpp>
#include <cassert>
#include <cstdio>
#include <iostream>

#include "number_cast.h"
#include "StandardConsole.h"

EditLine *StandardConsole::m_editline;
std::wstring StandardConsole::m_editline_prompt;

wchar_t *StandardConsole::editline_prompt_callback(EditLine *editline)
{
    return const_cast<wchar_t *>(m_editline_prompt.c_str());
}

StandardConsole::StandardConsole()
{
    m_editline = el_init("btag", stdin, stdout, stderr);
    assert(m_editline);

    el_wset(m_editline, EL_EDITOR, L"emacs");
    el_wset(m_editline, EL_PROMPT, &StandardConsole::editline_prompt_callback);
}

StandardConsole::~StandardConsole()
{
    el_end(m_editline);
}

bool StandardConsole::ask_yes_no_question(const std::wstring &question,
        const boost::optional<bool> &default_answer)
{
    std::wstring yn;
    if (default_answer) {
        yn = *default_answer ? L"Y/n" : L"y/N";
    } else {
        yn = L"y/n";
    }

    std::wcout << question << " [" << yn << "]? ";
    std::wcout.flush();

    for (;;) {
        wchar_t c = 0;
        el_wgetc(m_editline, &c);
        if (c == L'\n' && default_answer) {
            std::wcout << std::endl;
            return *default_answer;
        }
        else if (c == L'y' || c == L'Y') {
            std::wcout << c << std::endl;
            return true;
        }
        else if (c == L'n' || c == L'N') {
            std::wcout << c << std::endl;
            return false;
        }
    }
}

std::wstring StandardConsole::ask_string_question(const std::wstring &question,
        const boost::optional<std::wstring> &default_answer,
        const Validator<std::wstring> *validator)
{
    for (;;) {
        m_editline_prompt = question + L": ";

        if (default_answer)
            el_wpush(m_editline, default_answer->c_str());

        int length;
        const wchar_t *response = el_wgets(m_editline, &length);
        if (length > 1)
            return std::wstring(response, length - 1);

        std::cout << "Please enter the requested information" << std::endl;
    }
}

int StandardConsole::ask_number_question(const std::wstring &question,
        const boost::optional<int> &default_answer,
        const Validator<int> *validator)
{
    for (;;) {
        m_editline_prompt = question + L": ";

        std::wstring default_answer_str;
        if (default_answer) {
            default_answer_str = number_cast(*default_answer);
            el_wpush(m_editline, default_answer_str.c_str());
        }

        int length;
        const wchar_t *response = el_wgets(m_editline, &length);
        if (length <= 1) {
            std::cout << "Please enter the requested information" << std::endl;
            continue;
        }

        int number;
        try {
            number = boost::lexical_cast<int>(std::wstring(response, length - 1));
        }
        catch (boost::bad_lexical_cast) {
            std::cout << "Please enter a valid number" << std::endl;
            continue;
        }

        if (validator) {
            boost::optional<std::wstring> error_message;
            if (validator->validate(number, error_message))
                return number;
            if (error_message)
                std::wcout << *error_message << std::endl;
            else
                std::cout << "Unknown validation error" << std::endl;
            continue;
        }
        else {
            return number;
        }
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
    std::cerr << "WARNING: " << message << std::endl;
}

void StandardConsole::display_warning_message(const std::wstring &message)
{
    std::wcerr << L"WARNING: " << message << std::endl;
}
