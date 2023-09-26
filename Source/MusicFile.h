/*
  ==============================================================================

    MusicFile.h
    Created: 24 Aug 2023 1:15:55pm
    Author:  Wai-San

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <string>

class MusicFile
{
public:
    /* Constuctor, take an id number and five strings that represent different data about the file.*/
    MusicFile(int _id, std::string _filePath, std::string _filename, std::string _title, std::string _artist, std::string _album);

    /* Returns the id number */
    int getID();

    /* Returns the path */
    std::string getPath();

    /* Returns the filename */
    std::string getFileName();

    /* Returns the song title */
    std::string getTitle();

    /* Returns the song artist */
    std::string getArtist();

    /* Returns the album name*/
    std::string getAlbum();

    /* Returns the highest ID number, used for setting new id numbers*/
    static int getHighID(std::vector<MusicFile>& musicFiles);

    /* Sort by title assending */
    static bool titleCompareAssend(MusicFile a, MusicFile b);
    /* Sort by title descending */
    static bool titleCompareDecend(MusicFile a, MusicFile b);
    /* Sort by filename assending */
    static bool fileNameCompareAssend(MusicFile a, MusicFile b);
    /* Sort by filename descending */
    static bool fileNameCompareDecend(MusicFile a, MusicFile b);
    /* Sort by artist assending */
    static bool artistCompareAssend(MusicFile a, MusicFile b);
    /* Sort by artist descending */
    static bool artistCompareDecend(MusicFile a, MusicFile b);
    /* Sort by album assending */
    static bool albumCompareAssend(MusicFile a, MusicFile b);
    /* Sort by album descending */
    static bool albumCompareDecend(MusicFile a, MusicFile b);

private:
    // MusicFile attributes
    int id{};
    std::string filePath;
    std::string fileName;
    std::string title;
    std::string artist{};
    std::string album{};
};
