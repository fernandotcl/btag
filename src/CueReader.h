/*
 * This file is part of btag.
 *
 * © 2012 Fernando Tarlá Cardoso Lemos
 *
 * Refer to the LICENSE file for licensing information.
 *
 */

#ifndef CUE_READER_H
#define CUE_READER_H

#include <boost/optional.hpp>
#include <string>

extern "C" {
#include <libcue/libcue.h>
}

class CueReader
{
    public:
        CueReader(const std::string &filename);
        ~CueReader();

        boost::optional<std::wstring> artist_for_track(int track);
        boost::optional<std::wstring> title_for_track(int track);

        boost::optional<std::wstring> album();
        boost::optional<int> year();

    private:
        boost::optional<std::wstring> cdtext_string(Cdtext *cdt, Pti pti);

        Cd *m_cd;
        boost::optional<std::wstring> m_artist;
};

#endif
