/*
  ==============================================================================

    WaveformDisplay.h
    Created: 23 Aug 2023 12:51:10pm
    Author:  Wai-San

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>


/* Wavefor display displays the audio thumbnail of the loaded track */
class WaveformDisplay   : public juce::Component
                        , public juce::ChangeListener
{
public:
    /* Constructon takes the app AudioFormatMsnager, and the AudioFormatCache, and the colour of the deck it is on */
    WaveformDisplay(juce::AudioFormatManager& formatManagerToUse, juce::AudioThumbnailCache& cacheToUse, juce::Colour _colour);

    /* repaints the audiothumbnail when a new file is loaded */
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;

    /* Draws the audioThumbnail */
    void paint (juce::Graphics&) override;

    /* Loads the file into the audioThumbnail*/
    void loadURL(juce::URL audioURL);

    /* Sets the current position of the track so that the shaded area of the audioThumbnail can be drawn */
    void setPosition(double pos);

private:
    // Audiothumbnail
    juce::AudioThumbnail audioThumb;

    // Checks whether a file has been loaded
    bool fileLoaded{ false };

    // Holds the relative position of the track
    double relativePosition{};

    // Initilized in the construction function, holds the colour value of the deck
    juce::Colour colour{};
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformDisplay)
};
