/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class MultiFXAudioProcessor  : public juce::AudioProcessor
{
public:
    using AudioGraphIOProcessor = juce::AudioProcessorGraph::AudioGraphIOProcessor;
    using Node = juce::AudioProcessorGraph::Node;
    //==============================================================================
    MultiFXAudioProcessor();
    ~MultiFXAudioProcessor() override;
    
    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    
#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
#endif
    
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    
    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;
    
    //==============================================================================
    const juce::String getName() const override;
    
    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
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
    
    // Returns the ChorusProcessor NodeID
    Node::Ptr getChorus();
    Node::Ptr getCompressor();
    Node::Ptr updateGraph(int slotIndex, const juce::String& effect);

    std::unique_ptr<juce::AudioProcessorGraph> mainProcessor;
    Node::Ptr slot1Node;
    Node::Ptr slot2Node;

    std::array<Node::Ptr, 2> nodeID_Array;
    
    void removeNode(int index);
    
private:
    void initialiseGraph();
    void connectAudioNodes();
//    void updateGraph(int slotIndex);
    
//    juce::StringArray processorChoices {"Empty", "Chorus", "Reverb"};
    
    juce::AudioParameterChoice* processorSlot1;
    juce::AudioParameterChoice* processorSlot2;
    
    
    
    Node::Ptr chorusNode;
    
    Node::Ptr audioInputNode;
    Node::Ptr audioOutputNode;
    Node::Ptr midiInputNode;
    Node::Ptr midiOutputNode;
    
//    juce::ReferenceCountedArray<Node> nodeList = new juce::ReferenceCountedArray<Node>();
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MultiFXAudioProcessor)
};
