/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
//#include "MainComponent.h"
//#include ChorusEditor.h

//==============================================================================
ActiveComponent::ActiveComponent()
{
    
}
ActiveComponent::~ActiveComponent()
{

}

void ActiveComponent::paint(juce::Graphics&)
{

}
void ActiveComponent::resized()
{
  if (mainEditor != nullptr)
  {
    addAndMakeVisible(mainEditor);
    mainEditor->setBounds(getLocalBounds());
  }
}
MultiFXAudioProcessorEditor::MultiFXAudioProcessorEditor (MultiFXAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (1200, 800);
    //addAndMakeVisible(audioWindow);
     //active effects window
    activeEffects.setColour(juce::ListBox::backgroundColourId, juce::Colour(36, 44, 50));
    activeEffects.setOutlineThickness(3);
    activeEffects.setColour(juce::ListBox::outlineColourId, juce::Colour(26, 30, 32));
    addAndMakeVisible(activeEffects);

    reverb.setColour(juce::TextButton::buttonColourId, getLookAndFeel().findColour(juce::TextButton::buttonColourId));
    chorus.setColour(juce::TextButton::buttonColourId, getLookAndFeel().findColour(juce::TextButton::buttonColourId));
    muteChorus.setColour(juce::TextButton::buttonColourId, getLookAndFeel().findColour(juce::TextButton::buttonColourId));
    muteReverb.setColour(juce::TextButton::buttonColourId, getLookAndFeel().findColour(juce::TextButton::buttonColourId));
    reverb.setButtonText("Compressor");
    reverb.addListener(this);
    chorus.setButtonText("Chorus");
    chorus.addListener(this);
    muteReverb.setButtonText("Remove");
    muteReverb.addListener(this);
    muteChorus.setButtonText("Remove");
    muteChorus.addListener(this);
    //muteChorus.setImages(false,true,true,unmuted,1.0,juce::Colours::white,unmuted,1.0, juce::Colours::white,muted,1.0, juce::Colours::white,0);
    //muteReverb.setImages(false, true, true, unmuted, 1.0, juce::Colours::white, unmuted, 1.0, juce::Colours::white, muted, 1.0, juce::Colours::white, 0);
    addAndMakeVisible(reverb);
    addAndMakeVisible(chorus);
    addAndMakeVisible(muteChorus);
    addAndMakeVisible(muteReverb);


    //footer buttons and list
    save.setColour(juce::TextButton::buttonColourId, getLookAndFeel().findColour(juce::TextButton::buttonColourId));
    load.setColour(juce::TextButton::buttonColourId, getLookAndFeel().findColour(juce::TextButton::buttonColourId));
    save.setButtonText("Save");
    load.setButtonText("Load");
    addAndMakeVisible(save);
    addAndMakeVisible(load);


    //effect window
    effectLibrary.setColour(juce::ListBox::backgroundColourId, juce::Colour(36, 44, 50));
    effectLibrary.setOutlineThickness(3);
    effectLibrary.setColour(juce::ListBox::outlineColourId, juce::Colour(26, 30, 32));
    addAndMakeVisible(effectLibrary);

    addAndMakeVisible(audioWindow);

    
}

void MultiFXAudioProcessorEditor::buttonClicked(juce::Button* button) {
    if(button == &chorus)
    {
        DBG("Chorus Selected\n");
        
        // Replace 0 with your own global index for updateGraph
        // PluginProcessor.h has the statement: using Node = juce::AudioProcessorGraph::Node;
        // See PluginProcessor.h for Node::Ptr array declaration (std::array<Node::Ptr, 2> NameOfArray)
        // Function returns Node::Ptr object or nullptr
        // If functions returns a nullptr, do nothing
        // Let me know if you have any problems
        audioProcessor.updateGraph(0, "CHORUS");

        audioWindow.mainEditor = new juce::GenericAudioProcessorEditor(audioProcessor.nodeID_Array[0]->getProcessor());
        chorus.setColour(juce::TextButton::buttonColourId, juce::Colours::green);
        reverb.setColour(juce::TextButton::buttonColourId, getLookAndFeel().findColour(juce::TextButton::buttonColourId));

        muteChorus.setColour(juce::TextButton::buttonColourId, juce::Colours::red);
        audioWindow.resized();

        audioWindow.resized();
        // Get chorusProcessor
        // Get chorus NodeID
        // Pass processor to subcompnent and get its reference
    } else if(button == &reverb)
    {
        DBG("Reverb Selected\n");
        reverb.setColour(juce::TextButton::buttonColourId, juce::Colours::green);
        chorus.setColour(juce::TextButton::buttonColourId, getLookAndFeel().findColour(juce::TextButton::buttonColourId));
        muteReverb.setColour(juce::TextButton::buttonColourId, juce::Colours::red);
        audioProcessor.updateGraph(1, "COMPRESSOR");

        audioWindow.mainEditor = new juce::GenericAudioProcessorEditor(audioProcessor.nodeID_Array[1]->getProcessor());
        audioWindow.resized();

    }
  else if (button == &muteChorus)
  {
    if (audioProcessor.nodeID_Array[0] != nullptr /*&& audioWindow.mainEditor->getAudioProcessor() == audioProcessor.nodeID_Array[0]->getProcessor()*/)
    {
      audioWindow.removeChildComponent(audioWindow.mainEditor);
      audioWindow.mainEditor = nullptr;
      audioWindow.resized();
      audioProcessor.removeNode(0);
      muteChorus.setColour(juce::TextButton::buttonColourId, getLookAndFeel().findColour(juce::TextButton::buttonColourId));
      chorus.setColour(juce::TextButton::buttonColourId, getLookAndFeel().findColour(juce::TextButton::buttonColourId));
    }
    else
    {
        audioProcessor.removeNode(0);
        muteChorus.setColour(juce::TextButton::buttonColourId, getLookAndFeel().findColour(juce::TextButton::buttonColourId));
    }
  }
  else if (button == &muteReverb)
  {
    if (audioProcessor.nodeID_Array[1] != nullptr /*&& audioWindow.mainEditor->getAudioProcessor() == audioProcessor.nodeID_Array[1]->getProcessor()*/)
    {
        audioWindow.removeChildComponent(audioWindow.mainEditor);
        audioWindow.mainEditor = nullptr;
        audioWindow.resized();
        audioProcessor.removeNode(1);
        muteReverb.setColour(juce::TextButton::buttonColourId, getLookAndFeel().findColour(juce::TextButton::buttonColourId));
        reverb.setColour(juce::TextButton::buttonColourId, getLookAndFeel().findColour(juce::TextButton::buttonColourId));
    }
    else
    {
        audioProcessor.removeNode(1);
        muteReverb.setColour(juce::TextButton::buttonColourId, getLookAndFeel().findColour(juce::TextButton::buttonColourId));
    }
  }
}


MultiFXAudioProcessorEditor::~MultiFXAudioProcessorEditor()
{
    reverb.removeListener(this);
    chorus.removeListener(this);
    muteReverb.removeListener(this);
    muteChorus.removeListener(this);
}

//==============================================================================
void MultiFXAudioProcessorEditor::paint (juce::Graphics& g)
{
    //audioWindow->paint(g);
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    g.setColour(juce::Colour(26, 30, 32));
    g.fillRect(0, getHeight() - 40, getWidth(), 40);

}

void MultiFXAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    //audioWindow->resized();
    auto area = getLocalBounds();

    auto footerHeight = 36;
    auto sideItemHeight = 40;
    auto sideItemMargin = 5;

    auto sideBarArea = area.removeFromLeft(juce::jmax(80, area.getWidth() / 4));
    auto activeArea = sideBarArea.removeFromTop(area.getHeight() / 2);
    activeEffects.setBounds(activeArea);

    effectLibrary.setBounds(sideBarArea.removeFromBottom(activeArea.getHeight()).removeFromTop(activeArea.getHeight() - footerHeight - 4));




    auto sideButtonArea = activeArea.removeFromLeft(activeArea.getWidth() / 1.5);
    auto muteButtonArea = activeArea.removeFromLeft(activeArea.getWidth() / 1.2);
    reverb.setBounds(sideButtonArea.removeFromBottom(sideItemHeight).reduced(sideItemMargin));
    muteReverb.setBounds(muteButtonArea.removeFromBottom(sideItemHeight).reduced(sideItemMargin));
    chorus.setBounds(sideButtonArea.removeFromBottom(sideItemHeight).reduced(sideItemMargin));
    muteChorus.setBounds(muteButtonArea.removeFromBottom(sideItemHeight).reduced(sideItemMargin));


    auto effectArea = area.removeFromTop(getHeight() - footerHeight);
    auto footerArea = area.removeFromBottom(getHeight() - footerHeight);

    audioWindow.setBounds(effectArea);

    save.setBounds(footerArea.removeFromLeft(getWidth() / 10).reduced(sideItemMargin));
    load.setBounds(footerArea.removeFromLeft(getWidth() / 10).reduced(sideItemMargin));
}



