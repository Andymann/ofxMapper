#pragma once

#include "ofMain.h"
#include "ofxPiMapper.h"
#include "ofxMidi.h"
#include "ofxFBOGlslSource.h"
#include "ofxFBOSyphonSource.h"
#include "ofxSyphonServerDirectory.h"

class ofApp : public ofBaseApp, public ofxMidiListener{

    private:
    void enumShaders();
    void enumMidi();
    void registerSyphonServer(string pServername, string pAppname);
    void getSyphonServers();
    
    public:
    void setup();
    void update();
    void draw();
    void exit();
    
    // We need to forward key and mouse events to ofxPiMapper.
    // By not doing it we have the opportunity to use ofxPiMapper
    // witout the interface.
    
    void keyPressed(int key);
    void keyReleased(int key);
    
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    
    // This is our mapper object.
    ofxPiMapper mapper;
    
    void newMidiMessage(ofxMidiMessage& eventArgs);
    
    ofxMidiIn midiIn;
    std::vector<ofxMidiMessage> midiMessages;
    std::size_t maxMessages = 10; //< max number of messages to keep track of
        
    //ofTexture tex;
    ofxSyphonServer syphonServer;
    ofxSyphonServerDirectory syphonList;
    
};
