/*
  ==============================================================================

    FileReaderWriter.h
    Created: 2 Sep 2023 1:27:10pm
    Author:  Wai-San

  ==============================================================================
*/
#pragma once
#include "MusicFile.h"
#include <vector>
#include <string>

// This class has several functions that were converted from the CSV reader used in the Merkel trading app
class FileReaderWriter
{
public:
	/* Class contains only static functions, so constructor function contains no functionality */
	FileReaderWriter();

	/* Member functions that process a File and convert it into an MusicFile */
	static std::vector<MusicFile> readFile(std::string filename);

	/* breaks up a line from a file into seperate strings, based on a seperator string. Does not use a comma value, or any one character value
	as other characters too common in song and band names */
	static std::vector<std::string> tokenise(std::string fileLine, std::string separator);

	/* Takes a music file, and parses it's data and adds it to a file */
	static void writeMusicFileToFile(std::string filename, MusicFile musicFile);

	/* Finds and removes a musicFile from a file */
	static void removeMusicFileFromFile(std::string filename, MusicFile& musicFile);

private:
	/* Creates a MusicFile from a vector with the required parameters.These parameters are an ID number, a path, a filename, a track title, an artist
	and an album name */ 
	static MusicFile stringsToMusicFile(std::vector<std::string> tokens);
};