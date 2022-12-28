/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class PatelP02ClassicEQAudioProcessorEditor  : public juce::AudioProcessorEditor,
                                            public juce::Slider::Listener,
                                            public juce::Timer
{
public:
    PatelP02ClassicEQAudioProcessorEditor (PatelP02ClassicEQ&);
    ~PatelP02ClassicEQAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void sliderValueChanged(juce::Slider*) override;
    void timerCallback() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    PatelP02ClassicEQ& audioProcessor;
    
    juce::Slider LSfreq;
    juce::Label LSfreqLabel;
    
    juce::Slider HSfreq;
    juce::Label HSfreqLabel;
    
    juce::Slider B1freq;
    juce::Label B1freqLabel;
    
    juce::Slider B2freq;
    juce::Label B2freqLabel;
    
    juce::Slider LSgain;
    juce::Label LSgainLabel;
    
    juce::Slider HSgain;
    juce::Label HSgainLabel;
    
    juce::Slider B1gain;
    juce::Label B1gainLabel;
    
    juce::Slider B2gain;
    juce::Label B2gainLabel;
    
    juce::Slider Q1;
    juce::Label Q1Label;
    
    juce::Slider Q2;
    juce::Label Q2Label;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PatelP02ClassicEQAudioProcessorEditor)
};
