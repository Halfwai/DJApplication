#pragma once

#include <JuceHeader.h>
#include <vector>

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class EQSliderLookAndFeel : public juce::LookAndFeel_V4
{
public:
    /* Constructs an EQ slider look and feel using a juce::Colour as a parameter */
    EQSliderLookAndFeel(juce::Colour& _colour);

    /* overrides the drawRotarySlider method to draw a custom slider */
    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider) override;
   
private:
    // Sets the numbe of pips used in the slider
    int numOfPips{ 11 };

    // The colour the slider uses, set up in the constructor
    juce::Colour colour{ };
};


class VolumeSliderLookAndFeel :  public juce::LookAndFeel_V4
{
public:
    /* Constructs a volume slider look and feel using a juce::Colour as a parameter */
    VolumeSliderLookAndFeel(juce::Colour& _colour);    
    
    /* overrides the drawLinearSlider method to draw a custom slider */
    void drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, const juce::Slider::SliderStyle, juce::Slider& slider);

    /* Used to make the slider a little smaller in its bounds, to prevent the image used as the slider from clipping at the top and bottom */
    juce::Slider::SliderLayout getSliderLayout(juce::Slider& s) override;


private:
    // sets the number of ticks drawn on the slider
    int numOfTicks{ 16 };

    // The colour the slider uses, set up in the constructor
    juce::Colour colour;

    // image taken from https://www.vectorstock.com/royalty-free-vector/sound-mixer-console-dj-equipment-slider-buttons-vector-18496457
    const juce::Image sliderImage = juce::ImageCache::getFromMemory(BinaryData::sliderDial_png, BinaryData::sliderDial_pngSize);
};


class VolumeMixerLookAndFeel : public juce::LookAndFeel_V4
{
public:
    /* Constructs a volume mixer slider look and feel using two juce::Colour as a parameter */
    VolumeMixerLookAndFeel(juce::Colour& _leftColour, juce::Colour& _rightColour);

    /* overrides the drawLinearSlider method to draw a custom slider */
    void drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, const juce::Slider::SliderStyle, juce::Slider& slider);

private:
    // the colours used in the left and right decks, used to customize the colours of the volume mixer slider
    juce::Colour leftColour;
    juce::Colour rightColour;

    // image taken from https://www.crushpixel.com/stock-vector/mixing-console-1696724.html
    const juce::Image loadedImage = juce::ImageCache::getFromMemory(BinaryData::volumeMixerDial_png, BinaryData::volumeMixerDial_pngSize);

    // sets the number of ticks drawn on the slider
    int numberOfTicks{ 21 }; 
};

class SpeedSliderLookAndFeel : public juce::LookAndFeel_V4
{
public:
    /* Constructs a speed slider look and feel using two juce::Colour as a parameter */
    SpeedSliderLookAndFeel(juce::Colour& _colour);

    /* overrides the drawLinearSlider method to draw a custom slider */
    void drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, const juce::Slider::SliderStyle, juce::Slider& slider);

private:

    // image taken from https://www.crushpixel.com/stock-vector/mixing-console-1696724.html
    const juce::Image loadedImage = juce::ImageCache::getFromMemory(BinaryData::speedSliderDial_png, BinaryData::speedSliderDial_pngSize);

    // sets the number of ticks drawn on the slider
    int numOfTicks{ 11 };

    // The colour the slider uses, set up in the constructor
    juce::Colour colour{};
};

class ControlButtonLookAndFeel : public juce::LookAndFeel_V4
{
public:
    /* Constructs a control button look and feel using an image to display on the button, and juce::Colour as a parameter */
    ControlButtonLookAndFeel(const juce::Image& _image, juce::Colour& _colour);

    /* overrides the drawButtonBackground method to draw a custom button */
    void drawButtonBackground(juce::Graphics& g, juce::Button& button, const juce::Colour& backgroundColour, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown);

private:
    // The image to be shown on the button, loaded in the constructor
    const juce::Image image{};

    // Colour used when mouse hovers and clicks on button
    const juce::Colour colour{};
};

class PositionSliderLookAndFeel : public juce::LookAndFeel_V4
{
public:
    /* overrides the drawLinearSlider method to draw a custom slider. Note in this case the function is empty as I want the 
    position slider to be invisible */
    void drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, const juce::Slider::SliderStyle, juce::Slider& slider);

    /* Stretches the slider accross it's entire length for better interaction with the wavefore display */
    juce::Slider::SliderLayout getSliderLayout(juce::Slider& s) override;
private:

};
