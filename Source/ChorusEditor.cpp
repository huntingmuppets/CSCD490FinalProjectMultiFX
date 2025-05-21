/*
  ==============================================================================

    ChorusEditor.cpp
    Created: 5 Mar 2024 11:54:52am
    Author:  Grant Howard

  ==============================================================================
*/

// May have to use PluginProcessor.h, check my logic here
//#include "PluginProcessor.h"
#include "ChorusProcessor.h"
#include "ChorusEditor.h"

//==============================================================================
MultiFXChorusEditor::MultiFXChorusEditor (ChorusProcessor& p)
    : audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (300, 400);
    //addAndMakeVisible(audioWindow);
}


MultiFXChorusEditor::~MultiFXChorusEditor()
{
}

//==============================================================================
void MultiFXChorusEditor::paint (juce::Graphics& g)
{
    // Paint function for subcompnent
    // audioWindow->paint(g);
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    g.setColour(juce::Colour(26, 30, 32));
    g.fillRect(0, getHeight() - 40, getWidth(), 40);

}

void MultiFXChorusEditor::resized()
{
    // Resized function for subcomponent
}



