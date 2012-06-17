/*
 * This file is part of btag.
 *
 * © 2010-2011 Fernando Tarlá Cardoso Lemos
 *
 * Refer to the LICENSE file for licensing information.
 *
 */

#ifndef INTERACTIVE_TAGGER_H
#define INTERACTIVE_TAGGER_H

#include <boost/filesystem.hpp>
#include <boost/optional.hpp>
#include <boost/scoped_ptr.hpp>
#include <taglib/fileref.h>
#include <list>
#include <map>
#include <string>
#include <utility>

#include "BasicStringFilter.h"
#include "config.h"
#include "ConfirmationHandler.h"
#include "InteractiveTerminal.h"
#include "RenamingFilter.h"

#ifdef CUEFILE_SUPPORT
# include "CueReader.h"
#endif

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

        void set_dry_run(bool dry_run = true) { m_dry_run = dry_run; }

#ifdef CUEFILE_SUPPORT
        void set_cue_file(const std::string &filename);
#endif

        void tag(int num_paths, const char **paths);

    private:
        bool is_supported_extension(const boost::filesystem::path &path);
        std::wstring replace_tokens(const std::wstring &str, const std::map<std::wstring, std::wstring> &replacements);

        void tag_file(const boost::filesystem::path &path, ConfirmationHandler &artist_confirmation,
                ConfirmationHandler &album_confirmation, int *year, int track);
        void tag_file(const boost::filesystem::path &path);
        void tag_directory(const boost::filesystem::path &path);

        BasicStringFilter *m_input_filter, *m_output_filter;
        RenamingFilter *m_renaming_filter;
        InteractiveTerminal *m_terminal;
        boost::optional<std::wstring> m_file_rename_format, m_dir_rename_format;
        bool m_dry_run;

        std::list<TagLib::FileRef> m_unsaved_files;
        std::list<std::pair<boost::filesystem::path, boost::filesystem::path> > m_pending_renames;

        std::list<std::wstring> m_supported_extensions;

#ifdef CUEFILE_SUPPORT
        boost::scoped_ptr<CueReader> m_cue;
#endif
};

#endif
