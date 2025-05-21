/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "ChorusProcessor.h"
//#include "MainComponent.h"

//==============================================================================
/**
*/
class ActiveComponent : public juce::Component 
{
public:
    ActiveComponent();
    ~ActiveComponent() override;

    void paint(juce::Graphics&) override;
    void resized() override;

    
    juce::AudioProcessorEditor* mainEditor = nullptr;

};
class MultiFXAudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::Button::Listener
{
public:
    MultiFXAudioProcessorEditor (MultiFXAudioProcessor&);
    ~MultiFXAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    void buttonClicked(juce::Button* button) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    MultiFXAudioProcessor& audioProcessor;

    ActiveComponent audioWindow;

    juce::ListBox activeEffects;
    juce::ListBox effectLibrary;



    juce::TextButton reverb;
    juce::TextButton chorus;
    juce::TextButton muteChorus;
    juce::TextButton muteReverb;
    //juce::ImageButton muteChorus;
    //juce::ImageButton muteReverb;
    juce::File mutedPic = juce::File::getCurrentWorkingDirectory().getChildFile("Muted.png");
    juce::File unMutedPic = juce::File::getCurrentWorkingDirectory().getChildFile("Unmuted.png");
    juce::Image muted = juce::ImageFileFormat::loadFrom(mutedPic);
    juce::Image unmuted = juce::ImageFileFormat::loadFrom(unMutedPic);
    //juce::Drawable *mutedSVG{ juce::DrawableImage::createFromImageFile(mutedPic) };
    juce::TextButton save;
    juce::TextButton load;
    
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MultiFXAudioProcessorEditor)
};



