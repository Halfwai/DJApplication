/*
  ==============================================================================

    DJAudioPlayer.cpp
    Created: 20 Aug 2023 2:23:12pm
    Author:  wai_s

  ==============================================================================
*/

#include "DJAudioPlayer.h"

DJAudioPlayer::DJAudioPlayer(juce::AudioFormatManager& _formatManager)
    : formatManager{ _formatManager }
{

}

DJAudioPlayer::~DJAudioPlayer()
{
    transportSource.releaseResources();
    resampleSource.releaseResources();
    bass.releaseResources();
    mid.releaseResources();
    high.releaseResources();
}

void DJAudioPlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    // saves the sample rate of the loaded track
    rate = sampleRate;
    
    // Sets up each audio sources
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);

    bass.setCoefficients(juce::IIRCoefficients::makeLowShelf(sampleRate, 200.0, 1.0, 1));
    bass.prepareToPlay(samplesPerBlockExpected, sampleRate);

    mid.setCoefficients(juce::IIRCoefficients::makePeakFilter(sampleRate, 4000, 0.5, 1));
    mid.prepareToPlay(samplesPerBlockExpected, sampleRate);

    high.setCoefficients(juce::IIRCoefficients::makeHighShelf(sampleRate, 200.0, 1.0, 1));
    high.prepareToPlay(samplesPerBlockExpected, sampleRate);



}
void DJAudioPlayer::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    // Clears the buffer if the track is not playing
    if (!playing)
    {
        bufferToFill.clearActiveBufferRegion();
        return;
    }
    resampleSource.getNextAudioBlock(bufferToFill);
    // sets left and right level variables 
    leftSoundLevel = bufferToFill.buffer->getMagnitude(0, bufferToFill.startSample, bufferToFill.numSamples);
    rightSoundLevel = bufferToFill.buffer->getMagnitude(1, bufferToFill.startSample, bufferToFill.numSamples);

}

void DJAudioPlayer::releaseResources()
{
    transportSource.releaseResources();
    resampleSource.releaseResources();
    bass.releaseResources();
    mid.releaseResources();
    high.releaseResources();
}

void DJAudioPlayer::loadFile(juce::File audioFile)
{
    // audioURL.createInputStream() has been deprieciated, function converted to take juce::File insead. 
    auto* reader = formatManager.createReaderFor(audioFile);
    if (reader != nullptr) // good file!
    {         
        // Loads up the transport source with the track
        std::unique_ptr<juce::AudioFormatReaderSource> newSource
        (new juce::AudioFormatReaderSource(reader, true));
        transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
        readerSource.reset(newSource.release());
    }
    else
    {
        DBG("Something went wrong loading the file");
    }
}

void DJAudioPlayer::setGain(double gain)
{
    // note I decided to give the user a little bit of room to overdrive the gain on their track. This was implimented by design, I like the overdrive sound
    // and I want to give the user the option to use it if they desire.
    if (gain < 1.5 && gain >= 0)
        transportSource.setGain(static_cast<float>(gain));
}

void DJAudioPlayer::setSpeed(double speedRatio)
{
    if (speedRatio < 2 && speedRatio >= 0)
        resampleSource.setResamplingRatio(speedRatio);
}

void DJAudioPlayer::setPosition(double posInSeconds)
{
    if (posInSeconds < getTrackLength() && posInSeconds >= 0)
        transportSource.setPosition(posInSeconds);
}

void DJAudioPlayer::start()
{
    transportSource.start();
    playing = transportSource.isPlaying();
}

void DJAudioPlayer::stop()
{
    transportSource.stop();
    playing = transportSource.isPlaying();
}

double DJAudioPlayer::getPosition() const
{
    return transportSource.getCurrentPosition();
}

double DJAudioPlayer::getTrackLength() const
{
    double length = transportSource.getLengthInSeconds();
    // track length is used to calculate the scale of gui objects. To prevent zero division errors, will return 1 if length is equal to zero.
    if (length == 0)
    {
        return 1;
    }
    return length;
}

void DJAudioPlayer::setLow(double bassLevel)
{
    if (bassLevel < 2 && bassLevel > 0)
        bass.setCoefficients(juce::IIRCoefficients::makeLowShelf(rate, 200.0, 1.0, static_cast<float>(bassLevel)));
}

void DJAudioPlayer::setMid(double midLevel)
{
    if (midLevel < 2 && midLevel > 0)
        mid.setCoefficients(juce::IIRCoefficients::makePeakFilter(rate, 4000, 0.5, static_cast<float>(midLevel)));
}

void DJAudioPlayer::setHigh(double highLevel)
{
    if (highLevel < 2 && highLevel > 0)
        high.setCoefficients(juce::IIRCoefficients::makeHighShelf(rate, 200.0, 1.0, static_cast<float>(highLevel)));
}

std::vector<float> DJAudioPlayer::getSoundLevel()
{
    std::vector<float> channelsMagnitude{};
    channelsMagnitude.push_back(leftSoundLevel);
    channelsMagnitude.push_back(rightSoundLevel);    
    return channelsMagnitude;
}

bool DJAudioPlayer::getPlaying()
{
    return playing;
}
