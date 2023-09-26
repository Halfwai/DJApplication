/*
  ==============================================================================

    PlaylistComponent.h
    Created: 24 Aug 2023 11:37:28am
    Author:  Wai-San

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <vector>
#include <string>
#include "DJAudioPlayer.h"
#include "DeckGUI.h"




//==============================================================================
/* Playlist Component class displays the playlist GUI and includes all playlist functionality */
class PlaylistComponent  : public juce::Component,
    public juce::TableListBoxModel,
    public juce::Button::Listener,
    public juce::FileDragAndDropTarget,   
    public juce::TextEditor::Listener
{
public:
    /* Constructor takes the left and right gui and it's colour. Sets up the table component to display data on MusicFiles, as well as 
    two buttons to link them to the left and right deck, also adds and makes visiable some general playlist buttons and search bar */
    PlaylistComponent(DeckGUI* _leftConsole, DeckGUI* _rightConsole, juce::Colour _leftColour, juce::Colour _rightColour, juce::Colour _backgroundColour);
    
    /* removes the pointer from the table component */
    ~PlaylistComponent() override;

    /* Empty function as table takes up all component space */
    void paint (juce::Graphics&) override;
    
    /* resizes table and columns */
    void resized() override;

    // TableListBoxModel pure virtual functions
    /* Returns the number of playlist items to be displayed after being searched through by the value in the search bar */
    int getNumRows() override;

    /* Sets colour for rows based on wether they have been selected or not */
    void paintRowBackground(juce::Graphics&, int rowNumber, int width, int height, bool rowIsSelected) override;

    /* Writes the MusicFile values into the table */
    void paintCell(juce::Graphics&, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;

    /* Adds and maintains the buttons to play on each deck */
    juce::Component* refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component* existingComponentToUpdate) override;

    /* Button listener, runs code based on what button has been clicked */
    void buttonClicked(juce::Button* button) override;

    /* Allows files to be dragged and dropped onto the playlist component */
    bool isInterestedInFileDrag(const juce::StringArray& files) override;

    /* Takes the files that have been dropped into the playlistComponent and tries to process them into MusicFile items also adds them to the data.txt file
    so that they can be reloaded when if the app is rebooted */
    void filesDropped(const juce::StringArray& files, int x, int y) override;

    /* Runs remove file function if the delete key is pressed */
    void deleteKeyPressed(int lastRowSelected) override;

    /* Sorts the MusicFiles based on which column sorting button is pressed */
    void sortOrderChanged(int newSortColumnId, bool isForwards) override;

    /* Listener for the search bar text editor. Researches the files each time the user edits their search term */
    void textEditorTextChanged(juce::TextEditor& searchBox) override;

private:
    // Table gui
    juce::TableListBox tableComponent{};

    // Pointers tpo left and right gui
    DeckGUI* leftConsole;
    DeckGUI* rightConsole;

    // vector to store all the music files loaded into the app
    std::vector<MusicFile> musicFiles{};

    // stores the higest ID number of the tracks in musicFiles, so that duplicate IDs are not given to new tracks
    int highID{};

    // file where loaded tracks are stored for reboot
    std::string playlistFilename{ "playlist.txt" };

    // Colours of the left and righ deck, and the background colour for the app. Initilised in the constructor
    juce::Colour leftColour{};
    juce::Colour rightColour{};
    juce::Colour backgroundColour{};

    // Playlist menu buttons
    juce::TextButton loadButton{ "Load music files" };
    juce::TextButton removeFileButton{"Remove selected file"};

    // Search variables. Contains TextEditor for the user to search in, a label, the string that the user is searching for, and a vector
    // containing the index numbers of all matching MusicFiles found in the musicFiles vector
    juce::TextEditor searchBoxInput{};
    juce::Label searchBoxLabel{};
    std::string searchterm{};
    std::vector<int> foundIndex{};

    // FileChooser for the user to load files
    juce::FileChooser fChooser{ "Select a file..." };

    // Contains the row number of the currently selected row
    int selectedRowNumber{ -1 };

    // Contains ID number of the track currently loaded into each deck
    int leftActiveFileID{};
    int rightActiveFileID{};

    /* Removes a MusicFile from the musicFiles vector, and the.txt file containing files to be loaded on opening */ 
    void removeFile(int lastRowSelected, MusicFile musicFile);

    /* Parses a file information and attempts to load it to the playlist */
    void loadFilesToPlaylist(const juce::StringArray& files);

    /* Searches the filename of the MusicFile for substring in the search bar, then adds index number to an array of ints */
    void searchPlaylist();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
