/*
  ==============================================================================

    ReverbProcessor.h
    Created: 23 Jan 2024 5:53:37pm
    Author:  Grant Howard

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
//#include "Base.h"

class CompressorProcessor : public juce::AudioProcessor,
                            public juce::AudioProcessorValueTreeState::Listener {
public:
    //==============================================================================
    CompressorProcessor();
    ~CompressorProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void processBlock (juce::AudioSampleBuffer&, juce::MidiBuffer&) override;
    void reset() override;

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
    
    juce::dsp::Compressor<float> compressor;
    juce::AudioProcessorValueTreeState::ParameterLayout createParameters();
    
    juce::AudioPlayHead::CurrentPositionInfo positionInfo;
    
//    int bpm { 0 };
    void parameterChanged (const juce::String& parameterID, float newValue) override;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CompressorProcessor)
};
