/*
 * This file is part of btag.
 *
 * © 2012, 2015 Fernando Tarlá Cardoso Lemos
 *
 * Refer to the LICENSE file for licensing information.
 *
 */

#include <boost/lexical_cast.hpp>
#include <cstdio>
#include <stdexcept>
#include <vector>

#include "CueReader.h"
#include "validators.h"

CueReader::CueReader(const std::string &filename, const std::string &encoding)
    : m_cd(NULL), m_iconv((iconv_t)-1)
{
    // Get a file pointer
    FILE *fp = fopen(filename.c_str(), "r");
    if (!fp)
        throw std::runtime_error("Unable to open cue sheet for reading");

    // Parse it
    m_cd = cue_parse_file(fp);
    fclose(fp);
    if (!m_cd)
        throw std::runtime_error("Unable to parse cue sheet");

    // Create an iconv descriptor
    m_iconv = iconv_open("WCHAR_T//TRANSLIT", encoding.c_str());
    if (m_iconv == (iconv_t)-1) {
        cd_delete(m_cd);
        throw std::runtime_error("Unable to create iconv descriptor");
    }

    // Fetch the performing artist
    Cdtext *cdt = cd_get_cdtext(m_cd);
    if (cdt)
        m_artist = cdtext_string(cdt, PTI_PERFORMER);
}

CueReader::~CueReader()
{
    if (m_cd)
        cd_delete(m_cd);
    if (m_iconv != (iconv_t)-1)
        iconv_close(m_iconv);
}

int CueReader::number_of_tracks()
{
    return cd_get_ntrack(m_cd);
}

boost::optional<std::wstring> CueReader::cdtext_string(Cdtext *cdt, Pti pti)
{
    // Reset the iconv descriptor
    iconv(m_iconv, NULL, NULL, NULL, NULL);

    // Read the string
    const char *cdtext_str = cdtext_get(pti, cdt);
    if (!cdtext_str)
        return boost::optional<std::wstring>();
    const size_t cdtext_len = strlen(cdtext_str);

    // Copy into an input buffer
    std::vector<char> inbuf(cdtext_len + 1);
    memcpy(&inbuf[0], cdtext_str, cdtext_len + 1);

    // Create the output buffer
    size_t inbytesleft = inbuf.size() * sizeof(char);
    size_t outbytesleft = inbytesleft * sizeof(wchar_t);
    size_t buffer_size = outbytesleft;
    wchar_t outbuf_data[buffer_size / sizeof(wchar_t)];
    char *outbuf = reinterpret_cast<char *>(outbuf_data);

    // Do the conversion
    char *inbuf_ptr = &inbuf[0];
    size_t rc = iconv(m_iconv, &inbuf_ptr, &inbytesleft, &outbuf, &outbytesleft);
    if (rc == (size_t)-1)
        return boost::optional<std::wstring>();

    // Create and return a wide string
    return std::wstring(outbuf_data, (buffer_size - outbytesleft) / sizeof(wchar_t));
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
