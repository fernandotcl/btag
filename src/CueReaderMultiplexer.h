/*
 * This file is part of btag.
 *
 * © 2015 Fernando Tarlá Cardoso Lemos
 *
 * Refer to the LICENSE file for licensing information.
 *
 */

#ifndef CUE_READER_MULTIPLEXER_H
#define CUE_READER_MULTIPLEXER_H

#include <boost/optional.hpp>
#include <boost/shared_ptr.hpp>
#include <list>
#include <string>

#include "CueReader.h"

class CueReaderMultiplexer
{
    public:
        CueReaderMultiplexer(const std::list<std::string> &filenames, const std::string &encoding);

        boost::optional<std::wstring> artist_for_track(int track);
        boost::optional<std::wstring> title_for_track(int track);

        boost::optional<std::wstring> album();
        boost::optional<int> year();

    private:
        std::list<boost::shared_ptr<CueReader> > m_cue_readers;

        boost::shared_ptr<CueReader> cue_reader_for_track(int track, int &offset);
};

#endif
