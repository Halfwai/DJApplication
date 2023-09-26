/*
  ==============================================================================

    DeckGUI.cpp
    Created: 22 Aug 2023 1:48:01pm
    Author:  Wai-San

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DeckGUI.h"
#include <sstream>

//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer* _djAudioPlayer
    , juce::AudioFormatManager& _formatManagerToUse
    , juce::AudioThumbnailCache& _cacheToUse
    , juce::Colour& _colour
    , juce::Colour& _backgroundColour
    , const juce::Image& _recordImage)

    : player{_djAudioPlayer}
    , waveformDisplay{_formatManagerToUse, _cacheToUse, _colour}
    , deckColour{_colour}
    , backgroundColour{_backgroundColour}
    , recordImage{_recordImage}
{
    // setup gui buttons
    addAndMakeVisible(playButton);
    playButton.setLookAndFeel(&playButtonLookAndFeel);
    playButton.addListener(this);

    addAndMakeVisible(stopButton);
    stopButton.setLookAndFeel(&stopButtonLookAndFeel);
    stopButton.addListener(this);

    addAndMakeVisible(setCueButton);
    setCueButton.setColour(0x1000100, juce::Colour(60, 60, 60));
    setCueButton.addListener(this);
    
    // Setup the speed slider
    addAndMakeVisible(speedSlider);
    speedSlider.addListener(this);
    speedSlider.setRange(0, 2);
    speedSlider.setValue(1);
    speedSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    speedSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    speedSlider.setLookAndFeel(&speedSliderLookAndFeel);
    speedSlider.setDoubleClickReturnValue(true, 1);

    // Setup the position slider. Note slider is invisible on top of the waveform. This allows the user to click on the waveform in the position desired
    // and the slider will move the track to this position.
    addAndMakeVisible(positionSlider);
    positionSlider.addListener(this);
    positionSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    positionSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    positionSlider.setLookAndFeel(&positionSliderLookAndFeel);

    // Setup song details labels
    addAndMakeVisible(fileNameLabel);
    addAndMakeVisible(artistLabel);
    addAndMakeVisible(songTitleLabel);
    addAndMakeVisible(songLength);
    fileNameLabel.setJustificationType(juce::Justification::topLeft);
    artistLabel.setJustificationType(juce::Justification::topLeft);
    songTitleLabel.setJustificationType(juce::Justification::topLeft);
    songPosition.setJustificationType(juce::Justification::bottomLeft);
    songLength.setJustificationType(juce::Justification::bottomRight);

    // Sets up waveformDisplay to display, but not intercept mouse clicks so that it does not intefere with the position slider
    addAndMakeVisible(waveformDisplay);
    waveformDisplay.setInterceptsMouseClicks(false, false);

    // Sets up the record image
    record.setImage(recordImage);
    addAndMakeVisible(record);

    // starts the timer
    startTimer(timerInterval);
}

DeckGUI::~DeckGUI()
{
    // removes look and feel pointers and stops the timer
    playButton.setLookAndFeel(nullptr);
    stopButton.setLookAndFeel(nullptr);
    speedSlider.setLookAndFeel(nullptr);
    positionSlider.setLookAndFeel(nullptr);
    stopTimer();
}

void DeckGUI::paint (juce::Graphics& g)
{
    // fills background
    g.fillAll (backgroundColour);
}

void DeckGUI::resized()
{
    // sets grid sizes. Note that I have used static cast a lot througt this section. setBounds only takes ints as arguments, and I wanted to be explicit in
    // the values that I was using to avoid complier warnings.
    int rowH = static_cast<int>(getHeight() / 6);
    int columnW = static_cast<int>(getWidth() / 9);

    // size and location of control buttons
    playButton.setBounds(columnW * 2, rowH * 5, columnW * 2, rowH / 2);
    stopButton.setBounds(columnW * 5, rowH * 5, columnW * 2, rowH / 2);

    // size and location of cue buttons. 
    setCueButton.setBounds(static_cast<int>(columnW * 0.2), static_cast<int>(rowH * 2.2), static_cast<int>(columnW * 1.5), rowH / 2);
    for (auto i = 0; i < cuePoints.size(); ++i)
    {
        cuePoints[i]->goToCue.setBounds(static_cast<int>(columnW * 0.2), static_cast<int>(rowH * 2.8) + (i * rowH / 4), static_cast<int>(columnW * 0.8), static_cast<int>(rowH / 4));
        cuePoints[i]->removeCue.setBounds(columnW, static_cast<int>(rowH * 2.8) + (i * rowH / 4), static_cast<int>(columnW * 0.8), rowH / 4);
    }

    // size and location of file detail labels
    fileNameLabel.setBounds(0, 0, static_cast<int>(columnW * 4.5), static_cast<int>(rowH * 0.25));
    artistLabel.setBounds(0, static_cast<int>(rowH * 0.25), static_cast<int>(columnW * 4.5), static_cast<int>(rowH * 0.25));
    songTitleLabel.setBounds(0, static_cast<int>(rowH * 0.5), static_cast<int>(columnW * 4.5), static_cast<int>(rowH * 0.25));
    songLength.setBounds(columnW * 8, static_cast<int>(rowH * 0.8), columnW, static_cast<int>(rowH * 0.2));
    songPosition.setBounds(0, static_cast<int>(rowH * 0.8), columnW, static_cast<int>(rowH * 0.2));

    // size and location of speedSlider
    speedSlider.setBounds(columnW * 8, static_cast<int>(rowH * 2.5), columnW, rowH * 3);
    
    //size and location of record image
    int imageSize{ std::min(columnW * 4, rowH * 2) };
    record.setBounds(static_cast<int>(columnW * 4.5 - (imageSize / 2)), static_cast<int>(rowH * 2.5), imageSize, imageSize);
  
    // size and position of position slider and wavfore image. Note that they are exatly the same. I also overrode the "getSliderLayout" function
    // in the position slider look and feel so that the slider exactly overlays the waveform display
    positionSlider.setBounds(0, rowH, getWidth(), rowH);
    waveformDisplay.setBounds(0, rowH, getWidth(), rowH);
}

void DeckGUI::buttonClicked(juce::Button* button)
{
    // play button functionality. After clicking I also check whether the file is playing or not and adapt the look and feel to match
    if (button == &playButton)
    {             
        if (!player->getPlaying())
        {   
            player->start();
            if (player->getPlaying())
                playButton.setLookAndFeel(&pauseButtonLookAndFeel);
        }
        else
        {
            player->stop();
            playButton.setLookAndFeel(&playButtonLookAndFeel);
        }
        
    }
    if (button == &stopButton)
    {
        player->stop();
        player->setPosition(0.0);
        playButton.setLookAndFeel(&playButtonLookAndFeel);
    }

    // adds CuePoint class to vector, code adapted from here - https://forum.juce.com/t/dynamic-2d-array-of-textbuttons/45624/10
    if (button == &setCueButton)
    {
        if (cuePoints.size() < 5) 
        {
            createCuepoint();
            resized();
        }
    }
    // cuepoint button functionality
    for (auto i = 0; i < cuePoints.size(); ++i)
    {
        if (button == &cuePoints[i]->goToCue)
        {
            player->setPosition(cuePoints[i]->getPosition());
        }
        // updates the numbers on the cuepoint gotocue button
        if (button == &cuePoints[i]->removeCue)
        {
            cuePoints.erase(std::remove(cuePoints.begin(), cuePoints.end(), cuePoints[i]));
            for (auto j = 0; j < cuePoints.size(); ++j)
            {
                std::stringstream label;
                label << j + 1;
                cuePoints[j]->goToCue.setButtonText(label.str());
            }
            resized();
        }
    }
}

void DeckGUI::sliderValueChanged(juce::Slider* slider)
{ 
    if (slider == &speedSlider)
    {
        player->setSpeed(speedSlider.getValue());
    }
    if (slider == &positionSlider)
    {
        player->setPosition(positionSlider.getValue());
    }
}

void DeckGUI::timerCallback()
{
    // updates the songposition label, and spins the record if playing
    songPosition.setText(secondsDoubleToString(player->getPosition()), juce::dontSendNotification);
    waveformDisplay.setPosition(player->getPosition());
    if (player->getPlaying())
    {
        recordRotation += static_cast<float>(0.01f * speedSlider.getValue());
        record.setTransform(juce::AffineTransform::rotation(recordRotation, record.getX() + (record.getWidth() / 2.0f), record.getY() + (record.getHeight() / 2.0f)));
    }        
}


void DeckGUI::addURL(MusicFile musicFile)
{   
    // stops and resets the player
    player->stop();
    player->setPosition(0.0);
    playButton.setLookAndFeel(&playButtonLookAndFeel);
    // removes all cuepoints
    cuePoints.clear();
    // loads the new file and updates everything for the new track
    juce::URL chosenFile{ juce::File { musicFile.getPath() } };
    player->loadFile(juce::File{ musicFile.getPath() });
    waveformDisplay.loadURL(chosenFile);
    double trackLength{ player->getTrackLength() };
    setupSongDetails(musicFile, trackLength);    
    positionSlider.setRange(0, trackLength);
    positionSlider.setValue(0);
}

std::string DeckGUI::secondsDoubleToString(double trackLength)
{
    int minutes{ 0 };
    while ((minutes + 1 ) * 60 < trackLength)
        minutes += 1;
    int seconds{ static_cast<int>(std::floor(fmod(trackLength, 60.0))) };
    trackLength -= minutes * 60;
    std::stringstream length;
    if (seconds < 10)
        length << minutes << ":0" << std::fixed << std::setprecision(0) << seconds;
    else
        length << minutes << ':' << std::fixed << std::setprecision(0) << seconds;
    return length.str();
}

void DeckGUI::setupSongDetails(MusicFile musicFile, double trackLength)
{
    // updates all details for the song
    addAndMakeVisible(songPosition);
    fileNameLabel.setText(musicFile.getFileName(), juce::dontSendNotification);
    artistLabel.setText(musicFile.getArtist(), juce::dontSendNotification);
    songTitleLabel.setText(musicFile.getTitle(), juce::dontSendNotification);
    songLength.setText(secondsDoubleToString(trackLength), juce::dontSendNotification);
    songPosition.setText(secondsDoubleToString(player->getPosition()), juce::dontSendNotification);
}


void DeckGUI::createCuepoint()
{
    auto newCuePoint = std::make_unique<CuePoint>(player->getPosition());
    std::stringstream label;
    label << cuePoints.size() + 1;
    newCuePoint->goToCue.setButtonText(label.str());
    newCuePoint->goToCue.setColour(0x1000100, juce::Colour(60, 60, 60));
    newCuePoint->goToCue.addListener(this);
    addAndMakeVisible(newCuePoint->goToCue);
    newCuePoint->removeCue.setColour(0x1000100, juce::Colour(60, 60, 60));
    newCuePoint->removeCue.addListener(this);
    addAndMakeVisible(newCuePoint->removeCue);
    cuePoints.push_back(std::move(newCuePoint));
}