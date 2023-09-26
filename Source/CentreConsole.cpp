/*
  ==============================================================================

    CentreConsole.cpp
    Created: 29 Aug 2023 11:24:19am
    Author:  Wai-San

  ==============================================================================
*/

#include <JuceHeader.h>
#include "CentreConsole.h"

//==============================================================================
CentreConsole::CentreConsole(DJAudioPlayer* _leftDjAudioPlayer, DJAudioPlayer* _rightDjAudioPlayer, juce::Colour& _leftDeckColour, juce::Colour& _rightDeckColour, juce::Colour& _backgroundColour)
    : leftPlayer{_leftDjAudioPlayer}
    , rightPlayer{_rightDjAudioPlayer}
    , leftDeckColour{_leftDeckColour}
    , rightDeckColour{_rightDeckColour}
    , backgroundColour{_backgroundColour}
{
    // Setup left deck components
    setupEQSlider(leftDeckHigh, leftEQLookAndFeel);
    setupEQSlider(leftDeckMid, leftEQLookAndFeel);
    setupEQSlider(leftDeckLow, leftEQLookAndFeel);

    setupEQLabel("HIGH", leftDeckHighLabel);
    setupEQLabel("MID", leftDeckMidLabel);
    setupEQLabel("LOW", leftDeckLowLabel);

    setupGainSlider(leftDeckGain, leftVolLookAndFeel);

    addAndMakeVisible(leftDeckVolumeMeter);

    // Setup right deck components
    setupEQSlider(rightDeckHigh, rightEQLookAndFeel);
    setupEQSlider(rightDeckMid, rightEQLookAndFeel);
    setupEQSlider(rightDeckLow, rightEQLookAndFeel);

    setupEQLabel("HIGH", rightDeckHighLabel);
    setupEQLabel("MID", rightDeckMidLabel);
    setupEQLabel("LOW", rightDeckLowLabel);

    setupGainSlider(rightDeckGain, rightVolLookAndFeel);

    addAndMakeVisible(rightDeckVolumeMeter);
    
    // Setup volume mixer
    volumeMixer.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    volumeMixer.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    volumeMixer.setRange(0, 2);
    volumeMixer.setValue(1);
    volumeMixer.addListener(this);
    volumeMixer.setLookAndFeel(&mixerLookAndFeel);
    volumeMixer.setDoubleClickReturnValue(true, 1);
    addAndMakeVisible(volumeMixer);

    startTimer(timerInterval);
}

CentreConsole::~CentreConsole()
{
    // removes all pointers to look and feel components
    leftDeckHigh.setLookAndFeel(nullptr);
    leftDeckMid.setLookAndFeel(nullptr);
    leftDeckLow.setLookAndFeel(nullptr);

    leftDeckHighLabel.setLookAndFeel(nullptr);
    leftDeckMidLabel.setLookAndFeel(nullptr);
    leftDeckLowLabel.setLookAndFeel(nullptr);

    leftDeckGain.setLookAndFeel(nullptr);

    rightDeckHigh.setLookAndFeel(nullptr);
    rightDeckMid.setLookAndFeel(nullptr);
    rightDeckLow.setLookAndFeel(nullptr);

    rightDeckHighLabel.setLookAndFeel(nullptr);
    rightDeckMidLabel.setLookAndFeel(nullptr);
    rightDeckLowLabel.setLookAndFeel(nullptr);

    rightDeckGain.setLookAndFeel(nullptr);

    volumeMixer.setLookAndFeel(nullptr);
}

void CentreConsole::paint (juce::Graphics& g)
{
    // fills background
    g.fillAll(backgroundColour);
}

void CentreConsole::resized()
{
    // setup grid sizes
    int column = static_cast<int>(getWidth() / 8);
    int row = static_cast<int>(getHeight() / 15);

    // size and position of left deck components
    leftDeckHigh.setBounds(column, row, column, row * 2);
    leftDeckHighLabel.setBounds(column, row * 3, column, row);

    leftDeckMid.setBounds(column, row * 4, column, row * 2);
    leftDeckMidLabel.setBounds(column, row * 6, column, row);

    leftDeckLow.setBounds(column, row * 7, column, row * 2);
    leftDeckLowLabel.setBounds(column, row * 9, column, row);

    leftDeckGain.setBounds(static_cast<int>(column * 2.25), row, column, row * 10);

    leftDeckVolumeMeter.setBounds(static_cast<int>(column * 3.4), row, column / 2, row * 10);

    // size and position of right deck components
    rightDeckHigh.setBounds(column * 6, row, column, row * 2);
    rightDeckHighLabel.setBounds(column * 6, row * 3, column, row);

    rightDeckMid.setBounds(column * 6, row * 4, column, row * 2);
    rightDeckMidLabel.setBounds(column * 6, row * 6, column, row);

    rightDeckLow.setBounds(column * 6, row * 7, column, row * 2);
    rightDeckLowLabel.setBounds(column * 6, row * 9, column, row);

    rightDeckGain.setBounds(static_cast<int>(column * 4.75), row, column, row * 10);

    rightDeckVolumeMeter.setBounds(static_cast<int>(column * 4.1), row, column / 2, row * 10);

    volumeMixer.setBounds(column, static_cast<int>(row * 12.5), column * 6, row * 2);
}


// impliment slider::listener
void CentreConsole::sliderValueChanged(juce::Slider* slider)
{
    // left deck EQ sliders
    if (slider == &leftDeckHigh)
    {
        leftPlayer->setHigh(leftDeckHigh.getValue());
    }
    if (slider == &leftDeckMid)
    {
        leftPlayer->setMid(leftDeckMid.getValue());
    }
    if (slider == &leftDeckLow)
    {
        leftPlayer->setLow(leftDeckLow.getValue());
    }

    // left deck gain slider
    if (slider == &leftDeckGain)
    {
        leftDeckVolume = std::min(2 - volumeMixer.getValue(), 1.0) * leftDeckGain.getValue();
        leftPlayer->setGain(leftDeckVolume);
    }

    // Right deck EQ slider
    if (slider == &rightDeckHigh)
    {
        rightPlayer->setHigh(rightDeckHigh.getValue());
    }
    if (slider == &rightDeckMid)
    {
        rightPlayer->setMid(rightDeckMid.getValue());
    }
    if (slider == &rightDeckLow)
    {
        rightPlayer->setLow(rightDeckLow.getValue());
    }

    //right deck gain slider
    if (slider == &rightDeckGain)
    {
        rightDeckVolume = std::min(volumeMixer.getValue(), 1.0) * rightDeckGain.getValue();
        rightPlayer->setGain(rightDeckVolume);
    }

    //volume mixer slider
    if (slider == &volumeMixer)
    {
        if (volumeMixer.getValue() > 1) {
            leftDeckVolume = (2 - volumeMixer.getValue()) * leftDeckGain.getValue();
            leftPlayer->setGain(leftDeckVolume);
        }
        if (volumeMixer.getValue() < 1) {
            rightDeckVolume = volumeMixer.getValue() * rightDeckGain.getValue();
            rightPlayer->setGain(rightDeckVolume);
        }
    }
}

void CentreConsole::setupEQSlider(juce::Slider& EQSlider, EQSliderLookAndFeel& EQLookAndFeel)
{
    EQSlider.addListener(this);
    EQSlider.setRange(0.001, 2);
    EQSlider.setValue(1);
    EQSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    EQSlider.setDoubleClickReturnValue(true, 1);
    EQSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    EQSlider.setLookAndFeel(&EQLookAndFeel);
    addAndMakeVisible(EQSlider);
}

void CentreConsole::setupEQLabel(juce::String name, juce::Label& EQLabel)
{
    EQLabel.setText(name, juce::dontSendNotification);
    EQLabel.setJustificationType(juce::Justification::centredTop);
    addAndMakeVisible(EQLabel);
}

void CentreConsole::setupGainSlider(juce::Slider& gainSlider, VolumeSliderLookAndFeel& gainLookAndFeel)
{
    gainSlider.addListener(this);
    gainSlider.setLookAndFeel(&gainLookAndFeel);
    gainSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    gainSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    gainSlider.setRange(0, 1.5);
    gainSlider.setValue(1);
    gainSlider.setDoubleClickReturnValue(true, 1);
    addAndMakeVisible(gainSlider);
}

void CentreConsole::timerCallback()
{
    // updates the volume meter components
    std::vector<float> leftSoundLevels{ leftPlayer->getSoundLevel() };
    leftDeckVolumeMeter.setChannels( leftSoundLevels );
    leftDeckVolumeMeter.repaint();
    std::vector<float> rightSoundLevels{ rightPlayer->getSoundLevel() };
    rightDeckVolumeMeter.setChannels( rightSoundLevels );
    rightDeckVolumeMeter.repaint();
}