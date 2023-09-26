/*
  ==============================================================================

    DJAudioPlayer.h
    Created: 20 Aug 2023 2:23:12pm
    Author:  wai_s

  ==============================================================================
*/
#include <JuceHeader.h>
#include <vector>
#pragma once

class DJAudioPlayer : public juce::AudioSource {
    public:
        /* Constructor function, takes the app AudioFormatManager as an argument */
        DJAudioPlayer(juce::AudioFormatManager& formatManager);
        
        /* Destructor function, releases all the resources from the audio sources */
        ~DJAudioPlayer();

        /* prepares each audio source when a track is loaded and saves the sample rate of that track in a private variable for use
        with the filter sources */
        void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
        
        /* processes the next block of audio that the AudioSource needs to play*/
        void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
        
        /* releases the resources of each of the audio sources */
        void releaseResources() override;

        /* Takes a juce::File and loads it into the audioSource */
        void loadFile(juce::File audioFile);
        
        /* Sets the gain for the audio channel */
        void setGain(double gain);

        /* Sets the spped at which the audio file is played */
        void setSpeed(double speedRatio);
        
        /* Sets the position from which the audio file should play from */
        void setPosition(double posInSeconds);
        
        /* Starts the audio file */
        void start();

        /* Stops the audio file */
        void stop();

        /* Returns the current position of the audiofile */
        double getPosition() const;

        /* Gets the total track length of the file currently loaded in to the audioplayer */
        double getTrackLength() const;

        /* Set low levels on the track using a low shelf */
        void setLow(double bass);

        /* Set mid levels on the track using a peak filter */
        void setMid(double midLevel);

        /* Set high levels on the track using a high shelf */
        void setHigh(double highLevel);

        /* Returns a vector of the current sound levels of the track. In the 0 position we have the left channel and 
        in the 1 position we have the right channel */
        std::vector<float> getSoundLevel();

        /* returns if the audiosource is currently playing */
        bool getPlaying();

    private:
        // Audio formating variables used to process audio files
        juce::AudioFormatManager& formatManager;
        std::unique_ptr<juce::AudioFormatReaderSource> readerSource{};
        
        // Layers in the audio formatting process
        juce::AudioTransportSource transportSource{};

        // Filters used to change the sound
        juce::IIRFilterAudioSource bass{&transportSource, false};
        juce::IIRFilterAudioSource high{&bass, false};
        juce::IIRFilterAudioSource mid{&high, false};
                       
        // Resample sources is used to spped up and slow down the track
        juce::ResamplingAudioSource resampleSource{&mid, false, 2};
        
        // holds variable to see if audioSource is playing
        bool playing{ false };

        // Holds the sample rate of the currently loaded track
        double rate{};

        // Holds the current sound levels of the left and right channel
        float leftSoundLevel{};
        float rightSoundLevel{};
};