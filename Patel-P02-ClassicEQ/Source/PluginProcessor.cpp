/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PatelP02ClassicEQ::PatelP02ClassicEQ()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    addParameter (LSfreqUParam = new juce::AudioParameterFloat (
                                                                "LSfreq", // parameterID
                                                                "Low Shelf Frequency", // parameter name
                                                                juce::NormalisableRange<float> (33.0f, 270.0f), //parameter range
                                                                100.0f)); // default value
    addParameter (LSGainUParam = new juce::AudioParameterFloat (
                                                                "LSgain", // parameterID
                                                                "Low Shelf Gain", // parameter name
                                                                juce::NormalisableRange<float> (-12.0f, 12.0f), //parameter range
                                                                0.0f)); // default value
    addParameter (B1freqUParam = new juce::AudioParameterFloat (
                                                                "B1freq", // parameterID
                                                                "Low Mids Frequency", // parameter name
                                                                juce::NormalisableRange<float> (40.0f, 1200.0f), //parameter range
                                                                500.0f)); // default value
    addParameter (B1GainUParam = new juce::AudioParameterFloat (
                                                                "B1gain", // parameterID
                                                                "Low Mids Gain", // parameter name
                                                                juce::NormalisableRange<float> (-12.0f, 12.0f), //parameter range
                                                                0.0f)); // default value
    addParameter (Q1UParam = new juce::AudioParameterFloat (
                                                                "Q1", // parameterID
                                                                "Low Mids Q", // parameter name
                                                                juce::NormalisableRange<float> (0.667f, 4.0f), //parameter range
                                                                1.0f)); // default value
    addParameter (B2freqUParam = new juce::AudioParameterFloat (
                                                                "B2freq", // parameterID
                                                                "High Mids Frequency", // parameter name
                                                                juce::NormalisableRange<float> (600.0f, 18000.0f), //parameter range
                                                                2000.0f)); // default value
    addParameter (B2GainUParam = new juce::AudioParameterFloat (
                                                                "B2gain", // parameterID
                                                                "High Mids Gain", // parameter name
                                                                juce::NormalisableRange<float> (-12.0f, 12.0f), //parameter range
                                                                0.0f)); // default value
    addParameter (Q2UParam = new juce::AudioParameterFloat (
                                                                "Q2", // parameterID
                                                                "High Mids Q", // parameter name
                                                                juce::NormalisableRange<float> (0.667f, 4.0f), //parameter range
                                                                1.0f)); // default value
    addParameter (HSfreqUParam = new juce::AudioParameterFloat (
                                                                "HSfreq", // parameterID
                                                                "High Shelf Frequency", // parameter name
                                                                juce::NormalisableRange<float> (3300.0f, 15000.0f), //parameter range
                                                                10000.0f)); // default value
    addParameter (HSGainUParam = new juce::AudioParameterFloat (
                                                                "HSgain", // parameterID
                                                                "High Shelf Gain", // parameter name
                                                                juce::NormalisableRange<float> (-12.0f, 12.0f), //parameter range
                                                                0.0f)); // default value
}

PatelP02ClassicEQ::~PatelP02ClassicEQ()
{
}

//==============================================================================
const juce::String PatelP02ClassicEQ::getName() const
{
    return JucePlugin_Name;
}

bool PatelP02ClassicEQ::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool PatelP02ClassicEQ::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool PatelP02ClassicEQ::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double PatelP02ClassicEQ::getTailLengthSeconds() const
{
    return 0.0;
}

int PatelP02ClassicEQ::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int PatelP02ClassicEQ::getCurrentProgram()
{
    return 0;
}

void PatelP02ClassicEQ::setCurrentProgram (int index)
{
}

const juce::String PatelP02ClassicEQ::getProgramName (int index)
{
    return {};
}

void PatelP02ClassicEQ::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void PatelP02ClassicEQ::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    fs = sampleRate;
    LSfc = LSfreqUParam->get();
    LSGain = LSGainUParam->get();
    HSfc = HSfreqUParam->get();
    HSGain = HSGainUParam->get();
    B1fc = B1freqUParam->get();
    B1Gain = B1GainUParam->get();
    B2fc = B2freqUParam->get();
    B2Gain = B2GainUParam->get();
    Q1 = Q1UParam->get();
    Q2 = Q2UParam->get();
    
    FilterCalc::calcCoeffsLowShelf(LScoeffs, LSfc, LSGain, fs);
    FilterCalc::calcCoeffsHighShelf(HScoeffs, HSfc, HSGain, fs);
    FilterCalc::calcCoeffsPeak(B1coeffs, B1fc, B1Gain, Q1, fs);
    FilterCalc::calcCoeffsPeak(B2coeffs, B2fc, B2Gain, Q2, fs);
    
    LSfilterL.setCoefficients(LScoeffs[0], LScoeffs[1], LScoeffs[2], LScoeffs[3], LScoeffs[4]);
    LSfilterR.setCoefficients(LScoeffs[0], LScoeffs[1], LScoeffs[2], LScoeffs[3], LScoeffs[4]);
    curLSFreqParamVal = LSfc;
    curLSGainParamVal = LSGain;
    
    HSfilterL.setCoefficients(HScoeffs[0], HScoeffs[1], HScoeffs[2], HScoeffs[3], HScoeffs[4]);
    HSfilterR.setCoefficients(HScoeffs[0], HScoeffs[1], HScoeffs[2], HScoeffs[3], HScoeffs[4]);
    curHSFreqParamVal = HSfc;
    curHSGainParamVal = HSGain;
    
    B1filterL.setCoefficients(B1coeffs[0], B1coeffs[1], B1coeffs[2], B1coeffs[3], B1coeffs[4]);
    B1filterR.setCoefficients(B1coeffs[0], B1coeffs[1], B1coeffs[2], B1coeffs[3], B1coeffs[4]);
    curB1FreqParamVal = B1fc;
    curB1GainParamVal = B1Gain;
    curQ1ParamVal = Q1;
    
    B2filterL.setCoefficients(B2coeffs[0], B2coeffs[1], B2coeffs[2], B2coeffs[3], B2coeffs[4]);
    B2filterR.setCoefficients(B2coeffs[0], B2coeffs[1], B2coeffs[2], B2coeffs[3], B2coeffs[4]);
    curB2FreqParamVal = B2fc;
    curB2GainParamVal = B2Gain;
    curQ2ParamVal = Q2;
}

void PatelP02ClassicEQ::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool PatelP02ClassicEQ::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void PatelP02ClassicEQ::calcAlgorithmParams() {
    LSfc = LSfreqUParam->get();
    LSGain = LSGainUParam->get();
    HSfc = HSfreqUParam->get();
    HSGain = HSGainUParam->get();
    B1fc = B1freqUParam->get();
    B1Gain = B1GainUParam->get();
    B2fc = B2freqUParam->get();
    B2Gain = B2GainUParam->get();
    Q1 = Q1UParam->get();
    Q2 = Q2UParam->get();
    
    FilterCalc::calcCoeffsLowShelf(LScoeffs, LSfc, LSGain, fs);
    FilterCalc::calcCoeffsHighShelf(HScoeffs, HSfc, HSGain, fs);
    FilterCalc::calcCoeffsPeak(B1coeffs, B1fc, B1Gain, Q1, fs);
    FilterCalc::calcCoeffsPeak(B2coeffs, B2fc, B2Gain, Q2, fs);
    
    LSfilterL.setCoefficients(LScoeffs[0], LScoeffs[1], LScoeffs[2], LScoeffs[3], LScoeffs[4]);
    LSfilterR.setCoefficients(LScoeffs[0], LScoeffs[1], LScoeffs[2], LScoeffs[3], LScoeffs[4]);
//    curLSFreqParamVal = LSfc;
//    curLSGainParamVal = LSGain;
    
    HSfilterL.setCoefficients(HScoeffs[0], HScoeffs[1], HScoeffs[2], HScoeffs[3], HScoeffs[4]);
    HSfilterR.setCoefficients(HScoeffs[0], HScoeffs[1], HScoeffs[2], HScoeffs[3], HScoeffs[4]);
//    curHSFreqParamVal = HSfc;
//    curHSGainParamVal = HSGain;
    
    B1filterL.setCoefficients(B1coeffs[0], B1coeffs[1], B1coeffs[2], B1coeffs[3], B1coeffs[4]);
    B1filterR.setCoefficients(B1coeffs[0], B1coeffs[1], B1coeffs[2], B1coeffs[3], B1coeffs[4]);
//    curB1FreqParamVal = B1fc;
//    curB1GainParamVal = B1Gain;
//    curQ1ParamVal = Q1;
    
    B2filterL.setCoefficients(B2coeffs[0], B2coeffs[1], B2coeffs[2], B2coeffs[3], B2coeffs[4]);
    B2filterR.setCoefficients(B2coeffs[0], B2coeffs[1], B2coeffs[2], B2coeffs[3], B2coeffs[4]);
//    curB2FreqParamVal = B2fc;
//    curB2GainParamVal = B2Gain;
//    curQ2ParamVal = Q2;
    
}

//linear interpolation.  NEW!~
float PatelP02ClassicEQ::linInterpolation(float start, float end, float fract) {
    return start + ( fract * (end - start)); //a - fract * ( b - a )
}

void PatelP02ClassicEQ::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    calcAlgorithmParams();
    const int numSamps = buffer.getNumSamples();
    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
        
        // ..do something to the data...
        for (int i = 0; i < numSamps; i++) {
            
//            float fract = ((float) i / (float) numSamps); //creates a 0-1 ramp 0-1 across the sample block
//
//            if ((curLSFreqParamVal != prevLSFreqParamVal) || (curLSGainParamVal != prevLSGainParamVal)) {
//                LSfc = linInterpolation(prevLSFreqParamVal, curLSFreqParamVal, fract);
//                LSGain = linInterpolation(prevLSGainParamVal, curLSGainParamVal, fract);
//                FilterCalc::calcCoeffsLowShelf(LScoeffs, LSfc, LSGain, fs);
//                LSfilterL.setCoefficients(LScoeffs[0], LScoeffs[1], LScoeffs[2], LScoeffs[3], LScoeffs[4]);
//                LSfilterR.setCoefficients(LScoeffs[0], LScoeffs[1], LScoeffs[2], LScoeffs[3], LScoeffs[4]);
//            }
//
//            if ((curB1FreqParamVal != prevB1FreqParamVal) || (curB1GainParamVal != prevB1GainParamVal) || (curQ1ParamVal != prevQ1ParamVal)) {
//                B1fc = linInterpolation(prevB1FreqParamVal, curB1FreqParamVal, fract);
//                B1Gain = linInterpolation(prevB1GainParamVal, curB1GainParamVal, fract);
//                Q1 = linInterpolation(prevQ1ParamVal, curQ1ParamVal, fract);
//                FilterCalc::calcCoeffsPeak(B1coeffs, B1fc, B1Gain, Q1, fs);
//                B1filterL.setCoefficients(B1coeffs[0], B1coeffs[1], B1coeffs[2], B1coeffs[3], B1coeffs[4]);
//                B1filterR.setCoefficients(B1coeffs[0], B1coeffs[1], B1coeffs[2], B1coeffs[3], B1coeffs[4]);
//            }
//
//            if ((curB2FreqParamVal != prevB2FreqParamVal) || (curB2GainParamVal != prevB2GainParamVal) || (curQ2ParamVal != prevQ2ParamVal)) {
//                B2fc = linInterpolation(prevB2FreqParamVal, curB2FreqParamVal, fract);
//                B2Gain = linInterpolation(prevB2GainParamVal, curB2GainParamVal, fract);
//                Q2 = linInterpolation(prevQ2ParamVal, curQ2ParamVal, fract);
//                FilterCalc::calcCoeffsPeak(B2coeffs, B2fc, B2Gain, Q2, fs);
//                B2filterL.setCoefficients(B2coeffs[0], B2coeffs[1], B2coeffs[2], B2coeffs[3], B2coeffs[4]);
//                B2filterR.setCoefficients(B2coeffs[0], B2coeffs[1], B2coeffs[2], B2coeffs[3], B2coeffs[4]);
//            }
//
//            if ((curHSFreqParamVal != prevHSFreqParamVal) || (curHSGainParamVal != prevHSGainParamVal)) {
//                HSfc = linInterpolation(prevHSFreqParamVal, curHSFreqParamVal, fract);
//                HSGain = linInterpolation(prevHSGainParamVal, curHSGainParamVal, fract);
//                FilterCalc::calcCoeffsHighShelf(HScoeffs, HSfc, HSGain, fs);
//                HSfilterL.setCoefficients(HScoeffs[0], HScoeffs[1], HScoeffs[2], HScoeffs[3], HScoeffs[4]);
//                HSfilterR.setCoefficients(HScoeffs[0], HScoeffs[1], HScoeffs[2], HScoeffs[3], HScoeffs[4]);
//            }
            
            if (channel == 0) {
                channelData[i] = LSfilterL.tick(channelData[i]);
                channelData[i] = B1filterL.tick(channelData[i]);
                channelData[i] = B2filterL.tick(channelData[i]);
                channelData[i] = HSfilterL.tick(channelData[i]);
            }
            else if (channel == 1) {
                channelData[i] = LSfilterR.tick(channelData[i]);
                channelData[i] = B1filterR.tick(channelData[i]);
                channelData[i] = B2filterR.tick(channelData[i]);
                channelData[i] = HSfilterR.tick(channelData[i]);
            }
        }
    }
//    prevLSFreqParamVal = curLSFreqParamVal;
//    prevLSGainParamVal = curLSGainParamVal;
//
//    prevB1FreqParamVal = curB1FreqParamVal;
//    prevB1GainParamVal = curB1GainParamVal;
//    prevQ1ParamVal = curQ1ParamVal;
//
//    prevB2FreqParamVal = curB2FreqParamVal;
//    prevB2GainParamVal = curB2GainParamVal;
//    prevQ2ParamVal = curQ2ParamVal;
//
//    prevHSFreqParamVal = curHSFreqParamVal;
//    prevHSGainParamVal = curHSGainParamVal;
}

//==============================================================================
bool PatelP02ClassicEQ::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* PatelP02ClassicEQ::createEditor()
{
    return new PatelP02ClassicEQAudioProcessorEditor (*this);
}

//==============================================================================
void PatelP02ClassicEQ::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    std::unique_ptr<juce::XmlElement> xml (new juce::XmlElement ("EQParams"));
    xml->setAttribute ("LSfreq", (double) *LSfreqUParam);
    xml->setAttribute ("LSgain", (double) *LSGainUParam);
    xml->setAttribute ("B1freq", (double) *B1freqUParam);
    xml->setAttribute ("B1gain", (double) *B1GainUParam);
    xml->setAttribute ("Q1", (double) *Q1UParam);
    xml->setAttribute ("B2freq", (double) *B2freqUParam);
    xml->setAttribute ("B2gain", (double) *B2GainUParam);
    xml->setAttribute ("Q2", (double) *Q2UParam);
    xml->setAttribute ("HSfreq", (double) *HSfreqUParam);
    xml->setAttribute ("HSgain", (double) *HSGainUParam);
    
    copyXmlToBinary (*xml, destData);
}

void PatelP02ClassicEQ::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName ("EQParams")) {
            *LSfreqUParam = xmlState->getDoubleAttribute ("LSfreq", 0.0);
            *LSGainUParam = xmlState->getDoubleAttribute ("LSgain", 0.0);
            *B1freqUParam = xmlState->getDoubleAttribute ("B1freq", 0.0);
            *B1GainUParam = xmlState->getDoubleAttribute ("B1gain", 0.0);
            *Q1UParam = xmlState->getDoubleAttribute("Q1", 0.0);
            *B2freqUParam = xmlState->getDoubleAttribute ("B2freq", 0.0);
            *B2GainUParam = xmlState->getDoubleAttribute ("B2gain", 0.0);
            *Q2UParam = xmlState->getDoubleAttribute("Q2", 0.0);
            *HSfreqUParam = xmlState->getDoubleAttribute ("HSfreq", 0.0);
            *HSGainUParam = xmlState->getDoubleAttribute ("HSgain", 0.0);
        }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PatelP02ClassicEQ();
}
