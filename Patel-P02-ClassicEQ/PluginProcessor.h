/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "FilterCalc.h"
#include "BiQuad.h"

//==============================================================================
/**
*/
class PatelP02ClassicEQ  : public juce::AudioProcessor
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    //==============================================================================
    PatelP02ClassicEQ();
    ~PatelP02ClassicEQ() override;

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
    
    juce::AudioParameterFloat* LSfreqUParam;
    juce::AudioParameterFloat* HSfreqUParam;
    juce::AudioParameterFloat* B1freqUParam;
    juce::AudioParameterFloat* B2freqUParam;
    juce::AudioParameterFloat* LSGainUParam;
    juce::AudioParameterFloat* HSGainUParam;
    juce::AudioParameterFloat* B1GainUParam;
    juce::AudioParameterFloat* B2GainUParam;
    juce::AudioParameterFloat* Q1UParam;
    juce::AudioParameterFloat* Q2UParam;
    juce::AudioParameterBool* toggleActive;
    // BPM vars. we need these public if we communicate with the PluginEditor.
    

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PatelP02ClassicEQ)
    float fs, LSfc, HSfc, B1fc, B2fc, LSGain, HSGain, B1Gain, B2Gain, Q1, Q2;
    float LScoeffs[5];
    float HScoeffs[5];
    float B1coeffs[5];
    float B2coeffs[5];
    stk::BiQuad LSfilterL, LSfilterR, HSfilterL, HSfilterR, B1filterL, B1filterR, B2filterL, B2filterR;
    float prevLSFreqParamVal, curLSFreqParamVal, prevHSFreqParamVal, curHSFreqParamVal, prevB1FreqParamVal, curB1FreqParamVal, prevB2FreqParamVal, curB2FreqParamVal;
    float prevLSGainParamVal, curLSGainParamVal, prevHSGainParamVal, curHSGainParamVal, prevB1GainParamVal, curB1GainParamVal, prevB2GainParamVal, curB2GainParamVal;
    float prevQ1ParamVal, curQ1ParamVal, prevQ2ParamVal, curQ2ParamVal;
    float linInterpolation(float start, float end, float fract); //linear interpolation
    void calcAlgorithmParams();
};
