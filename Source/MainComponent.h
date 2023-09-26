#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "DeckGUI.h"
#include "PlaylistComponent.h"
#include "CentreConsole.h"

class MainComponent : public juce::AudioAppComponent
{
public:
    /* Constructor function. Sets up all the gui components, the format manager, some audio components */
    MainComponent();

    /* Clears the audio sources*/
    ~MainComponent() override;

    /* Adds each player to the mixer source */
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;

    /* Gets the next audio block for the mixer source */
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;

    /* releases all the resources for the audio players sources */
    void releaseResources() override;

    /* fills the background in with the background colour */
    void paint(juce::Graphics& g) override;

    /* Resizes the gui components when the window is resized */
    void resized() override;

private:
    //==============================================================================
    // Your private member variables go here...
    juce::AudioFormatManager formatManager{};
    juce::AudioThumbnailCache thumbCache{100};
    
    // loads each image of a record to be used on the left and right decks
    const juce::Image leftRecordImage = juce::ImageCache::getFromMemory(BinaryData::redRecord_png, BinaryData::redRecord_pngSize);
    const juce::Image rightRecordImage = juce::ImageCache::getFromMemory(BinaryData::blueRecord_png, BinaryData::blueRecord_pngSize);
    
    // Sets the colours used throughout the app. Changes here will result in the colours changing in all parts of the app
    juce::Colour leftDeckColour{ juce::Colour(149, 35, 35) };
    juce::Colour rightDeckColour{ juce::Colour(23, 107, 135) };
    juce::Colour backgroundColour{ juce::Colour(30, 30, 30) };

    // Sets up the left DJAudioPlayer and its gui
    DJAudioPlayer leftPlayer{ formatManager };
    DeckGUI deck1{ &leftPlayer, formatManager, thumbCache, leftDeckColour, backgroundColour, leftRecordImage };

    // Sets up the right DJAudioPlayer and its gui
    DJAudioPlayer rightPlayer{ formatManager };
    DeckGUI deck2{ &rightPlayer, formatManager, thumbCache, rightDeckColour, backgroundColour, rightRecordImage };

    // Sets up the centre console
    CentreConsole centreConsole{ &leftPlayer, &rightPlayer, leftDeckColour, rightDeckColour, backgroundColour };

    // The mixer audio source that mixes the left and right decks together
    juce::MixerAudioSource mixerSource{};

    // Sets up the playlist
    PlaylistComponent playlistComponent{ &deck1, &deck2, leftDeckColour, rightDeckColour, backgroundColour };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};