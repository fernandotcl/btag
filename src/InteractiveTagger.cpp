/*
 * This file is part of btag.
 *
 * © 2010 Fernando Tarlá Cardoso Lemos
 *
 * Refer to the LICENSE file for licensing information.
 *
 */

#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/optional.hpp>
#include <taglib/fileref.h>
#include <taglib/tag.h>
#include <cassert>
#include <cstdlib>
#include <exception>

#include "InteractiveTagger.h"
#include "validators.h"
#include "wide_string_cast.h"

namespace fs = boost::filesystem;

InteractiveTagger::InteractiveTagger()
    : m_input_filter(NULL), m_output_filter(NULL)
{
}


void InteractiveTagger::set_file_rename_format(const std::string &format)
{
    m_file_rename_format = boost::lexical_cast<std::wstring>(format);
}

void InteractiveTagger::set_dir_rename_format(const std::string &format)
{
    m_dir_rename_format = boost::lexical_cast<std::wstring>(format);
}

void InteractiveTagger::tag(int num_paths, const char **paths)
{
    assert(m_terminal);

    // Build a list with the normalized paths
    std::list<fs::wpath> path_list;
    for (int i = 0; i < num_paths; ++i) {
        char *real_path = realpath(paths[i], NULL);
        if (!real_path) {
            std::string errstr("\"");
            errstr += paths[i];
            errstr += "\" is not a valid path";
            m_terminal->display_warning_message(errstr);
            continue;
        }
        path_list.push_back(fs::wpath(boost::lexical_cast<std::wstring>(std::string(real_path))));
        free(real_path);
    }

    // Sort it and tag the paths
    path_list.sort();
    BOOST_FOREACH(const fs::wpath &path, path_list) {
        // Check if the path exists
        if (!fs::exists(path)) {
            m_terminal->display_warning_message(L"Path \"" + path.string() + L"\" not found, skipping...");
            continue;
        }

        try {
            // If it's a regular file, just tag it
            if (fs::is_regular_file(path)) {
                if (!is_supported_extension(path)) {
                    m_terminal->display_warning_message(L"Path \"" + path.string() + L"\" has no supported extension, skipping...");
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
                m_terminal->display_warning_message(L"Path \"" + path.string() + L"\" is not a regular file or directory, skipping...");
            }
        }
        catch (std::exception &e) {
            m_terminal->display_warning_message(e.what());
        }
    }

    // Save the unsaved files
    if (!m_unsaved_files.empty() && m_terminal->ask_yes_no_question(L"=== OK to save the changes to the files?")) {
        BOOST_FOREACH(TagLib::FileRef &f, m_unsaved_files) {
            m_terminal->display_info_message("Saving \"" + std::string(f.file()->name()) + "\"");
            if (!f.save())
                m_terminal->display_warning_message("Unable to save " + std::string(f.file()->name()) + "\"");
        }
    }

    // Perform the pending renames
    if (!m_pending_renames.empty() && m_terminal->ask_yes_no_question(L"=== OK to rename the files?")) {
        std::list<std::pair<fs::wpath, fs::wpath> >::const_iterator it;
        for (it = m_pending_renames.begin(); it != m_pending_renames.end(); ++it) {
            const fs::wpath &from((*it).first);
            const fs::wpath &to((*it).second);
            m_terminal->display_info_message(from.string());
            m_terminal->display_info_message(L"-> " + to.string());
            try { fs::rename(from, to); }
            catch (std::exception &e) { m_terminal->display_warning_message(e.what()); }
        }
    }

    m_terminal->display_info_message("=== All done!");
}

bool InteractiveTagger::is_supported_extension(const fs::wpath &path)
{
    static const wchar_t *supported_extensions[] = { L".mp3", L".ogg", L".flac", L".mpc", NULL };

    std::wstring extension(path.extension());
    boost::to_lower(extension);

    for (int i = 0; supported_extensions[i]; ++i) {
        if (extension == supported_extensions[i])
            return true;
    }

    return false;
}

std::wstring InteractiveTagger::replace_tokens(const std::wstring &str, const std::map<std::wstring, std::wstring> &replacements)
{
    std::wstring res;
    res.reserve(str.size() * 3);

    bool parsing_token = false;
    std::wstring current_token;
    BOOST_FOREACH(char c, str) {
        if (c == '%') {
            if (parsing_token) {
                std::map<std::wstring, std::wstring>::const_iterator it = replacements.find(current_token);
                res += it == replacements.end() ? current_token : it->second;
                current_token.erase();
            }
            parsing_token = true;
        }
        else if (parsing_token) {
            if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
                current_token += c;
            }
            else {
                std::map<std::wstring, std::wstring>::const_iterator it = replacements.find(current_token);
                res += it == replacements.end() ? current_token : it->second;
                res += c;
                parsing_token = false;
                current_token.erase();
            }
        }
        else {
            res += c;
        }
    }
    if (parsing_token) {
        std::map<std::wstring, std::wstring>::const_iterator it = replacements.find(current_token);
        res += it == replacements.end() ? current_token : it->second;
    }

    return res;
}

void InteractiveTagger::tag_file(const fs::wpath &path, std::wstring *artist, std::wstring *album, int *year, int track)
{
    m_terminal->display_info_message(L"=== Tagging \"" + path.filename() + L"\"");

    // Get a reference to the file
    TagLib::FileRef f(path.external_file_string().c_str());

    // Ask for the artist
    boost::optional<std::wstring> default_artist;
    if (artist && !artist->empty())
        default_artist = *artist;
    else if (!f.tag()->artist().isNull())
        default_artist = m_input_filter->filter(f.tag()->artist().toWString());
    std::wstring new_artist = m_terminal->ask_string_question(L"Artist:", default_artist);
    if (m_output_filter) {
        std::wstring filtered_artist = m_output_filter->filter(new_artist);
        if (m_input_filter->requires_confirmation_as_output_filter() && filtered_artist != new_artist)
            filtered_artist = m_terminal->ask_string_question(L"Artist (confirmation):", filtered_artist);
        new_artist = filtered_artist;
    }
    f.tag()->setArtist(new_artist);
    if (artist) *artist = new_artist;

    // Ask for the album
    boost::optional<std::wstring> default_album;
    if (album && !album->empty())
        default_album = *album;
    else if (!f.tag()->album().isNull())
        default_album = m_input_filter->filter(f.tag()->album().toWString());
    std::wstring new_album = m_terminal->ask_string_question(L"Album:", default_album);
    if (m_output_filter) {
        std::wstring filtered_album = m_output_filter->filter(new_album);
        if (m_input_filter->requires_confirmation_as_output_filter() && filtered_album != new_album)
            filtered_album = m_terminal->ask_string_question(L"Album (confirmation):", filtered_album);
        new_album = filtered_album;
    }
    f.tag()->setAlbum(new_album);
    if (album) *album = new_album;

    // Ask for the year
    boost::optional<int> default_year;
    if (year && *year != -1)
        default_year = *year;
    else if (f.tag()->year())
        default_year = f.tag()->year();
    YearValidator year_validator;
    int new_year = m_terminal->ask_number_question(L"Year:", default_year, &year_validator);
    f.tag()->setYear(new_year);
    if (year) *year = new_year;

    // Ask for the track
    if (track == -1)
        track = f.tag()->track();
    TrackValidator track_validator;
    int new_track = m_terminal->ask_number_question(L"Track:",
            track > 0 ? track : boost::optional<int>(), &track_validator);
    f.tag()->setTrack(new_track);

    // Ask for the song title
    boost::optional<std::wstring> default_title;
    if (!f.tag()->title().isNull())
        default_title = m_input_filter->filter(f.tag()->title().toWString());
    std::wstring new_title = m_terminal->ask_string_question(L"Title:", default_title);
    if (m_output_filter) {
        std::wstring filtered_title = m_output_filter->filter(new_title);
        if (m_input_filter->requires_confirmation_as_output_filter() && filtered_title != new_title)
            filtered_title = m_terminal->ask_string_question(L"Title (confirmation):", filtered_title);
        new_title = filtered_title;
    }
    f.tag()->setTitle(new_title);

    // Reset the comment and genre fields
    f.tag()->setComment(TagLib::String::null);
    f.tag()->setGenre(TagLib::String::null);

    // Add it to the list of unsaved files
    m_unsaved_files.push_back(f);

    // Add it to the list of pending renames based on the supplied format
    if (m_file_rename_format) {
        std::map<std::wstring, std::wstring> tokens;
        tokens[L"artist"] = m_renaming_filter->filter(new_artist);
        tokens[L"album"] = m_renaming_filter->filter(new_album);
        tokens[L"year"] = boost::lexical_cast<std::wstring>(new_year);
        std::wstring track_str(boost::lexical_cast<std::wstring>(new_track));
        if (track_str.size() == 1) track_str = L"0" + track_str;
        tokens[L"track"] = track_str;
        tokens[L"title"] = m_renaming_filter->filter(new_title);
        fs::wpath new_path = path.parent_path();
        new_path /= replace_tokens(*m_file_rename_format, tokens) + boost::to_lower_copy(path.extension());
        if (new_path != path)
            m_pending_renames.push_back(std::pair<fs::wpath, fs::wpath>(path, new_path));
    }
}

void InteractiveTagger::tag_directory(const fs::wpath &path)
{
    m_terminal->display_info_message(L"=== Entering \"" + path.string() + L"\"");

    // Create lists of files and subdirectories
    std::list<fs::wpath> file_list, dir_list;
    fs::wdirectory_iterator end_it;
    for (fs::wdirectory_iterator it(path); it != end_it; ++it) {
        // Normalize the path (needed here so that we follow symlinks)
        char *real_path = realpath(it->path().external_directory_string().c_str(), NULL);
        if (!real_path) {
            m_terminal->display_warning_message(L"\"" + it->filename() + L"\" is not a valid path, skipping...");
            continue;
        }
        fs::wpath boost_path(boost::lexical_cast<std::wstring>(real_path));
        free(real_path);

        // Add to the right list
        if (fs::is_regular_file(boost_path)) {
            if (is_supported_extension(boost_path))
                file_list.push_back(boost_path);
            else
                m_terminal->display_warning_message(L"\"" + it->filename() + L"\" has no supported extension, skipping...");
        }
        else if (fs::is_directory(boost_path)) {
            dir_list.push_back(boost_path);
        }
        else {
            m_terminal->display_warning_message(L"\"" + boost_path.filename() + L"\" is not a regular file or directory, skipping...");
        }
    }

    // Sort those lists
    file_list.sort();
    dir_list.sort();

    // Tag all individual files
    std::wstring artist, album;
    int year = -1;
    if (!file_list.empty()) {
        int track = 1;
        BOOST_FOREACH(const fs::wpath &p, file_list)
            tag_file(p, &artist, &album, &year, track++);
    }

    // We'll ask confirmation to descend into the subdirectories only if there are files
    BOOST_FOREACH(const fs::wpath &p, dir_list) {
        if (file_list.empty() || m_terminal->ask_yes_no_question(L"Descend into subdirectory \""
                    + boost::lexical_cast<std::wstring>(p.filename()) + L"\"?", false))
            tag_directory(p);
    }

    // Add it to the list of pending renames based on the supplied format
    if (!artist.empty() && m_dir_rename_format) {
        std::map<std::wstring, std::wstring> tokens;
        tokens[L"artist"] = m_renaming_filter->filter(artist);
        tokens[L"album"] = m_renaming_filter->filter(album);
        tokens[L"year"] = boost::lexical_cast<std::wstring>(year);
        fs::wpath new_path = path.parent_path();
        new_path /= replace_tokens(*m_dir_rename_format, tokens);
        if (new_path != path)
            m_pending_renames.push_back(std::pair<fs::wpath, fs::wpath>(path, new_path));
    }
}
