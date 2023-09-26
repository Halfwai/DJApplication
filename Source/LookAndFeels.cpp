/*
  ==============================================================================

    LookAndFeels.cpp
    Created: 29 Aug 2023 10:30:48am
    Author:  Wai-San

  ==============================================================================
*/

#include "LookAndFeels.h"


EQSliderLookAndFeel::EQSliderLookAndFeel(juce::Colour& _colour) : colour{ _colour }
{

}

// Some of this code has been adapted from this video from the AudioProgrammer - https://youtu.be/_IocXoXUWdQ?si=Vda6Mou52VeZvNTU
void EQSliderLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, juce::Slider&)
{
    // Set up variable to draw the slider 
    float sideLength{ juce::jmin(static_cast<float>(width), static_cast<float>(height)) };
    float sizeUnit{ sideLength / 10 };
    float diameter{ sideLength - (sizeUnit * 2) };
    float radius{ diameter / 2 };
    float tickAreaWidth{ radius / 5 };
    float tickWidth{ radius / 10 };
    float centreX{ x + static_cast<float>(width) / 2 };
    float centreY{ y + static_cast<float>(height) / 2 };
    float rx{ centreX - radius };
    float ry{ centreY - radius };
    float angle{ rotaryStartAngle + (sliderPosProportional * (rotaryEndAngle - rotaryStartAngle)) };
    float distanceBetweenPips{ (rotaryEndAngle - rotaryStartAngle) / (numOfPips - 1) };
    juce::Rectangle<float> dialArea{ rx, ry, diameter, diameter };
    // draws the slider based on these variables
    g.setColour(juce::Colours::grey);
    g.fillEllipse(dialArea);
    g.setColour(juce::Colours::darkgrey);
    g.fillEllipse(rx + tickAreaWidth, ry + tickAreaWidth, (diameter / 5) * 4, (diameter / 5) * 4);
    juce::Path dialTick{};
    dialTick.addRectangle(-(tickWidth/2), -radius, tickWidth, radius);
    g.setColour(juce::Colours::white);
    g.fillPath(dialTick, juce::AffineTransform::rotation(angle).translated(centreX, centreY));
    g.setColour(juce::Colours::white);
    
    for (auto i = 0; i < numOfPips; ++i)
    {
        float pipAngle{ rotaryStartAngle + (distanceBetweenPips * i) };
        if (angle >= pipAngle)
        {
            g.setColour(colour);
        }
        else
        {
            g.setColour(juce::Colours::white);
        }
        float pipSize{ tickAreaWidth };
        juce::Path pip{};
        pip.addEllipse(-pipSize / 2, -(radius + (pipSize * 1.2f)), pipSize, pipSize );
        g.fillPath(pip, juce::AffineTransform::rotation(rotaryStartAngle + (distanceBetweenPips * i)).translated(centreX, centreY));
    }
}


VolumeSliderLookAndFeel::VolumeSliderLookAndFeel(juce::Colour& _colour) : colour{ _colour }
{

}


void VolumeSliderLookAndFeel::drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float, const juce::Slider::SliderStyle, juce::Slider&)
{
    // Set up variable to draw the slider 
    float centreX{ static_cast<float>(x) + (static_cast<float>(width) / 2) };
    float barWidth{ static_cast<float>(width) / 4 };
    float barHeight{ static_cast<float>(height) };
    float sliderKnobWidth{ barWidth * 2 };
    float sliderKnobHeight{ static_cast<float>(height) * 0.2f };
    float sliderKnob{ sliderPos - (sliderKnobHeight / 2) };
    float distanceBetweenTicks{ barHeight / (numOfTicks - 1) };
    float tickHeight{ barHeight / (numOfTicks * 5) };
    float tickStartPoint{ (static_cast<float>(y) + barHeight) - (tickHeight / 2) };
    float tickWidth{ barWidth * 2 };
    float leftTickX{ centreX - (tickWidth * 1.6f) };

    // draws the slider based on these variables
    juce::Rectangle<float> topBar{ centreX - (barWidth / 2), static_cast<float>(y), barWidth, sliderPos - static_cast<float>(y) };
    juce::Rectangle<float> bottomBar{ centreX - (barWidth / 2), sliderPos, barWidth, barHeight - (sliderPos - static_cast<float>(y)) };

    g.setColour(juce::Colours::black);
    g.fillRect(topBar);
    g.setColour(colour);
    g.fillRect(bottomBar);

    for (auto i = 0; i < numOfTicks; ++i)
    {
        float tickY{ tickStartPoint - (distanceBetweenTicks * i) };
        g.setColour(juce::Colours::white);
        if (sliderKnob + (sliderKnobHeight / 2) <= tickY + tickHeight && sliderPos != minSliderPos)
            g.setColour(colour);
        if (i == 10)
        {
            juce::Rectangle<float> rightTick{ leftTickX, tickY, tickWidth * 4, tickHeight };
            g.fillRect(rightTick);
        }
        juce::Rectangle<float> leftTick{ leftTickX, tickY, tickWidth, tickHeight };
        g.fillRect(leftTick);
    }
    g.setColour(juce::Colours::grey);
    juce::Rectangle<float> knobPosition{ centreX - (sliderKnobWidth / 2), sliderKnob, sliderKnobWidth, sliderKnobHeight };
    g.drawImage(sliderImage, knobPosition);
}

juce::Slider::SliderLayout VolumeSliderLookAndFeel::getSliderLayout(juce::Slider& s)
{
    // reduces the size of the slider within its bounds
    juce::Slider::SliderLayout layout{};
    int gap{ s.getHeight() / 10 };
    int sliderY{ static_cast<int>(s.getY()) };
    int sliderHeight{ static_cast<int>(s.getHeight() - (gap * 2)) };
    juce::Rectangle<int> slider{ 0, sliderY, s.getWidth(), sliderHeight };
    layout.sliderBounds = slider;
    return layout;
}

VolumeMixerLookAndFeel::VolumeMixerLookAndFeel(juce::Colour& _leftColour, juce::Colour& _rightColour)
    : leftColour{ _leftColour }
    , rightColour{ _rightColour }
{

}

void VolumeMixerLookAndFeel::drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float, float, const juce::Slider::SliderStyle, juce::Slider&)
{
    // Set up variable to draw the slider
    float sliderHeight{ height / 1.5f };
    float sliderYPos{ height / 2.0f - (sliderHeight / 2) };
    float sliderWidth{ width / 30.0f };
    float sliderXPos{ sliderPos - (sliderWidth / 2) };
    float tickDistance{ static_cast<float>(width) / (numberOfTicks - 1) };
    float tickWidth{ tickDistance / 10 };
    float tickHeight{ height / 2.0f };
    float tickY{ y + (tickHeight / 2) };

    // draws the slider based on these variables
    g.setColour(juce::Colour(50, 50, 50));
    g.fillRect(x, y, width, height);
    g.setColour(juce::Colours::white);
    for (auto i = 0; i < numberOfTicks; ++i)
    {
        if (i == 10)
        {
            juce::Rectangle tick{(x + (i * tickDistance)) - (tickWidth / 2), static_cast<float>(y), tickWidth, static_cast<float>(height) };
            g.fillRect(tick);
        }
        else
        {
            juce::Rectangle tick{(x + (i * tickDistance)) - (tickWidth / 2), tickY, tickWidth, tickHeight };
            g.fillRect(tick);
        }
    }    
    juce::Rectangle leftsliderBar{static_cast<float>(x), (height / 2.0f) - (height / 20.0f), sliderPos - x, height / 10.0f};
    g.setColour(rightColour);
    g.fillRect(leftsliderBar);
    juce::Rectangle rightSliderBar{sliderPos, (height / 2.0f) - (height / 20.0f), width - sliderPos + x, height / 10.0f};
    g.setColour(leftColour);
    g.fillRect(rightSliderBar);
    juce::Rectangle<float> sliderPosition{sliderXPos, sliderYPos, sliderWidth, sliderHeight };
    g.drawImage(loadedImage, sliderPosition);
}


SpeedSliderLookAndFeel::SpeedSliderLookAndFeel(juce::Colour& _colour) : colour{_colour}
{

}

void SpeedSliderLookAndFeel::drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float, float, const juce::Slider::SliderStyle, juce::Slider&)
{
    // Set up variable to draw the slider
    float barHeight{ static_cast<float>(height) };
    float barY{ static_cast<float>(y) };
    float barWidth{ width / 4.0f };
    float barX{ ((x + width) / 2.0f) - (barWidth / 2) };
    float sliderHeight{ height / 20.0f };
    float sliderY{ sliderPos - (sliderHeight / 2) };
    float sliderWidth{ static_cast<float>(width) };
    float sliderX{ ((x + width) / 2.0f) - (sliderWidth / 2) };
    float tickDistance{ static_cast<float>(height) / (numOfTicks - 1) };
    float tickHeight{ height / 100.0f };
    float tickY{ y - (tickHeight / 2) };
    float tickWidth{ width / 1.5f };
    float tickX{ ((x + width) / 2.0f) - (tickWidth/2) };
    
    // draws the slider based on these variables
    for (auto i = 0; i < numOfTicks; ++i)
    {
        g.setColour(juce::Colours::white);
        if (sliderPos < height / 2.0f && tickY < height / 2.0f)        
        {
            if (sliderPos < tickY)
                g.setColour(colour);
        }
        if (sliderPos > height / 2.0f && tickY > height / 2.0f)
        {
            if (sliderPos > tickY)
                g.setColour(colour);
        }
        juce::Rectangle<float> tick{ tickX, tickY, tickWidth, tickHeight };
        if (i == 5)
        {
            tick = juce::Rectangle{ static_cast<float>(x), tickY, static_cast<float>(width), tickHeight};  
            g.setColour(juce::Colours::white);
        }                     
        g.fillRect(tick);
        tickY += tickDistance;
    }    
    juce::Rectangle bar{ barX, barY, barWidth, barHeight };
    g.setColour(juce::Colours::black);
    g.fillRect(bar);
    juce::Rectangle<float> sliderPosition{ sliderX, sliderY, sliderWidth, sliderHeight };
    g.drawImage(loadedImage, sliderPosition);
}


ControlButtonLookAndFeel::ControlButtonLookAndFeel(const juce::Image& _image, juce::Colour& _colour) 
    : image{ _image }
    , colour{_colour}
{

}

void ControlButtonLookAndFeel::drawButtonBackground(juce::Graphics& g, juce::Button&, const juce::Colour&, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
    // Set up variable to draw the button
    float imageWidth{ g.getClipBounds().getWidth() / 4.0f };
    float imageX{ (g.getClipBounds().getWidth() / 2) - (imageWidth / 2) };
    float imageHeight{ imageWidth };
    float imageY{ (g.getClipBounds().getHeight() / 2.0f) - (imageHeight / 2) };
   
    // draws the button based on these variables
    juce::Rectangle imagePosition{ imageX, imageY, imageWidth, imageHeight };    
    g.setColour(juce::Colours::white);
    if (shouldDrawButtonAsHighlighted)
        g.setColour(colour);
    g.drawRoundedRectangle(g.getClipBounds().toFloat(), 10.0f, 2.0f);
    g.setColour(juce::Colour(50, 50, 50));
    if (shouldDrawButtonAsDown)
        g.setColour(colour);
    g.fillRoundedRectangle(g.getClipBounds().toFloat(), 10.0f);
    g.drawImage(image, imagePosition);
    
}

void PositionSliderLookAndFeel::drawLinearSlider(juce::Graphics&, int, int, int, int, float, float, float, const juce::Slider::SliderStyle, juce::Slider&)
{

}


juce::Slider::SliderLayout PositionSliderLookAndFeel::getSliderLayout(juce::Slider& s)
{
    juce::Slider::SliderLayout layout{};
    juce::Rectangle sliderBox{ s.getX(), s.getY(), s.getWidth(), s.getHeight() };
    layout.sliderBounds = sliderBox;
    return layout;
}