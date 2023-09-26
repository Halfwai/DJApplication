/*
  ==============================================================================

    MusicFile.cpp
    Created: 24 Aug 2023 1:15:55pm
    Author:  Wai-San

  ==============================================================================
*/

#include <JuceHeader.h>
#include "MusicFile.h"

//==============================================================================
MusicFile::MusicFile(int _id, std::string _filePath, std::string _filename, std::string _title, std::string _artist, std::string _album)
    : id{_id}
    , filePath{_filePath}
    , fileName{ _filename }
    , title{_title}
    , artist{_artist}
    , album{_album}     
{

}

int MusicFile::getID()
{
    return id;
}

std::string MusicFile::getPath()
{
    return filePath;
}

std::string MusicFile::getFileName()
{
    return fileName;
}

std::string MusicFile::getTitle()
{
    return title;
}

std::string MusicFile::getArtist()
{
    return artist;
}

std::string MusicFile::getAlbum()
{
    return album;
}

int MusicFile::getHighID(std::vector<MusicFile>& musicFiles)
{
    int id = 0;
    for (MusicFile& m : musicFiles)
    {
        if (m.getID() > id)
            id = m.getID();
    }
    return id;
}

bool MusicFile::fileNameCompareAssend(MusicFile a, MusicFile b)
{
    return a.getFileName() < b.getFileName();
}

bool MusicFile::fileNameCompareDecend(MusicFile a, MusicFile b)
{
    return a.getFileName() > b.getFileName();
}

bool MusicFile::titleCompareAssend(MusicFile a, MusicFile b)
{
    return a.getTitle() < b.getTitle();
}

bool MusicFile::titleCompareDecend(MusicFile a, MusicFile b)
{
    return a.getTitle() > b.getTitle();
}

bool MusicFile::artistCompareAssend(MusicFile a, MusicFile b)
{
    return a.getArtist() < b.getArtist();
}

bool MusicFile::artistCompareDecend(MusicFile a, MusicFile b)
{
    return a.getArtist() > b.getArtist();
}

bool MusicFile::albumCompareAssend(MusicFile a, MusicFile b)
{
    return a.getAlbum() < b.getAlbum();
}

bool MusicFile::albumCompareDecend(MusicFile a, MusicFile b)
{
    return a.getAlbum() > b.getAlbum();
}

