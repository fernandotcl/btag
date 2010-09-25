/*
 * This file is part of btag.
 *
 * © 2010 Fernando Tarlá Cardoso Lemos
 *
 * Refer to the LICENSE file for licensing information.
 *
 */

#ifndef INTERACTIVE_TAGGER_H
#define INTERACTIVE_TAGGER_H

#include <boost/filesystem.hpp>
#include <boost/optional.hpp>
#include <taglib/fileref.h>
#include <list>
#include <map>
#include <string>
#include <utility>

#include "BasicStringFilter.h"
#include "InteractiveTerminal.h"
#include "RenamingFilter.h"

class InteractiveTagger
{
    public:
        InteractiveTagger();

        void set_terminal(InteractiveTerminal *terminal) { m_terminal = terminal; }
        void set_file_rename_format(const std::string &format);
        void set_dir_rename_format(const std::string &format);

        void set_input_filter(BasicStringFilter *filter) { m_input_filter = filter; }
        void set_output_filter(BasicStringFilter *filter) { m_output_filter = filter; }
        void set_renaming_filter(RenamingFilter *filter) { m_renaming_filter = filter; }

        void tag(int num_paths, const char **paths);

    private:
        bool is_supported_extension(const boost::filesystem::wpath &path);
        std::wstring replace_tokens(const std::wstring &str, const std::map<std::wstring, std::wstring> &replacements);

        void tag_file(const boost::filesystem::wpath &path, std::wstring *artist = NULL,
                std::wstring *album = NULL, int *year = NULL, int track = -1);
        void tag_directory(const boost::filesystem::wpath &path);

        BasicStringFilter *m_input_filter, *m_output_filter;
        RenamingFilter *m_renaming_filter;
        InteractiveTerminal *m_terminal;
        boost::optional<std::wstring> m_file_rename_format, m_dir_rename_format;

        std::list<TagLib::FileRef> m_unsaved_files;
        std::list<std::pair<boost::filesystem::wpath, boost::filesystem::wpath> > m_pending_renames;
};

#endif
