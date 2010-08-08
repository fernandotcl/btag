#ifndef INTERACTIVE_TAGGER_H
#define INTERACTIVE_TAGGER_H

#include <boost/filesystem.hpp>
#include <boost/optional.hpp>
#include <taglib/fileref.h>
#include <list>
#include <string>
#include <utility>

#include "InteractiveTerminal.h"

class InteractiveTagger
{
    public:
        void set_terminal(InteractiveTerminal *terminal) { m_terminal = terminal; }
        void tag(int num_paths, const char **paths);

    private:
        bool is_supported_extension(const boost::filesystem::path &path);

        void tag_file(const boost::filesystem::path &path, std::string *artist = NULL,
                std::string *album = NULL, int *year = NULL, int track = -1);
        void tag_directory(const boost::filesystem::path &path);

        InteractiveTerminal *m_terminal;

        std::list<TagLib::FileRef> m_unsaved_files;
        std::list<std::pair<boost::filesystem::path, std::string> > m_pending_renames;
};

#endif
