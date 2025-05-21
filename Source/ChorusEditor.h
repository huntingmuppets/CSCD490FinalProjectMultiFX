/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
// May have to use PluginProcessor.h, check my logic here
//#include "PluginProcessor.h"
#include "ChorusProcessor.h"

//==============================================================================
/**
*/

class MultiFXChorusEditor  : public juce::Component
{
public:
    MultiFXChorusEditor (ChorusProcessor&);
    ~MultiFXChorusEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    ChorusProcessor& audioProcessor;
    
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MultiFXChorusEditor)
};



