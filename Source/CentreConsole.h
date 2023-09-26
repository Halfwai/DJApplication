/*
  ==============================================================================

    CentreConsole.h
    Created: 29 Aug 2023 11:24:19am
    Author:  Wai-San

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "LookAndFeels.h"
#include <vector>
#include "VolumeMeter.h"
#include "DJAudioPlayer.h"

//==============================================================================
/*
*/
class CentreConsole  : public juce::Component,
    public juce::Slider::Listener,
    public juce::Timer
{
public:
    /* Constructor function takes the pointer address of the left and right DJ audio player, the colours for the left and right decks, 
    and the bakcground colour of the application. These are all set up in MainComponent.h */
    CentreConsole(DJAudioPlayer* _leftDjAudioPlayer, DJAudioPlayer* _rightDjAudioPlayer, juce::Colour& _leftDeckColour, juce::Colour& _rightDeckColour, juce::Colour& backgroundColour);
    
    /* Destructor function clears memory */
    ~CentreConsole() override;

    /* Paint function. For this component it just colours the background colour */
    void paint (juce::Graphics&) override;

    /* Method called every time the window is resized. Updates component size and locations to fit */
    void resized() override;

    // impliment slider::listener. Runs slider functionality
    void sliderValueChanged(juce::Slider* slider) override;   

    
private:
    /* Colour component of the center console. Initilized in the constructor function*/
    juce::Colour leftDeckColour{};
    juce::Colour rightDeckColour{};
    juce::Colour backgroundColour{};

    // left deck player, dials, sliders, labels and look and feel components
    DJAudioPlayer* leftPlayer;
    juce::Slider leftDeckHigh{};
    juce::Label leftDeckHighLabel{};
    juce::Slider leftDeckMid{};
    juce::Label leftDeckMidLabel{};    
    juce::Slider leftDeckLow{};
    juce::Label leftDeckLowLabel{}; 
    juce::Slider leftDeckGain{};
    VolumeMeter leftDeckVolumeMeter{ leftDeckColour };
    EQSliderLookAndFeel leftEQLookAndFeel{ leftDeckColour };
    VolumeSliderLookAndFeel leftVolLookAndFeel{ leftDeckColour };

    // right deck player, dials, sliders, labels and look and feel components
    DJAudioPlayer* rightPlayer;
    juce::Slider rightDeckHigh{};
    juce::Label rightDeckHighLabel{};
    juce::Slider rightDeckMid{};
    juce::Label rightDeckMidLabel{};
    juce::Slider rightDeckLow{};
    juce::Label rightDeckLowLabel{};
    juce::Slider rightDeckGain{};
    VolumeMeter rightDeckVolumeMeter{ rightDeckColour };
    EQSliderLookAndFeel rightEQLookAndFeel{ rightDeckColour };
    VolumeSliderLookAndFeel rightVolLookAndFeel{ rightDeckColour };

    // mixer components
    juce::Slider volumeMixer{};
    double leftDeckVolume{ 1 };
    double rightDeckVolume{ 1 };
    VolumeMixerLookAndFeel mixerLookAndFeel{ leftDeckColour, rightDeckColour };

    /* Function to set up basic parameters of the EQ sliders. Takes the slider, and a look and feel as an argument, and then 
    applies the look and feel, and sets up some other functional paramaters including adding the slider listener and adding and making visable*/
    void setupEQSlider(juce::Slider& EQSlider, EQSliderLookAndFeel& lookAndFeel);

    /* Function to set up basic parameters of the EQ Label. Takes the label, and the text that should be applied, as well as adding and making it visable */
    void setupEQLabel(juce::String name, juce::Label& EQLAbel);

    /* Function to set up basic parameters of the gain slider. Takes the slider, and a look and feel as an argument, and then
    applies the look and feel, and sets up some other functional paramaters including adding the slider listener and adding and making visable*/
    void setupGainSlider(juce::Slider& gainSlider, VolumeSliderLookAndFeel& lookAndFeel);

    /* Callback to the timer, runs some funtionality that requires constant updating. The interval can be changed using the timerInterval variable */
    void timerCallback() override;
    int timerInterval{ 5 };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CentreConsole)
};
