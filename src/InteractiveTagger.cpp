#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <boost/optional.hpp>
#include <taglib/fileref.h>
#include <taglib/tag.h>
#include <cassert>
#include <cstdlib>
#include <exception>

#include "InteractiveTagger.h"
#include "validators.h"

namespace fs = boost::filesystem;

void InteractiveTagger::tag(int num_paths, const char **paths)
{
    assert(m_terminal);

    // Build a list with the normalized paths
    std::list<fs::path> path_list;
    for (int i = 0; i < num_paths; ++i) {
        char *real_path = realpath(paths[i], NULL);
        if (!real_path) {
            std::string errstr("\"");
            errstr += paths[i];
            errstr += "\" is not a valid path";
            m_terminal->display_warning_message(errstr);
            continue;
        }
        path_list.push_back(fs::path(real_path));
        free(real_path);
    }

    // Sort it and tag the paths
    path_list.sort();
    BOOST_FOREACH(const fs::path &path, path_list) {
        // Check if the path exists
        if (!fs::exists(path)) {
            m_terminal->display_warning_message("Path \""+ path.string() + "\" not found, skipping...");
            continue;
        }

        try {
            // If it's a regular file, just tag it
            if (fs::is_regular_file(path)) {
                if (!is_supported_extension(path)) {
                    m_terminal->display_warning_message("Path \"" + path.string() + "\" has no supported extension, skipping...");
                    continue;
                }
                tag_file(path);
            }

            // If it's a directory, tag its contents
            else if (fs::is_directory(path)) {
                tag_directory(path);
            }

            // It's none of the above, do something about it
            else {
                m_terminal->display_warning_message("Path \"" + path.string() + "\" is not a regular file or directory, skipping...");
            }
        }
        catch (std::exception &e) {
            m_terminal->display_warning_message(e.what());
        }
    }

    // Save the unsaved files
    if (!m_unsaved_files.empty() && m_terminal->ask_yes_no_question("=== OK to save the changes to the files?")) {
        BOOST_FOREACH(TagLib::FileRef &f, m_unsaved_files) {
            m_terminal->display_info_message("Saving \"" + std::string(f.file()->name()) + "\"");
            if (!f.save())
                m_terminal->display_warning_message("Unable to save " + std::string(f.file()->name()) + "\"");
        }
    }

    // Perform the pending renames
    if (!m_pending_renames.empty() && m_terminal->ask_yes_no_question("=== OK to rename the files?")) {
        std::list<std::pair<fs::path, std::string> >::const_iterator it;
        for (it = m_pending_renames.begin(); it != m_pending_renames.end(); ++it) {
            const fs::path &from((*it).first);
            fs::path to((*it).second);
            if (from != to)
                fs::rename(from, to);
        }
    }

    m_terminal->display_info_message("=== All done!");
}

bool InteractiveTagger::is_supported_extension(const fs::path &path)
{
    static const char *supported_extensions[] = { ".mp3", ".ogg", ".flac", ".mpc", NULL };

    std::string extension(path.extension());
    boost::to_lower(extension);

    for (int i = 0; supported_extensions[i]; ++i) {
        if (extension == supported_extensions[i])
            return true;
    }

    return false;
}

void InteractiveTagger::tag_file(const boost::filesystem::path &path,
        std::string *artist, std::string *album, int *year, int track)
{
    // TODO Implement string filters

    m_terminal->display_info_message("=== Tagging \"" + path.filename() + "\"");

    // Get a reference to the file
    TagLib::FileRef f(path.string().c_str());

    // Ask for the artist
    boost::optional<std::string> default_artist;
    if (artist && !artist->empty())
        default_artist = *artist;
    else if (!f.tag()->artist().isNull())
        default_artist = f.tag()->artist().to8Bit();
    std::string new_artist = m_terminal->ask_string_question("Artist:", default_artist);
    f.tag()->setArtist(new_artist);
    if (artist) *artist = new_artist;

    // Ask for the album
    boost::optional<std::string> default_album;
    if (album && !album->empty())
        default_album = *album;
    else if (!f.tag()->album().isNull())
        default_album = f.tag()->album().to8Bit();
    std::string new_album = m_terminal->ask_string_question("Album:", default_album);
    f.tag()->setAlbum(new_album);
    if (album) *album = new_album;

    // Ask for the year
    boost::optional<int> default_year;
    if (year && *year != -1)
        default_year = *year;
    else if (f.tag()->year())
        default_year = f.tag()->year();
    YearValidator year_validator;
    int new_year = m_terminal->ask_number_question("Year:", default_year, &year_validator);
    f.tag()->setYear(new_year);
    if (year) *year = new_year;

    // Ask for the track
    if (track == -1)
        track = f.tag()->track();
    TrackValidator track_validator;
    f.tag()->setTrack(m_terminal->ask_number_question("Track:",
                track > 0 ? track : boost::optional<int>(), &track_validator));

    // Ask for the song title
    boost::optional<std::string> default_title;
    if (!f.tag()->title().isNull())
        default_title = f.tag()->title().to8Bit();
    std::string new_title = m_terminal->ask_string_question("Title:", default_title);
    f.tag()->setTitle(new_title);

    // Reset the comment and genre fields
    f.tag()->setComment(TagLib::String::null);
    f.tag()->setGenre(TagLib::String::null);

    // Add it to the list of unsaved files
    m_unsaved_files.push_back(f);

    // Add it to the list of pending renames based on the supplied format
    // TODO
}

void InteractiveTagger::tag_directory(const fs::path &path)
{
    m_terminal->display_info_message("=== Entering \"" + path.string() + "\"");

    // Create lists of files and subdirectories
    std::list<fs::path> file_list, dir_list;
    fs::directory_iterator end_it;
    for (fs::directory_iterator it(path); it != end_it; ++it) {
        // Normalize the path (needed here so that we follow symlinks)
        char *real_path = realpath(it->string().c_str(), NULL);
        if (!real_path) {
            m_terminal->display_warning_message("\"" + it->filename() + "\" is not a valid path, skipping...");
            continue;
        }
        fs::path boost_path(real_path);
        free(real_path);

        // Add to the right list
        if (fs::is_regular_file(boost_path)) {
            if (is_supported_extension(boost_path))
                file_list.push_back(boost_path);
            else
                m_terminal->display_warning_message("\"" + it->filename() + "\" has no supported extension, skipping...");
        }
        else if (fs::is_directory(boost_path)) {
            dir_list.push_back(boost_path);
        }
        else {
            m_terminal->display_warning_message("\"" + boost_path.filename() + "\" is not a regular file or directory, skipping...");
        }
    }

    // Sort those lists
    file_list.sort();
    dir_list.sort();

    // Tag all individual files
    std::string artist, album;
    int year = -1;
    if (!file_list.empty()) {
        int track = 1;
        std::string artist, album;
        BOOST_FOREACH(const fs::path &p, file_list)
            tag_file(p, &artist, &album, &year, track++);
    }

    // We'll ask confirmation to descend into the subdirectories only if there are files
    BOOST_FOREACH(const fs::path &p, dir_list) {
        if (file_list.empty() || m_terminal->ask_yes_no_question("Descend into subdirectory \"" + p.filename() + "\"?", false))
            tag_directory(p);
    }

    // Rename this directory based on the supplied format
    // TODO
}
