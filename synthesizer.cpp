#include "synthesizer.h"

//function for setting up the synthesizer
void Synthesizer::setup()
{
    //initializing frequency for sinewave osc
    freq = 118.0f;
    //initializing amplitude
    amp = 1.0f;
    //initializing number of oscillators for additive synthesis
    numOfOscs = 22;
    //initializing frequency modulation value for pulse wave
    fModFreq = 200;
    //initalizing duty cycle value for pulse wave
    fModCycle = 0.46f;
    //initializing frequency modulation amp value
    fModAmp = 100;
    
    //initializing frequency for LFO
    lfoFreq = 0.2f;
    //initializing phasor end phase value for LFO
    lfoLength = 24;
    //initializing phasor amp value
    lfoIndex = 200.0f;
    
    //initalizing resonance filter cutoff value
    resCutOff = 370.0f;
    //initializing resonance amount value
    resAmount = 10.0f;
    
    //initializing variables for the ADSR envelope
    attack = 1000;
    decay = 1;
    sustain = 1;
    release = 100;
    
    //initializing delay feedback value
    delayFeedback = 0.6f;
    
    //initializing counting variables
    adsrCount = 0;
    freqCount = 0;
    
    //initializing output variables 
    adsrOutput = 0.0f;
    fModOutput = 0.0f;
    lfoOutput = 0.0f;
    oscOutput = 0.0f;
    oscMix = 0.0f;
    subtractiveOutput = 0.0f;
    adsrAmpOutput = 0.0f;
    delayOutput = 0.0f;
    finalSound = 0.0f;
    
    //a for loop the size of the numOfOscs variable which is used to create multiple detune values and multiple MaxiOsc objects
    for(int i =0; i < numOfOscs; i++){
        
        //initalizing a detune value using openFrameworks random number function with a range between -10.0 and 10.0
        double detuneValue = ofRandom(-10.0, 10.0);
        //using a vector to create random values which are used to create slight variations in frequency for each oscillator to make the sound more thicker and dynamic
        detune.push_back(ofRandom(detuneValue));
        //using a vector to create multiple maxiOsc objects which are used to create additive synthesis
        oscillators.push_back(new maxiOsc());

    }

    //initializing envelope ADSR values
    envelope.setAttack(attack);
    envelope.setDecay(decay);
    envelope.setSustain(sustain);
    envelope.setRelease(release);

}

//this function creates the sound and returns the value as a double so that it can be passed into the audioOut function inside ofApp.cpp
double Synthesizer::playSynth()
{
    //assigning a phasor to a variable called adsrCount which is used control how often the ADSR trigger happens
    adsrCount = (int)phasor.phasor(0.1,1,9);
//        adsrCount = (int)phasor.phasor(1);


    //assigning a phasor to the freqCount variable, this phasor is used to sequence through the freqs array which contains different frequency for the sqaure wave oscilator
    freqCount = phasor2.phasor(0.3,1,9);

    //if statement that checks if adsrCount does not equal 0
    if (adsrCount!=0) {
    
        //if the stament above is true this means adsrCount has reached 1 therefore the envelope is triggered
        envelope.trigger=1;

    }

    //assigning the adsrOutput variable to the envelope object with a value of 1 so it can be reused
    adsrOutput = envelope.adsr(1.0,envelope.trigger);
    //creating frequency modulation using a maxiOsc sinebuff object, the frequency of the sinebuff is a pulse wave and it's frequency is being multiplied by the ADSR envelope. Eveything is then being mutiplied by a modulation index
    fModOutput = fMod.sinebuf(osc1.pulse(fModFreq * adsrOutput, fModCycle)) * fModAmp;
    //creating a LFO using another maxiOsc sinebuff object which has a phasor with a start and end phase as it's frequency, which is then being multiplied by a modulation index;
    lfoOutput = lfo.sinebuf(phasor3.phasor(lfoFreq, 1, lfoLength)) * lfoIndex;
    //for loop for additive synthesis which is the same size as the for loop in setup that creates the multiple maxiOsc objects
    for(int j = 0; j < numOfOscs; j++){
        //creating additive synthesis and applying it a the ocsOutput variable by looping through the maxiOsc objects, firstly multiple square waves are being added together and their frequencies are an array of different frequencies which get sequenced through using a phasor. The frequencies are then being modulated by adding the fModOutput variable to them, finally the frequencies are being detuned using the detune variable. A sinewave is then being added to the sqaure wave, which contains the same features as the sqaure wave exepct the frequency is not an arrray of frequencies and low pass cufoff is set to 1 which still has an effect on the sound.
     
        oscOutput += oscillators[j]->square(porta.lopass(freqs[freqCount] + (j * freqs[freqCount]) + fModOutput + detune[j], 0.00001)) + oscillators[j]->sinewave(porta.lopass(freq + (j * freq) + fModOutput + detune[j],1));
    }

    //the output result of the additive synthesis is then being divided by the number of oscillators that were added together to prevent distortion.
    oscMix = oscOutput /= numOfOscs;

    //creating subtractive synthesis by passing the output of the oscillators into a maxi low resonance filter and then mutipling everything by an amplifier. The cufoff frequency of the filter is also being modulated with a lfo through the lfoOutput variable then multiplied with the ADSR envelope.
    subtractiveOutput = filter.lores(oscMix, resCutOff + lfoOutput * adsrOutput, resAmount) * amp;

    //multiplying the subractive synthesis output with the ADSR envelope so that it is applied to the amplidute of the sound
    adsrAmpOutput = subtractiveOutput * adsrOutput;
    
    //applying delay to the output using a maxiDelayLine object and multiplying the delay by 0.50 to reduce the amplidute of the delayed signal
    delayOutput = adsrAmpOutput + (delay.dl(adsrAmpOutput, 22000, delayFeedback)*0.50);

    //applying a soft low pass filter to the final sound to cutoff any very high frequencies
    finalSound = finalFilter.lopass(delayOutput, 0.9);
    
    //returning the sound
    return finalSound;

}

Synthesizer::~Synthesizer()
{
    //freeing up memory by deleteing the oscillators objects
    for (maxiOsc *o : oscillators){
        
        delete o;
    }
}
