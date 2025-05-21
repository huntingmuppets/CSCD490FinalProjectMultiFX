/*
  ==============================================================================

    ReverbProcessor.cpp
    Created: 23 Jan 2024 5:53:37pm
    Author:  Grant Howard

  ==============================================================================
*/

#include "CompressorProcessor.h"
#include "PluginEditor.h"

// Constructor has bus layout stero-stero or mono-mono
CompressorProcessor::CompressorProcessor() : AudioProcessor (BusesProperties().withInput ("Input", juce::AudioChannelSet::stereo())
                                                            .withOutput ("Output", juce::AudioChannelSet::stereo())),
                                                            apvts(*this, nullptr, "Parameters", createParameters())
{
    apvts.addParameterListener ("THRESHOLD", this);
    apvts.addParameterListener ("RATIO", this);
    apvts.addParameterListener ("ATTACK", this);
    apvts.addParameterListener ("RELEASE", this);
    apvts.addParameterListener("BYPASS", this);
}

CompressorProcessor::~CompressorProcessor()
{
    apvts.removeParameterListener ("THRESHOLD", this);
    apvts.removeParameterListener ("RATIO", this);
    apvts.removeParameterListener ("ATTACK", this);
    apvts.removeParameterListener ("RELEASE", this);
    apvts.removeParameterListener ("MIX", this);
    apvts.removeParameterListener("BYPASS", this);
}

juce::AudioProcessorValueTreeState::ParameterLayout CompressorProcessor::createParameters()
{
    juce::AudioProcessorValueTreeState::ParameterLayout params;
    
    using Range = juce::NormalisableRange<float>;
    
    params.add (std::make_unique<juce::AudioParameterFloat>  (juce::ParameterID {"THRESHOLD", 1}, "Threshold", Range {-60.0f, 0.0f, 0.01f}, 0.0f));
    params.add (std::make_unique<juce::AudioParameterFloat>(juce::ParameterID {"RATIO", 1}, "Ratio", Range { 1.0f, 100.0f, 0.01f }, 1.0f));
    params.add (std::make_unique<juce::AudioParameterFloat>  (juce::ParameterID {"ATTACK", 1}, "Attack", Range {0.0f, 100.0f, 0.01f}, 50.0f));
    params.add (std::make_unique<juce::AudioParameterFloat>(juce::ParameterID {"RELEASE", 1}, "Release", Range { 0.0f, 1000.0f, 0.01f }, 500.0f));
    params.add(std::make_unique<juce::AudioParameterBool>(juce::ParameterID {"BYPASS", 1}, "Bypass", false));
    
    return params;
}

void CompressorProcessor::parameterChanged (const juce::String& parameterID, float newValue)
{
    if (parameterID == "THRESHOLD")
        compressor.setThreshold(newValue);
    
    if (parameterID == "RATIO")
        compressor.setRatio (newValue);
    
    if (parameterID == "ATTACK")
        compressor.setAttack (newValue);
    
    if (parameterID == "RELEASE")
        compressor.setRelease (newValue);
}

void CompressorProcessor::reset()
{
    compressor.reset();
}

// Get name function
const juce::String CompressorProcessor::getName() const
{
    // Gets the name of the juce project... ignore
    return "Compressor";
}

// Accept midi function is always false (audio only)
bool CompressorProcessor::acceptsMidi() const
{
    return false;
}

bool CompressorProcessor::producesMidi() const
{
    return false;
}

// Returns length of the processor tail (don't need)
double CompressorProcessor::getTailLengthSeconds() const
{
    return 0;
}

// We only have one host program, PluginProcessor, return 0
int CompressorProcessor::getNumPrograms()
{
    return 0;
}

// Returns the number of the currently active program. Used with AudioProcessorGraph
int CompressorProcessor::getCurrentProgram()
{
    return 0;
}

// Called by the host to change the current program. Used with AudioProcessorGraph
void CompressorProcessor::setCurrentProgram (int index)
{
}

// Must return the name of a given program. Used with AudioProcessorGraph
const juce::String CompressorProcessor::getProgramName (int index)
{
    return {};
}

// Called by the host to rename a program. Used with AudioProcessorGraph
void CompressorProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================

// Called before playback starts, to let the processor prepare itself.
void CompressorProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // PRE DSP LOGIC HERE
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = getTotalNumOutputChannels();
    
    compressor.prepare (spec);
    compressor.reset();

}


void CompressorProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    // DSP LOGIC HERE
    bool isBypassed = *apvts.getRawParameterValue("BYPASS") != 0.0f;
    
    if(!isBypassed)
    {
        juce::ScopedNoDenormals noDenormals;
        auto totalNumInputChannels  = getTotalNumInputChannels();
        auto totalNumOutputChannels = getTotalNumOutputChannels();

        for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
            buffer.clear (i, 0, buffer.getNumSamples());

        juce::dsp::AudioBlock<float> sampleBlock (buffer);
        juce::dsp::ProcessContextReplacing<float> context (sampleBlock);
        
        compressor.process (context);
    } else
    {
        processBlockBypassed(buffer, midiMessages);
    }
}

//juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
//{
//    return new CompressorProcessor();
//}

// Called after playback has stopped, to let the object free up any resources it no longer needs.
void CompressorProcessor::releaseResources()
{
}

// Determines if there will be an editor
bool CompressorProcessor::hasEditor() const
{
    return false;
}

// Creates generic editor window for effect
juce::AudioProcessorEditor* CompressorProcessor::createEditor()
{
//    return new MultiFXAudioProcessorEditor (*this);
    return nullptr;
//    return new juce::GenericAudioProcessorEditor(*this);
}


// Both methods below will save and load presets
void CompressorProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    
}

void CompressorProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    
}
