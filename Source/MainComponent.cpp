#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // sets the initial size of the app window
    setSize(1000, 800);

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired(juce::RuntimePermissions::recordAudio)
        && !juce::RuntimePermissions::isGranted(juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request(juce::RuntimePermissions::recordAudio,
            [&](bool granted) { setAudioChannels(granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels(2, 2);
    }

    // add and make visible on all the gui components
    addAndMakeVisible(deck1);
    addAndMakeVisible(deck2);
    addAndMakeVisible(centreConsole);
    addAndMakeVisible(playlistComponent);

    // sets the format manager to take audio files
    formatManager.registerBasicFormats();
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    // adds sources to the mixer source and readies it
    mixerSource.addInputSource(&leftPlayer, false);
    mixerSource.addInputSource(&rightPlayer, false);
    mixerSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    mixerSource.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
    mixerSource.removeAllInputs();
    mixerSource.releaseResources();
    leftPlayer.releaseResources();
    rightPlayer.releaseResources();
}

void MainComponent::paint(juce::Graphics& g)
{
    // fills the background
    g.fillAll(backgroundColour);
}

void MainComponent::resized()
{
    deck1.setBounds(0, 0, getWidth() / 3, getHeight()/2);
    deck2.setBounds(getWidth() / 3 * 2, 0, getWidth() / 3, getHeight()/2);
    centreConsole.setBounds(getWidth() / 3, 0, getWidth() / 3, getHeight() / 2);
    playlistComponent.setBounds(0, getHeight()/2, getWidth(), getHeight()/2);
}
