/*
  ==============================================================================

    VolumeMeter.h
    Created: 30 Aug 2023 9:29:00pm
    Author:  Wai-San

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <vector>

//==============================================================================
/* Component class that displays the current volume on a DJAudioPlayer and animates it in a volume bar */
class VolumeMeter  : public juce::Component
{
public:
    /* Constructor takes the colour that the animated bar will be in */
    VolumeMeter(juce::Colour _colour);
    
    /* Paints the volume meter */
    void paint (juce::Graphics&) override;

    /* takes an vector<float> argument where the 0 postion has the current volume on the left channel, and the 1 position has the current
    volume on the right channel, and sets the visuals to match these levels */
    void setChannels(std::vector<float>& channels);

private:
    // Number of blocks in the volume meter
    int numberOfBlocks{ 50 };

    // Stores the volume levels in each channel
    float leftChannel{ 0 };
    float rightChannel{ 0 };

    // Initilized in the constructor, stores the colour of the volume meter
    juce::Colour colour{};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VolumeMeter)
};
