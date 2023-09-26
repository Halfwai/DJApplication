/*
  ==============================================================================

    VolumeMeter.cpp
    Created: 30 Aug 2023 9:29:00pm
    Author:  Wai-San

  ==============================================================================
*/

#include <JuceHeader.h>
#include "VolumeMeter.h"

//==============================================================================
VolumeMeter::VolumeMeter(juce::Colour _colour) : colour{_colour}
{

}


void VolumeMeter::paint (juce::Graphics& g)
{
    // sets up variables to draw the volume meter
    float gap = static_cast<float>(getHeight() / numberOfBlocks + 1);
    float leftChannelVolume{ leftChannel * getHeight() };
    float rightChannelVolume{ rightChannel * getHeight() };
    //draw the left channel meter
    for (auto i = 0; i < numberOfBlocks; ++i)
    {
        g.setColour(juce::Colours::black);
        if (leftChannelVolume >= i * gap && leftChannel != 0.0)
            g.setColour(colour);
        juce::Rectangle block{ 0.0f, (getHeight() - gap) - (i * gap), getWidth() / 2.0f, gap};
        g.fillRect(block);
    }

    // draw the right had meter
    for (auto i = 0; i < numberOfBlocks; ++i)
    {
        g.setColour(juce::Colours::black);
        if (rightChannelVolume >= i * gap && rightChannel != 0.0)
            g.setColour(colour);
        juce::Rectangle block{ getWidth() / 2.0f, (getHeight() - gap) - (i * gap), getWidth() / 2.0f, gap};
        g.fillRect(block);
    }
}

// sets the volume levels
void VolumeMeter::setChannels(std::vector<float>& channels)
{
    leftChannel = channels[0];
    rightChannel = channels[1];
}