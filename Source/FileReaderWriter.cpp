/*
  ==============================================================================

    FileReaderWriterWriter.cpp
    Created: 2 Sep 2023 1:27:10pm
    Author:  Wai-San

  ==============================================================================
*/

#include "FileReaderWriter.h"
#include <iostream>
#include <fstream>
#include <vector>

FileReaderWriter::FileReaderWriter()
{

}

/* Reads data from a File folder and returns it as an orderbook */
std::vector<MusicFile> FileReaderWriter::readFile(std::string filename)
{
    std::vector<MusicFile> musicFiles{};
    std::ifstream file{ filename };
    std::string line{};

    if (file.is_open())
    {
        std::cout << "file Open" << std::endl;

        while (std::getline(file, line))
        {
            try
            {
                // "<[]>" is the string that I am using to break up lines in my files. The nature of dealing with song filenames means 
                // that using quotation marks, or other regular characters could cause conflicts. I feel that the string "<[]>" is a lot more uncommon 
                MusicFile musicFile = stringsToMusicFile(tokenise(line, "<[]>"));
                musicFiles.push_back(musicFile);
            }
            catch (const std::exception& e)
            {
                std::cout << e.what() << std::endl;
                continue;
            }
        }
        file.close();
    }

    return musicFiles;
}

/* breaks a string into pieces based on a seperating character, then returns a vector of those pieces */
std::vector<std::string> FileReaderWriter::tokenise(std::string fileLine, std::string separator)
{
    std::vector<std::string> tokens{};

    size_t start = 0;
    size_t end{ fileLine.find(separator) };
    do
    {
        if (start == fileLine.size() || start == end)
        {
            break;
        }
        std::string token{ fileLine.begin() + start, fileLine.begin() + end };
        fileLine.erase(start, end + separator.size());
        end = fileLine.find(separator);
        tokens.push_back(token);
        if (end > fileLine.size())
        {
            tokens.push_back(fileLine);
        }
    } while (end != std::string::npos);

    return tokens;
}

/* converts a vector made of a tokenized string into an MusicFile */
MusicFile FileReaderWriter::stringsToMusicFile(std::vector<std::string> tokens)
{
    if (tokens.size() == 6)
    {
        int id{ std::stoi(tokens[0]) };
        std::string path{ tokens[1] };
        std::string fileName{ tokens[2] };
        std::string title{ tokens[3] };
        std::string artist{ tokens[4] };
        std::string album{ tokens[5] };
        MusicFile musicfile{ id, path, fileName, title, artist, album };
        return musicfile;
    }
    DBG("Bad line in file");
    throw std::exception{};
}

// append method adapted from here - https://mathbits.com/MathBits/CompSci/Files/Append.htm
void FileReaderWriter::writeMusicFileToFile(std::string filename, MusicFile musicFile)
{
    std::string id{ std::to_string(musicFile.getID()) + "<[]>" };
    std::string path{ musicFile.getPath() + "<[]>" };
    std::string fileName{ musicFile.getFileName() + "<[]>" };
    std::string title{ musicFile.getTitle() + "<[]>" };
    std::string artist{ musicFile.getArtist() + "<[]>" };
    std::string album{ musicFile.getAlbum() };

    std::string FileFileRow{ id + path + fileName + title + artist + album };

    std::ofstream file{ filename, std::ios::app };
    file << FileFileRow << std::endl;
    file.close();  

}

void FileReaderWriter::removeMusicFileFromFile(std::string filename, MusicFile& musicFile)
{
    std::ifstream playlistFile{ filename };
    std::ofstream temp("temp.txt");
    std::string line{};

    if (playlistFile.is_open())
    {

        while (std::getline(playlistFile, line))
        {
            if (musicFile.getID() != std::stoi(tokenise(line, "<[]>")[0]))
            {
                temp << line << std::endl;
            }
        }
        playlistFile.close();
        temp.close();
        remove(filename.c_str());
        if (rename("temp.txt", filename.c_str()))
        {
            DBG("File removed successfully");
        }
    }
}


