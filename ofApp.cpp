#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    //initalizing sample rate and buffer size
    int sampleRate = 44100;
    int bufferSize= 512;
    //passing the sample rate and buffer size variables into the openFramesworks maxi setup function with 2 output channels
    ofxMaxiSettings::setup(sampleRate, 2, bufferSize);
    //creating a ofsoundStreamsetting object so the program can access the computers sound card and play audio, 2 output channels are being set and 0 inputChannels as the program only needs an audio output
    ofSoundStreamSettings settings;
    settings.setOutListener(this);
    settings.sampleRate = sampleRate;
    settings.numOutputChannels = 2;
    settings.numInputChannels = 0;
    settings.bufferSize = bufferSize;
    soundStream.setup(settings);
    
    //using a for loop to setup an oscilloscope that is less than the width of the screen
    for (int i = 0; i < ofGetWidth(); ++i) {
        waveform[i] = 0;
    }
    //Initializing the oscilloscope index to 0
    waveIndex = 0;
    
    //creating the synthesizer object
    synth = new Synthesizer();

    //calling the synthesizer setup function which contains the necessary variables and functions needed to setup the object, if this function is not called the synthesizer object will break
    synth->setup();
    
    volLeft = 0.0f;
    volRight = 0.0f;
    maxVolLeft = 0.0f;
    maxVolRight = 0.0f;
    synthOutput = 0.0f;
    
    //GUI set up
    gui.setup("Parameters");
    //adding parameters to the GUI, each parameter corresponds to a feature on the synthesizer and the min and max values have been chosen to work well with that feature. The inital values also match the inital values inside the synthesizer object for the given feature.
    gui.add(freqGui.set("Freq",118,20,200));
    gui.add(fModFreqGui.set("FMod Freq",200,100,800));
    gui.add(fModCycleGui.set("FMod Cycle",0.46,0.10,0.50));
    gui.add(fModAmpGui.set("FMod Amp",100,0,200));
    gui.add(lfoFreqGui.set("LFO Freq",0.2,0.1,20));
    gui.add(lfoLengthGui.set("LFO Length",24,2,24));
    gui.add(lfoIndexGui.set("LFO Index",200,10,200));
    gui.add(resCutOffGui.set("Res CutOff",370,300,700));
    gui.add(resAmountGui.set("Res Amount",10,1,30));
    gui.add(delayFeedbackGui.set("Delay Feedback",0.6,0.1,1));
    gui.add(attackGui.set("Attack",1000,10,10000));
    gui.add(decayGui.set("Decay",1,1,10));
    gui.add(sustainGui.set("Sustain",1,0.1,1));
    gui.add(releaseGui.set("Release",100,10,10000));
    gui.add(panningEffect.set("Panning Effect", true));
    gui.add(amplitudeGui.set("Amplitude",1.0,0.10,1.0));

    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    //here values on the systhesizer are being updated to the values that the GUI sliders are outputing by accessing the variable for a given feature inside the systhesizer object and then assigning it to the GUI slider value. For example the systhesizer LFO frequency value changes to the value of the lfoFreqGui variable
    synth->freq = freqGui;
    synth->fModFreq = fModFreqGui;
    synth->fModCycle = fModCycleGui;
    synth->fModAmp = fModAmpGui;
    synth->lfoFreq = lfoFreqGui;
    synth->lfoLength = lfoLengthGui;
    synth->lfoIndex = lfoIndexGui;
    synth->resCutOff = resCutOffGui;
    synth->resAmount = resAmountGui;
    synth->delayFeedback = delayFeedbackGui;
    synth->attack = attackGui;
    synth->decay = decayGui;
    synth->sustain = sustainGui;
    synth->release = releaseGui;
    synth->amp = amplitudeGui;
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground(34, 34, 34);
    //rendering the GUI
    gui.draw();
    //translating the pixels so that the oscilloscope is drawn half way down the screen
    ofTranslate(0, ofGetHeight()/2);
    ofSetColor(138,43,226);
    ofFill();
    //increasing the line width of the oscilloscope
    ofSetLineWidth(2);
    //drawing the oscilloscope
    ofDrawLine(0, 0, 1, waveform[1] * ofGetHeight()/2);
    for(int i = 1; i < (ofGetWidth() - 1); ++i) {
        ofDrawLine(i, waveform[i] * ofGetHeight()/2, i + 1, waveform[i+1] * ofGetHeight()/2);
    }
    
}

//--------------------------------------------------------------
//audio out function for outputting sound to the speakers
void ofApp::audioOut(ofSoundBuffer& output){
    //assigning the variable outChannels to the number of output channels
    std::size_t outChannels = output.getNumChannels();
    
    //looping through the output buffer
    for (int i = 0; i < output.getNumFrames(); i++){
        //calling the synthesizers playSynth function which returns the output as a double and assigning it to the synthOutput variable
        synthOutput = synth->playSynth();
        
        //calling the maxiMix stereo function to turn the output from mono to stereo and passing in the synthesizer output, an array of size two for the outputs channels and for the left and right output channels ratio i am using a sinewave to that goes from 0 to 1 at a rate of twice a second to create an effect that makes the sound switch between the left and right speakers. This only happens if panningEffect is true using a ternary operator, if the panningEffect is false then the sound is outputed evenly between the left and right channels
      (panningEffect == false ? mix.stereo(synthOutput, outputs, 0.5) : mix.stereo(synthOutput, outputs, osc.sinewave(2)+1/2));
        
        //assigning variables for the left and right output channels
        volLeft = outputs[0];
        volRight = outputs[1];
        
        //passing the left and right output channels into the ofClamp function so that the output cannot go over -1 and 1 (prevents clipping) then assigning them to variables
        maxVolLeft = ofClamp(volLeft, -1, 1);
        maxVolRight = ofClamp(volRight, -1, 1);
        
        //assigning the max amplidute left and right variables to the left and right output channels
        output[i * outChannels] = maxVolLeft;
        output[i * outChannels + 1] = maxVolRight;
        
        //holding the output values so they can be used in the draw function
        waveform[waveIndex] =  output[i * outChannels];
        waveIndex = (waveIndex < ofGetWidth() -1 ? waveIndex+1 : 0);
        
    }
}

//--------------------------------------------------------------
ofApp::~ofApp()
{
    //freeing up memory by deleteing the synth object
    delete synth;
    
}
