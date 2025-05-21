/*
  ==============================================================================

    ChorusProcessor.cpp
    Created: 23 Jan 2024 5:53:25pm
    Author:  Grant Howard

  ==============================================================================
*/

#include "ChorusProcessor.h"
#include "PluginEditor.h"

ChorusProcessor::ChorusProcessor() : AudioProcessor (BusesProperties().withInput ("Input", juce::AudioChannelSet::stereo())
                                                     .withOutput ("Output", juce::AudioChannelSet::stereo())),
                                                     apvts(*this, nullptr, "Parameters", createParameters())
{
    apvts.addParameterListener ("RATE", this);
    apvts.addParameterListener ("DEPTH", this);
    apvts.addParameterListener ("CENTREDELAY", this);
    apvts.addParameterListener ("FEEDBACK", this);
    apvts.addParameterListener ("MIX", this);
    apvts.addParameterListener("BYPASS", this);
}

ChorusProcessor::~ChorusProcessor()
{
    apvts.removeParameterListener ("RATE", this);
    apvts.removeParameterListener ("DEPTH", this);
    apvts.removeParameterListener ("CENTREDELAY", this);
    apvts.removeParameterListener ("FEEDBACK", this);
    apvts.removeParameterListener ("MIX", this);
}


juce::AudioProcessorValueTreeState::ParameterLayout ChorusProcessor::createParameters()
{
    juce::AudioProcessorValueTreeState::ParameterLayout params;
    
    using Range = juce::NormalisableRange<float>;
    
    params.add (std::make_unique<juce::AudioParameterInt>  (juce::ParameterID {"RATE", 1}, "Rate", 0, 15, 0));
    params.add (std::make_unique<juce::AudioParameterFloat>(juce::ParameterID {"DEPTH", 1}, "Depth", Range { 0.0f, 0.4f, 0.01f }, 0.0f));
    params.add (std::make_unique<juce::AudioParameterInt>  (juce::ParameterID {"CENTREDELAY", 1}, "Centre Delay", 1, 50, 1));
    params.add (std::make_unique<juce::AudioParameterFloat>(juce::ParameterID {"FEEDBACK", 1}, "Feedback", Range { -1.0f, 1.0f, 0.01f }, 0.0f));
    params.add (std::make_unique<juce::AudioParameterFloat>(juce::ParameterID {"MIX", 1}, "Mix", Range { 0.0f, 1.0f, 0.01f }, 0.0f));
    params.add(std::make_unique<juce::AudioParameterBool>(juce::ParameterID {"BYPASS", 1}, "Bypass", false));
    
    return params;
}

void ChorusProcessor::parameterChanged (const juce::String& parameterID, float newValue)
{
    if (parameterID == "RATE")
        chorus.setRate (newValue);
    
    if (parameterID == "DEPTH")
        chorus.setDepth (newValue);
    
    if (parameterID == "CENTREDELAY")
        chorus.setCentreDelay (newValue);
    
    if (parameterID == "FEEDBACK")
        chorus.setFeedback (newValue);
    
    if (parameterID == "MIX")
        chorus.setMix (newValue);
}

void ChorusProcessor::reset()
{
    chorus.reset();
}

// Get name function
const juce::String ChorusProcessor::getName() const
{
    // Gets the name of the juce project... ignore
    return "Chorus";
}

// Accept midi function is always false (audio only)
bool ChorusProcessor::acceptsMidi() const
{
    return false;
}

bool ChorusProcessor::producesMidi() const
{
    return false;
}

// Returns length of the processor tail (don't need)
double ChorusProcessor::getTailLengthSeconds() const
{
    return 0;
}

// We only have one host program, PluginProcessor, return 0
int ChorusProcessor::getNumPrograms()
{
    return 0;
}

// Returns the number of the currently active program. Used with AudioProcessorGraph
int ChorusProcessor::getCurrentProgram()
{
    return 0;
}

// Called by the host to change the current program. Used with AudioProcessorGraph
void ChorusProcessor::setCurrentProgram (int index)
{
}

// Must return the name of a given program. Used with AudioProcessorGraph
const juce::String ChorusProcessor::getProgramName (int index)
{
    return {};
}

// Called by the host to rename a program. Used with AudioProcessorGraph
void ChorusProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================

// Called before playback starts, to let the processor prepare itself.
void ChorusProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // PRE DSP LOGIC HERE
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = getTotalNumOutputChannels();
    
    chorus.prepare (spec);
    chorus.reset();
}

void ChorusProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
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
        
        chorus.process (context);
    } else
    {
        processBlockBypassed(buffer, midiMessages);
    }
}

//juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
//{
//    return new ChorusProcessor();
//}

// Called after playback has stopped, to let the object free up any resources it no longer needs.
void ChorusProcessor::releaseResources()
{
}

// Determines if there will be an editor
bool ChorusProcessor::hasEditor() const
{
//    return true;
    return false;
}

// Creates generic editor window for effect
juce::AudioProcessorEditor* ChorusProcessor::createEditor()
{
//    return new MultiFXAudioProcessorEditor (*this);
    return nullptr;
//    return new juce::GenericAudioProcessorEditor(*this);
}


// Both methods below will save and load presets
void ChorusProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xmlState (state.createXml());
    copyXmlToBinary(*xmlState, destData);
}

void ChorusProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    
    if(xmlState.get() != nullptr)
    {
        if(xmlState->hasTagName(apvts.state.getType()))
        {
            apvts.replaceState(juce::ValueTree::fromXml(*xmlState));
        }
    }
}
