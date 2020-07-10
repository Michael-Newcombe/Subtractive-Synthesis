#pragma once

#include "ofMain.h"
#include "ofxMaxim.h"
#include "synthesizer.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
        //destructor
        ~ofApp();
    
        //openframeworks audioOut function and openframeworks audio setup object
        void audioOut(ofSoundBuffer& output);
        ofSoundStream soundStream;
    
        //maxi objects
        maxiOsc osc;
        maxiMix mix;
    
        //variables for right and left channels
        double volLeft;
        double volRight;
        double maxVolLeft;
        double maxVolRight;
    
        //oscilloscope variables
        double waveform[8096];
        int waveIndex;

        //number of output channels
        double outputs[2];
    
        //synth objects pointer
        Synthesizer *synth;
        //synth output variable
        double synthOutput;
    
        //GUI objects
        //GUI panel
        ofxPanel gui;
        //GUI parameters
        //ofParameterGroup parameters
        ofParameter<float> freqGui;
        ofParameter<float> fModFreqGui;
        ofParameter<float> fModCycleGui;
        ofParameter<float> fModAmpGui;
        ofParameter<float> lfoFreqGui;
        ofParameter<int> lfoLengthGui;
        ofParameter<int> lfoIndexGui;
        ofParameter<double> resCutOffGui;
        ofParameter<double> resAmountGui;
        ofParameter<double> delayFeedbackGui;
    
        ofParameter<double> attackGui;
        ofParameter<double> decayGui;
        ofParameter<double> sustainGui;
        ofParameter<double> releaseGui;
        ofParameter<bool> panningEffect;
        ofParameter<double> amplitudeGui;



};
