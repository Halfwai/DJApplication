/*
  ==============================================================================

    DeckGUI.h
    Created: 22 Aug 2023 1:48:01pm
    Author:  Wai-San

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"
#include "LookAndFeels.h"
#include "MusicFile.h"


// Cuepoint class holds two buttons and a double, and has one getter function. It's only used by the DeckGUI so I stored it here
class CuePoint
{
public:
    /* constructor function stores a double value for the position of the cuepoint */
    CuePoint(double _position = 0) : cuePosition{_position}
    {

    }

    // Each cuepoint has two buttons, a button to go to the cuepoint, and a butotn to remove it
    juce::TextButton goToCue{};
    juce::TextButton removeCue{ "X" };

    /* returns the cuepoint position as a double */
    double getPosition() {
        return cuePosition;
    }

private:
    // stores the cuepoint position
    double cuePosition{ };
};




//==============================================================================
/*
*/
class DeckGUI : public juce::Component,
                public juce::Button::Listener,
                public juce::Slider::Listener,
                public juce::Timer
{
public:
    /* Constructor function. Takes the DJAudioPlayer, the app AudioFormatManager and AudioFormatCache, the colour for the deck, the app background colour,
    and an image of a record */
    DeckGUI(DJAudioPlayer* _djAudioPlayer
        , juce::AudioFormatManager& _formatManagerToUse
        , juce::AudioThumbnailCache& _cacheToUse
        , juce::Colour& _colour
        , juce::Colour& _backgroundColour
        , const juce::Image& _recordImage);

    /* Destructor function. Removes look and feel pointers, and stops the timer */
    ~DeckGUI() override;

    /* sets the backgound colour. This can be changed when passing a different background color into the constructor */
    void paint (juce::Graphics&) override;
    
    /* Called when component resized, updates position and size of child components */
    void resized() override;

    /* impliment button::listener */ 
    void buttonClicked(juce::Button* button) override;

    /* impliment slider::listener */ 
    void sliderValueChanged(juce::Slider* slider) override;   

    /* Takes a MusicFile as an argument. Sends data to the DJAudioPlayer to be processed into a playable file, and updates the GUI to show
    track informaiton */
    void addURL(MusicFile musicFile);

private:
    // loads images for the control buttons
    const juce::Image playButtonImage = juce::ImageCache::getFromMemory(BinaryData::playbutton_png, BinaryData::playbutton_pngSize);
    const juce::Image pauseButtonImage = juce::ImageCache::getFromMemory(BinaryData::pausebutton_png, BinaryData::pausebutton_pngSize);
    const juce::Image stopButtonImage = juce::ImageCache::getFromMemory(BinaryData::stopbutton_png, BinaryData::stopbutton_pngSize);

    // stores colour values, initilised during the constructor function
    juce::Colour deckColour{};
    juce::Colour backgroundColour{};

    // setup look and feel components 
    EQSliderLookAndFeel EQKnobs{ deckColour };
    SpeedSliderLookAndFeel speedSliderLookAndFeel{ deckColour };
    ControlButtonLookAndFeel playButtonLookAndFeel{ playButtonImage,deckColour };
    ControlButtonLookAndFeel pauseButtonLookAndFeel{ pauseButtonImage, deckColour };
    ControlButtonLookAndFeel stopButtonLookAndFeel{ stopButtonImage, deckColour };
    PositionSliderLookAndFeel positionSliderLookAndFeel{};


    // control buttons
    juce::TextButton playButton{};
    juce::TextButton stopButton{};
    juce::TextButton setCueButton{"Set Cue"};

    // vector used to store cuePoints. Allows new cuepoints to be created and deleted by the user on the fly
    std::vector<std::unique_ptr<CuePoint>> cuePoints{};

    // Image of the spinning record in the middle of the deckGui
    const juce::Image recordImage{};
    juce::ImageComponent record{};

    // control dials
    juce::Slider speedSlider{};
    juce::Slider positionSlider{};    

    // labels for the song data section
    juce::Label fileNameLabel{};
    juce::Label artistLabel{};
    juce::Label songTitleLabel{};
    juce::Label songLength{};
    juce::Label songPosition{};

    // pointer to the DJAudioPlayer
    DJAudioPlayer* player;

    // waveform display component
    WaveformDisplay waveformDisplay;

    // stores the value of the rotation for the record
    float recordRotation{ 0.0f };

    /* function takes the return double values from the DJAudioPayer for trackLength and get position functions, and turns them into
    a string of minutes and seconds */
    static std::string DeckGUI::secondsDoubleToString(double trackLength);

    /* Sets up the values for the song details labels */
    void setupSongDetails(MusicFile musicFile, double trackLength);

    /* Timer callback updates the songposition label, and spins the record image component if the song is playing. Timer intival can be set using the
    related variable */
    void timerCallback() override;
    int timerInterval{ 5 };

    /* creates a CuePoint and adds its address to the cuepoints vector */
    void createCuepoint();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};


