/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PatelP02ClassicEQAudioProcessorEditor::PatelP02ClassicEQAudioProcessorEditor (PatelP02ClassicEQ& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    
    LSfreq.setRange(33.0, 270.0, 1.0);
    LSfreq.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    LSfreq.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    LSfreq.setTextValueSuffix(" Hz");
    LSfreq.setColour(0x1001300, juce::Colour(252, 186, 3));
    LSfreq.setColour(0x1001400, juce::Colours::whitesmoke);
    LSfreq.addListener(this);
    addAndMakeVisible(LSfreq);
    LSfreqLabel.setText("Low Shelf Freq", juce::dontSendNotification);
    LSfreqLabel.attachToComponent(&LSfreq, true);
    
    LSgain.setRange(-12.0, 12.0, 0.1);
    LSgain.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    LSgain.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    LSgain.setTextValueSuffix(" dB");
    LSgain.setColour(0x1001300, juce::Colour(252, 186, 3));
    LSgain.setColour(0x1001400, juce::Colours::whitesmoke);
    LSgain.addListener(this);
    addAndMakeVisible(LSgain);
    LSgainLabel.setText("Low Shelf Gain", juce::dontSendNotification);
    LSgainLabel.attachToComponent(&LSgain, true);
    
    B1freq.setRange(40.0, 1200.0, 1.0);
    B1freq.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    B1freq.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    B1freq.setTextValueSuffix(" Hz");
    B1freq.setColour(0x1001300, juce::Colour(252, 186, 3));
    B1freq.setColour(0x1001400, juce::Colours::whitesmoke);
    B1freq.addListener(this);
    addAndMakeVisible(B1freq);
    B1freqLabel.setText("Low Mids Freq", juce::dontSendNotification);
    B1freqLabel.attachToComponent(&B1freq, true);
    
    B1gain.setRange(-12.0, 12.0, 0.1);
    B1gain.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    B1gain.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    B1gain.setTextValueSuffix(" dB");
    B1gain.setColour(0x1001300, juce::Colour(252, 186, 3));
    B1gain.setColour(0x1001400, juce::Colours::whitesmoke);
    B1gain.addListener(this);
    addAndMakeVisible(B1gain);
    B1gainLabel.setText("Low Mids Gain", juce::dontSendNotification);
    B1gainLabel.attachToComponent(&B1gain, true);
    
    Q1.setRange(0.667, 4.0, 0.01);
    Q1.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    Q1.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    Q1.setColour(0x1001300, juce::Colour(252, 186, 3));
    Q1.setColour(0x1001400, juce::Colours::whitesmoke);
    Q1.addListener(this);
    addAndMakeVisible(Q1);
    Q1Label.setText("Low Mids Q", juce::dontSendNotification);
    Q1Label.attachToComponent(&Q1, true);
    
    B2freq.setRange(600.0, 18000.0, 1.0);
    B2freq.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    B2freq.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    B2freq.setTextValueSuffix(" Hz");
    B2freq.setColour(0x1001300, juce::Colour(252, 186, 3));
    B2freq.setColour(0x1001400, juce::Colours::whitesmoke);
    B2freq.addListener(this);
    addAndMakeVisible(B2freq);
    B2freqLabel.setText("High Mids Freq", juce::dontSendNotification);
    B2freqLabel.attachToComponent(&B2freq, true);
    
    B2gain.setRange(-12.0, 12.0, 0.1);
    B2gain.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    B2gain.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    B2gain.setTextValueSuffix(" dB");
    B2gain.setColour(0x1001300, juce::Colour(252, 186, 3));
    B2gain.setColour(0x1001400, juce::Colours::whitesmoke);
    B2gain.addListener(this);
    addAndMakeVisible(B2gain);
    B2gainLabel.setText("High Mids Gain", juce::dontSendNotification);
    B2gainLabel.attachToComponent(&B2gain, true);
    
    Q2.setRange(0.667, 4.0, 0.01);
    Q2.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    Q2.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    Q2.setColour(0x1001300, juce::Colour(252, 186, 3));
    Q2.setColour(0x1001400, juce::Colours::whitesmoke);
    Q2.addListener(this);
    addAndMakeVisible(Q2);
    Q2Label.setText("High Mids Q", juce::dontSendNotification);
    Q2Label.attachToComponent(&Q2, true);
    
    HSfreq.setRange(3300.0, 15000.0, 1.0);
    HSfreq.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    HSfreq.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    HSfreq.setTextValueSuffix(" Hz");
    HSfreq.setColour(0x1001300, juce::Colour(252, 186, 3));
    HSfreq.setColour(0x1001400, juce::Colours::whitesmoke);
    HSfreq.addListener(this);
    juce::Component::addAndMakeVisible(HSfreq);
    HSfreqLabel.setText("High Shelf Freq", juce::dontSendNotification);
    HSfreqLabel.attachToComponent(&HSfreq, true);
    
    HSgain.setRange(-12.0, 12.0, 0.1);
    HSgain.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    HSgain.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    HSgain.setTextValueSuffix(" dB");
    HSgain.setColour(0x1001300, juce::Colour(252, 186, 3));
    HSgain.setColour(0x1001400, juce::Colours::whitesmoke);
    HSgain.addListener(this);
    addAndMakeVisible(HSgain);
    HSgainLabel.setText("High Shelf Gain", juce::dontSendNotification);
    HSgainLabel.attachToComponent(&HSgain, true);
    
    
    
    setSize (1000, 350);
    startTimer(100); //right after setSize(), based in ms
}

PatelP02ClassicEQAudioProcessorEditor::~PatelP02ClassicEQAudioProcessorEditor()
{
}

//==============================================================================
void PatelP02ClassicEQAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour(30, 160, 247));
    g.setColour (juce::Colour(46, 55, 61));
    g.setFont (24.0f);
    g.drawFittedText ("Classic EQ", 751, 41, 300, 300, juce::Justification::centred, 1);
    g.setColour (juce::Colour(252, 186, 3));
    g.setFont (24.0f);
    g.drawFittedText ("Classic EQ", 750, 40, 300, 300, juce::Justification::centred, 1);
}

void PatelP02ClassicEQAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    int w = 90; int h = 90;
    
    LSfreq.setBounds(100, 200, w, h);
    LSfreqLabel.setJustificationType(juce::Justification::centred);
    LSfreqLabel.setBounds(100, 180, 100, 30);
    
    LSgain.setBounds(100, 50, w, h);
    LSgainLabel.setJustificationType(juce::Justification::centred);
    LSgainLabel.setBounds(100, 30, 100, 30);
    
    B1gain.setBounds(300, 40, w, h);
    B1gainLabel.setJustificationType(juce::Justification::centred);
    B1gainLabel.setBounds(300, 20, 100, 30);
    
    Q1.setBounds(300, 140, w, h);
    Q1Label.setJustificationType(juce::Justification::centred);
    Q1Label.setBounds(300, 125, 100, 30);
    
    B1freq.setBounds(300, 245, w, h);
    B1freqLabel.setJustificationType(juce::Justification::centred);
    B1freqLabel.setBounds(300, 225, 100, 30);
    
    B2gain.setBounds(500, 40, w, h);
    B2gainLabel.setJustificationType(juce::Justification::centred);
    B2gainLabel.setBounds(500, 20, 100, 30);
    
    Q2.setBounds(500, 140, w, h);
    Q2Label.setJustificationType(juce::Justification::centred);
    Q2Label.setBounds(500, 125, 100, 30);
    
    B2freq.setBounds(500, 245, w, h);
    B2freqLabel.setJustificationType(juce::Justification::centred);
    B2freqLabel.setBounds(500, 225, 100, 30);
    
    HSfreq.setBounds(700, 200, w, h);
    HSfreqLabel.setJustificationType(juce::Justification::centred);
    HSfreqLabel.setBounds(700, 180, 100, 30);
    
    HSgain.setBounds(700, 50, w, h);
    HSgainLabel.setJustificationType(juce::Justification::centred);
    HSgainLabel.setBounds(700, 30, 100, 30);
    
    
}

void PatelP02ClassicEQAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    // make an additional "else if" for each slider
    // &LPfreq is a reference to our Slider* variable
    if (slider == &LSfreq) {
        //notify HOST for plugin automation (touch, latch, etc)
        audioProcessor.LSfreqUParam->beginChangeGesture(); //adds automation capability!
        
        *audioProcessor.LSfreqUParam = (float) LSfreq.getValue();
        
        audioProcessor.LSfreqUParam->endChangeGesture();  //adds automation capability!
    }
    else if (slider == &LSgain) {
        //notify HOST for plugin automation (touch, latch, etc)
        audioProcessor.LSGainUParam->beginChangeGesture(); //adds automation capability!
        
        *audioProcessor.LSGainUParam = (float) LSgain.getValue();
        
        audioProcessor.LSGainUParam->endChangeGesture();  //adds automation capability!
    }
    else if (slider == &B1freq) {
        //notify HOST for plugin automation (touch, latch, etc)
        audioProcessor.B1freqUParam->beginChangeGesture(); //adds automation capability!
        
        *audioProcessor.B1freqUParam = (float) B1freq.getValue();
        
        audioProcessor.B1freqUParam->endChangeGesture();  //adds automation capability!
    }
    else if (slider == &B1gain) {
        //notify HOST for plugin automation (touch, latch, etc)
        audioProcessor.B1GainUParam->beginChangeGesture(); //adds automation capability!
        
        *audioProcessor.B1GainUParam = (float) B1gain.getValue();
        
        audioProcessor.B1GainUParam->endChangeGesture();  //adds automation capability!
    }
    else if (slider == &Q1) {
        //notify HOST for plugin automation (touch, latch, etc)
        audioProcessor.Q1UParam->beginChangeGesture(); //adds automation capability!
        
        *audioProcessor.Q1UParam = (float) Q1.getValue();
        
        audioProcessor.Q1UParam->endChangeGesture();  //adds automation capability!
    }
    else if (slider == &B2freq) {
        //notify HOST for plugin automation (touch, latch, etc)
        audioProcessor.B2freqUParam->beginChangeGesture(); //adds automation capability!
        
        *audioProcessor.B2freqUParam = (float) B2freq.getValue();
        
        audioProcessor.B2freqUParam->endChangeGesture();  //adds automation capability!
    }
    else if (slider == &B2gain) {
        //notify HOST for plugin automation (touch, latch, etc)
        audioProcessor.B2GainUParam->beginChangeGesture(); //adds automation capability!
        
        *audioProcessor.B2GainUParam = (float) B2gain.getValue();
        
        audioProcessor.B2GainUParam->endChangeGesture();  //adds automation capability!
    }
    else if (slider == &Q2) {
        //notify HOST for plugin automation (touch, latch, etc)
        audioProcessor.Q2UParam->beginChangeGesture(); //adds automation capability!
        
        *audioProcessor.Q2UParam = (float) Q2.getValue();
        
        audioProcessor.Q2UParam->endChangeGesture();  //adds automation capability!
    }
    else if (slider == &HSfreq) {
        //notify HOST for plugin automation (touch, latch, etc)
        audioProcessor.HSfreqUParam->beginChangeGesture(); //adds automation capability!
        
        *audioProcessor.HSfreqUParam = (float) HSfreq.getValue();
        
        audioProcessor.HSfreqUParam->endChangeGesture();  //adds automation capability!
    }
    else if (slider == &HSgain) {
        //notify HOST for plugin automation (touch, latch, etc)
        audioProcessor.HSGainUParam->beginChangeGesture(); //adds automation capability!
        
        *audioProcessor.HSGainUParam = (float) HSgain.getValue();
        
        audioProcessor.HSGainUParam->endChangeGesture();  //adds automation capability!
    }
}

void PatelP02ClassicEQAudioProcessorEditor::timerCallback()
{
    //Update the value of each slider to match the value in the Processor
    LSfreq.setValue(*audioProcessor.LSfreqUParam, juce::dontSendNotification);
    LSgain.setValue(*audioProcessor.LSGainUParam, juce::dontSendNotification);
    B1freq.setValue(*audioProcessor.B1freqUParam, juce::dontSendNotification);
    B1gain.setValue(*audioProcessor.B1GainUParam, juce::dontSendNotification);
    Q1.setValue(*audioProcessor.Q1UParam, juce::dontSendNotification);
    B2freq.setValue(*audioProcessor.B2freqUParam, juce::dontSendNotification);
    B2gain.setValue(*audioProcessor.B2GainUParam, juce::dontSendNotification);
    Q2.setValue(*audioProcessor.Q2UParam, juce::dontSendNotification);
    HSfreq.setValue(*audioProcessor.HSfreqUParam, juce::dontSendNotification);
    HSgain.setValue(*audioProcessor.HSGainUParam, juce::dontSendNotification);
    
}
