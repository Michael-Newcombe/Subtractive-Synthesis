#include "ofMain.h"
#include "ofxMaxim.h"

#ifndef synthesizer_h
#define synthesizer_h

class Synthesizer
{
public:
    
    //synth functions
    void setup();
    double playSynth();
    //destructor
    ~Synthesizer();
    
    //synth objects
    maxiOsc osc1;
    maxiOsc fMod;
    maxiOsc lfo;
    maxiOsc phasor;
    maxiOsc phasor2;
    maxiOsc phasor3;
    vector <maxiOsc*> oscillators;
    vector <double> detune;
    maxiFilter filter;
    maxiFilter porta;
    maxiFilter finalFilter;
    maxiEnv envelope;
    maxiDelayline delay;
    
    //synth variables
    double freq;
    double amp;
    
    double fModFreq;
    double fModCycle;
    double fModAmp;
    double lfoFreq;
    int lfoLength;
    double lfoIndex;
    double resCutOff;
    double resAmount;
    double delayFeedback;
    double attack;
    double decay;
    double sustain;
    double release;
    
    double oscMix;
    
    double oscOutput;
    double lfoOutput;
    double fModOutput;
    double adsrOutput;
    double delayOutput;
    double mixOutput;
    double finalSound;
    double subtractiveOutput;
    double adsrAmpOutput;
    
    int adsrCount;
    int freqCount;
    int lastCount;
    int numOfOscs;
    
    int freqs[8] = {100,200,300,400,500,600,700,800};
    
    
};

#endif /* synthesizer_h */

