/*
  ==============================================================================

    WaveformDisplay.cpp
    Created: 23 Aug 2023 12:51:10pm
    Author:  Wai-San

  ==============================================================================
*/

#include <JuceHeader.h>
#include "WaveformDisplay.h"


WaveformDisplay::WaveformDisplay(juce::AudioFormatManager& formatManagerToUse, juce::AudioThumbnailCache& cacheToUse, juce::Colour _colour)
    : audioThumb{1000, formatManagerToUse, cacheToUse}
    , colour{_colour}
{
    // listens for changes in the audioThumb
    audioThumb.addChangeListener(this);
}

void WaveformDisplay::paint (juce::Graphics& g)
{
    // Paints the audiothumb
    g.fillAll (juce::Colour(50, 50, 50));   // clear the background
    g.setColour(colour);
    if (fileLoaded)
    {         
        audioThumb.drawChannel(g, g.getClipBounds(), 0, audioThumb.getTotalLength(), 0, 1.0f);
        // This paints a dark transparant rectangle over the audioThumb to show how much of the track has been played
        g.setColour(juce::Colour(0.0f, 0.0f, 0.0f, 0.5f));
        g.fillRect(0, 0, static_cast<int>(relativePosition * getWidth()), getHeight());
        return;
    }

    // If a file hasn't been loaded, shows a message for the user
    g.setFont (20.0f);
    g.drawText ("File not yet loaded...", getLocalBounds(),
                juce::Justification::centred, true); 
}

void WaveformDisplay::loadURL(juce::URL audioURL)
{
    // loads a new file into the sudioThumb
    audioThumb.clear();
    fileLoaded = audioThumb.setSource(new juce::URLInputSource(audioURL));
}

void WaveformDisplay::changeListenerCallback(juce::ChangeBroadcaster*)
{
    // When the audioThumb changes, repaints it
    repaint();
}

void WaveformDisplay::setPosition(double pos)
{
    // sets the relativePosition variable to colour the shaded area
    double newRelativePosition{ pos / audioThumb.getTotalLength() };
    if (relativePosition != newRelativePosition)
    {
        relativePosition = newRelativePosition;
        repaint();
    }
}
