/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 24 Aug 2023 11:37:28am
    Author:  Wai-San

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistComponent.h"
#include "FileReaderWriter.h"
#include <iostream>
#include <fstream>


// mp3_id3_tags header library to extract metadata from mp3 files
#define MP3_ID3_TAGS_USE_GENRES
#define MP3_ID3_TAGS_IMPLEMENTATION
#include "mp3_id3_tags.h"

//==============================================================================
PlaylistComponent::PlaylistComponent(DeckGUI* _leftConsole, DeckGUI* _rightConsole, juce::Colour _leftColour, juce::Colour _rightColour, juce::Colour _backgroundColour)
    : leftConsole{_leftConsole}
    , rightConsole{_rightConsole}
    , leftColour{_leftColour}
    , rightColour{_rightColour}
    , backgroundColour{_backgroundColour}
{
    // loads up MusicFiles to be displayed
    musicFiles = FileReaderWriter::readFile(playlistFilename);
    highID = MusicFile::getHighID(musicFiles);

    // Set up and display gui components
    tableComponent.getHeader().addColumn("File Name", 1, 200);
    tableComponent.getHeader().addColumn("Song Title", 2, 200);
    tableComponent.getHeader().addColumn("Artist", 3, 200);
    tableComponent.getHeader().addColumn("Album", 4, 200);
    tableComponent.getHeader().addColumn("", 5, 100);
    tableComponent.getHeader().addColumn("", 6, 100);

    tableComponent.setModel(this); 
    tableComponent.setColour(0x1002800, backgroundColour);
    
    addAndMakeVisible(tableComponent);

    loadButton.addListener(this);
    addAndMakeVisible(loadButton);
    removeFileButton.addListener(this);
    addAndMakeVisible(removeFileButton);

    addAndMakeVisible(searchBoxInput);
    searchBoxInput.addListener(this);
    
    searchBoxLabel.setText("Search File Names:", juce::dontSendNotification);
    searchBoxLabel.setJustificationType(juce::Justification::centredRight);
    searchBoxLabel.attachToComponent(&searchBoxInput, true);
}

PlaylistComponent::~PlaylistComponent()
{
    tableComponent.setModel(nullptr);
}

void PlaylistComponent::paint (juce::Graphics&)
{

}

void PlaylistComponent::resized()
{
    // Set display grid variables
    int columnWidth{ static_cast<int>((getWidth() - tableComponent.getVerticalScrollBar().getWidth()) / 5.0) };      
    int buttonHeight{ getHeight() / 15 };

    tableComponent.setBounds(0, 0, getWidth(), getHeight() - buttonHeight);
    tableComponent.getHeader().setColumnWidth(1, columnWidth);
    tableComponent.getHeader().setColumnWidth(2, columnWidth);
    tableComponent.getHeader().setColumnWidth(3, columnWidth);
    tableComponent.getHeader().setColumnWidth(4, columnWidth);
    tableComponent.getHeader().setColumnWidth(5, columnWidth / 2);
    tableComponent.getHeader().setColumnWidth(6, columnWidth / 2);
    tableComponent.updateContent();
    tableComponent.repaint();

    removeFileButton.setBounds(0, getHeight() - buttonHeight, columnWidth, buttonHeight);
    loadButton.setBounds(columnWidth, getHeight() - buttonHeight, columnWidth, buttonHeight);
    searchBoxInput.setBounds(columnWidth * 4, getHeight() - buttonHeight, columnWidth, buttonHeight);
}

int PlaylistComponent::getNumRows()
{
    searchPlaylist();
    return static_cast<int>(foundIndex.size());
}

void PlaylistComponent::paintRowBackground(juce::Graphics& g, int rowNumber, int, int, bool rowIsSelected)
{
    // colours row if selected, otherwise uses background colour 
    if (rowIsSelected)
    {
        selectedRowNumber = rowNumber;
        g.fillAll(juce::Colours::orange);
        return;
    }
    g.fillAll(backgroundColour);
}

void PlaylistComponent::paintCell(juce::Graphics& g, int rowNumber, int columnId, int width, int height, bool)
{
    // fills out table with MusicFile information
    g.setColour(juce::Colours::white);
    MusicFile* musicFile{ &musicFiles[foundIndex[rowNumber]] };
    if (columnId == 1)
    {
        std::string path{ musicFile->getFileName() };
        g.drawText(path, 2, 0, width - 4, height, juce::Justification::centredLeft, true);
    }
    if (columnId == 2)
    {
        g.drawText(musicFile->getTitle(), 2, 0, width - 4, height, juce::Justification::centredLeft, true);
    }
    if (columnId == 3)
    {
        g.drawText(musicFile->getArtist(), 2, 0, width - 4, height, juce::Justification::centredLeft, true);
    }
    if (columnId == 4)
    {
        g.drawText(musicFile->getAlbum(), 2, 0, width - 4, height, juce::Justification::centredLeft, true);
    } 
}

juce::Component* PlaylistComponent::refreshComponentForCell(int rowNumber, int columnId, bool, Component* existingComponentToUpdate)
{
    // Adds and maintains buttons
    if (columnId == 5)
    {
        juce::String id{ std::to_string(rowNumber) + ",L" };
        if (existingComponentToUpdate == nullptr)
        {   
            juce::TextButton* btn{ new juce::TextButton{ "Play on left deck" } };
            btn->setColour(0x1000101, leftColour);
            btn->setComponentID(id);
            btn->addListener(this);
            existingComponentToUpdate = btn;
        }
        // polymorphism casting method adapted from the user Marc Claesen here - https://stackoverflow.com/questions/16616629/cant-access-members-of-child-class-in-polymorphism
        juce::TextButton* button = dynamic_cast<juce::TextButton*>(existingComponentToUpdate);
        if (button != NULL)
        {
            button->setComponentID(id);
            if (musicFiles[foundIndex[rowNumber]].getID() != leftActiveFileID)
                button->setToggleState(false, juce::dontSendNotification);
            if (musicFiles[foundIndex[rowNumber]].getID() == leftActiveFileID)
                button->setToggleState(true, juce::dontSendNotification);
        }
    }
    if (columnId == 6)
    {
        juce::String id{ std::to_string(rowNumber) + ",R" };
        if (existingComponentToUpdate == nullptr)
        {
            juce::TextButton* btn{ new juce::TextButton{ "Play on right deck" } };
            btn->setColour(0x1000101, rightColour);
            btn->setComponentID(id);
            btn->addListener(this);
            existingComponentToUpdate = btn;
        }
        juce::TextButton* button = dynamic_cast<juce::TextButton*>(existingComponentToUpdate);
        if (button != NULL)
        {
            button->setComponentID(id);
            if (musicFiles[foundIndex[rowNumber]].getID() != rightActiveFileID)
                button->setToggleState(false, juce::dontSendNotification);
            if (musicFiles[foundIndex[rowNumber]].getID() == rightActiveFileID)
                button->setToggleState(true, juce::dontSendNotification);
        }
    }
    return existingComponentToUpdate;
}

void PlaylistComponent::buttonClicked(juce::Button* button)
{
    // button listiner returns early if the buttons are class components, otherwise processes playlist buttons created in refreshComponentForCell function
    if (button == &removeFileButton)
    {
        if (selectedRowNumber < foundIndex.size())
            removeFile(selectedRowNumber, musicFiles[foundIndex[selectedRowNumber]]);
        return;
    }
    if (button == &loadButton)
    {      
        auto fileChooserFlags =
        juce::FileBrowserComponent::canSelectMultipleItems;
        fChooser.launchAsync(fileChooserFlags, [this](const juce::FileChooser& chooser)
        {
            juce::StringArray fileNames{};
            for (juce::File f : chooser.getResults())
            {
                fileNames.add(f.getFullPathName());
            }            
            loadFilesToPlaylist(fileNames);
        });
        return;
    }
    
    std::string buttonDetails{ button->getComponentID().toStdString() };
    std::vector buttonTokens{ FileReaderWriter::tokenise(buttonDetails, ",") };
    int id{ std::stoi(buttonTokens[0]) };
    button->setToggleState(true, juce::dontSendNotification);
    if (buttonTokens[1] == "L")
    {   
        leftActiveFileID = musicFiles[foundIndex[id]].getID();
        leftConsole->addURL( musicFiles[foundIndex[id]]);
        tableComponent.updateContent();
        return;
    }
    rightActiveFileID = musicFiles[foundIndex[id]].getID();
    rightConsole->addURL( musicFiles[foundIndex[id]] );
    tableComponent.updateContent();
}

bool PlaylistComponent::isInterestedInFileDrag(const juce::StringArray&)
{
    return true;
}

void PlaylistComponent::filesDropped(const juce::StringArray& files, int, int)
{   
    loadFilesToPlaylist(files);   
}

void PlaylistComponent::deleteKeyPressed(int lastRowSelected)
{
    removeFile(lastRowSelected, musicFiles[foundIndex[lastRowSelected]]);
}


void PlaylistComponent::sortOrderChanged(int newSortColumnId, bool isForwards)
{
    if (newSortColumnId == 1)
    {
        if (isForwards)
            sort(musicFiles.begin(), musicFiles.end(), MusicFile::fileNameCompareAssend);
        else
            sort(musicFiles.begin(), musicFiles.end(), MusicFile::fileNameCompareDecend);
    }
    if (newSortColumnId == 2)
    {
        if (isForwards)
            sort(musicFiles.begin(), musicFiles.end(), MusicFile::titleCompareAssend);
        else
            sort(musicFiles.begin(), musicFiles.end(), MusicFile::titleCompareDecend);
    }
    if (newSortColumnId == 3)
    {
        if (isForwards)
            sort(musicFiles.begin(), musicFiles.end(), MusicFile::artistCompareAssend);
        else
            sort(musicFiles.begin(), musicFiles.end(), MusicFile::artistCompareDecend);
    }
    if (newSortColumnId == 4)
    {
        if (isForwards)
            sort(musicFiles.begin(), musicFiles.end(), MusicFile::albumCompareAssend);
        else
            sort(musicFiles.begin(), musicFiles.end(), MusicFile::albumCompareDecend);
    }
}


void PlaylistComponent::removeFile(int lastRowSelected, MusicFile musicFile)
{
    // removes the file and updates relevant variables. Then researches to keep the playlist length relevant
    FileReaderWriter::removeMusicFileFromFile(playlistFilename, musicFile);
    musicFiles.erase(musicFiles.begin() + foundIndex[lastRowSelected]);
    searchPlaylist();
    tableComponent.updateContent();
    tableComponent.repaint();
}

void PlaylistComponent::loadFilesToPlaylist(const juce::StringArray& files)
{
    // Loads the file and processes it's metadata.
    for (const juce::String& file : files)
    {
        std::string FileLine{ file.toStdString() + ',' };
        FILE* f = fopen(file.toStdString().c_str(), "rb");
        // method of filtering files taken from here - https://forum.juce.com/t/test-file-type/15728
        // I set my app to only take mp3s, wav and aac files as these are the only files that I had on my computer to test with.
        // If you want to add other filetypes, you can just add "&& !file.endsWith(".filetype")" to this if statement.
        if (!file.endsWith(".mp3") && !file.endsWith(".wav") && !file.endsWith(".aac"))
            continue;
        std::string title{ "Unknown Title" };
        std::string artist{ "Unknown Artist" };
        std::string album{ "Unknown Album" };
        // uses the mp3_id3_tags library to extract id3 metadata from mp3s
        if (f) {
            mp3_id3_tags tags;
            if (mp3_id3_file_read_tags(f, &tags)) {
                title = tags.title;
                artist = tags.artist;
                album = tags.album;
            }
        }
        highID += 1;
        std::string fileName{ juce::URL::removeEscapeChars(juce::URL{juce::File{file}}.getFileName()).toStdString() };
        MusicFile musicFile{ highID, file.toStdString(), fileName, title, artist, album };
        musicFiles.push_back(musicFile);
        FileReaderWriter::writeMusicFileToFile(playlistFilename, musicFile);
    }
    searchPlaylist();
    tableComponent.updateContent();
    tableComponent.repaint();
}


void PlaylistComponent::textEditorTextChanged(juce::TextEditor& searchBox)
{
    searchterm = searchBox.getTextValue().toString().toStdString();
    tableComponent.updateContent();
    tableComponent.repaint();
}

void PlaylistComponent::searchPlaylist()
{
    foundIndex.clear();
    for (auto i = 0; i < musicFiles.size(); ++i)
    {
        std::size_t included{ musicFiles[i].getFileName().find(searchterm) };
        if (included != std::string::npos || searchterm.size() == 0)
            foundIndex.push_back(i);
    }
}