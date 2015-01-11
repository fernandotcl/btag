/*
 * This file is part of btag.
 *
 * © 2015 Fernando Tarlá Cardoso Lemos
 *
 * Refer to the LICENSE file for licensing information.
 *
 */

#include <boost/foreach.hpp>

#include "CueReaderMultiplexer.h"

CueReaderMultiplexer::CueReaderMultiplexer(const std::list<std::string> &filenames,
                                           const std::string &encoding)
{
    BOOST_FOREACH(const std::string &filename, filenames)
        m_cue_readers.push_back(boost::shared_ptr<CueReader>(new CueReader(filename, encoding)));
}

boost::shared_ptr<CueReader> CueReaderMultiplexer::cue_reader_for_track(int track, int &offset)
{
    int accumulated_tracks = 0;
    BOOST_FOREACH(boost::shared_ptr<CueReader> reader, m_cue_readers) {
        int number_of_tracks = reader->number_of_tracks();
        accumulated_tracks += number_of_tracks;
        if (accumulated_tracks >= track) {
            offset = number_of_tracks - accumulated_tracks;
            return reader;
        }
    }

    return boost::shared_ptr<CueReader>();
}

boost::optional<std::wstring> CueReaderMultiplexer::artist_for_track(int track)
{
    int offset;
    boost::shared_ptr<CueReader> reader = cue_reader_for_track(track, offset);
    return reader ? reader->artist_for_track(track + offset) : boost::none;
}

boost::optional<std::wstring> CueReaderMultiplexer::title_for_track(int track)
{
    int offset;
    boost::shared_ptr<CueReader> reader = cue_reader_for_track(track, offset);
    return reader ? reader->title_for_track(track + offset) : boost::none;
}

boost::optional<std::wstring> CueReaderMultiplexer::album()
{
    return m_cue_readers.empty() ? boost::none : m_cue_readers.front()->album();
}

boost::optional<int> CueReaderMultiplexer::year()
{
    return m_cue_readers.empty() ? boost::none : m_cue_readers.front()->year();
}
