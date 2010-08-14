/*
 * This file is part of qtagger.
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

class InteractiveTagger
{
    public:
        void set_terminal(InteractiveTerminal *terminal) { m_terminal = terminal; }
        void set_file_rename_format(const std::string &format) { m_file_rename_format = format; }
        void set_dir_rename_format(const std::string &format) { m_dir_rename_format = format; }
        void set_input_filter(BasicStringFilter *filter) { m_input_filter = filter; }

        void tag(int num_paths, const char **paths);

    private:
        bool is_supported_extension(const boost::filesystem::path &path);
        std::string replace_tokens(const std::string &str,
                const std::map<std::string, std::string> &replacements);

        void tag_file(const boost::filesystem::path &path, std::wstring *artist = NULL,
                std::wstring *album = NULL, int *year = NULL, int track = -1);
        void tag_directory(const boost::filesystem::path &path);

        BasicStringFilter *m_input_filter;
        BasicStringFilter m_output_filter;
        InteractiveTerminal *m_terminal;
        boost::optional<std::string> m_file_rename_format, m_dir_rename_format;

        std::list<TagLib::FileRef> m_unsaved_files;
        std::list<std::pair<boost::filesystem::path, boost::filesystem::path> > m_pending_renames;
};

#endif
