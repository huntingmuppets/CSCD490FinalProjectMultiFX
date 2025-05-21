/*
  ==============================================================================

    ChorusProcessor.h
    Created: 23 Jan 2024 5:53:25pm
    Author:  Grant Howard

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
//#include "Base.h"

class ChorusProcessor : public juce::AudioProcessor, 
                        public juce::AudioProcessorValueTreeState::Listener
{
public:
    //==============================================================================
    ChorusProcessor();
    ~ChorusProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
//    void prepare (const juce::dsp::ProcessSpec& spec) noexcept;
    void reset() override;
    void releaseResources() override;
    void processBlock (juce::AudioSampleBuffer&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;
    bool acceptsMidi() const override;
    bool producesMidi() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    juce::AudioProcessorValueTreeState apvts;
private:
    
    juce::dsp::Chorus<float> chorus;
    juce::AudioProcessorValueTreeState::ParameterLayout createParameters();
    
    juce::AudioPlayHead::CurrentPositionInfo positionInfo;
    
    int bpm { 0 };
    void parameterChanged (const juce::String& parameterID, float newValue) override;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ChorusProcessor)
};

