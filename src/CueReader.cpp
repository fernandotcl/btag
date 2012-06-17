/*
 * This file is part of btag.
 *
 * © 2012 Fernando Tarlá Cardoso Lemos
 *
 * Refer to the LICENSE file for licensing information.
 *
 */

#include <boost/lexical_cast.hpp>
#include <cstdio>
#include <stdexcept>

#include "CueReader.h"
#include "validators.h"

CueReader::CueReader(const std::string &filename)
    : m_cd(NULL)
{
    // Get a file pointer
    FILE *fp = fopen(filename.c_str(), "r");
    if (!fp)
        throw std::runtime_error("Unable to open CUE file for reading");

    // Parse it
    m_cd = cue_parse_file(fp);
    fclose(fp);
    if (!m_cd)
        throw std::runtime_error("Unable to parse CUE file");

    // Fetch the performing artist
    Cdtext *cdt = cd_get_cdtext(m_cd);
    if (cdt)
        m_artist = cdtext_string(cdt, PTI_PERFORMER);
}

CueReader::~CueReader()
{
    if (m_cd)
        cd_delete(m_cd);
}

boost::optional<std::wstring> CueReader::cdtext_string(Cdtext *cdt, Pti pti)
{
    const char *value = cdtext_get(pti, cdt);
    return value ? boost::lexical_cast<std::wstring>(value) : boost::optional<std::wstring>();
}

boost::optional<std::wstring> CueReader::album()
{
    Cdtext *cdt = cd_get_cdtext(m_cd);
    if (cdt) {
        boost::optional<std::wstring> album = cdtext_string(cdt, PTI_TITLE);
        return album;
    }
    else {
        return boost::optional<std::wstring>();
    }
}

boost::optional<int> CueReader::year()
{
    // Get the REM entry
    Rem *rem = cd_get_rem(m_cd);
    if (!rem)
        return boost::optional<int>();

    // Get the DATE entry
    const char *value = rem_get(REM_DATE, rem);
    if (!value)
        return boost::optional<int>();

    // Find the year
    int year;
    try {
        year = boost::lexical_cast<int>(value);
    }
    catch (boost::bad_lexical_cast &) {
        return boost::optional<int>();
    }

    // Validate and return it
    YearValidator validator;
    boost::optional<std::wstring> error_msg;
    return validator.validate(year, error_msg) ? year : boost::optional<int>();
}

boost::optional<std::wstring> CueReader::artist_for_track(int track)
{
    // Get the track
    Track *t = cd_get_track(m_cd, track);
    if (!t)
        return m_artist;

    // Get the CD-TEXT
    Cdtext *cdt = track_get_cdtext(t);
    if (!cdt)
        return m_artist;

    // Get and return the artist
    boost::optional<std::wstring> artist = cdtext_string(cdt, PTI_PERFORMER);
    return artist ? artist : m_artist;
}

boost::optional<std::wstring> CueReader::title_for_track(int track)
{
    // Get the track
    Track *t = cd_get_track(m_cd, track);
    if (!t)
        return boost::optional<std::wstring>();

    // Get the CD-TEXT
    Cdtext *cdt = track_get_cdtext(t);
    if (!cdt)
        return boost::optional<std::wstring>();

    // Get and return the title
    return cdtext_string(cdt, PTI_TITLE);
}
