#include "ofApp.h"
bool bNewMidi = false;

bool bKey1 = false;
bool bKey2 = false;
bool bFullScreen = false;
//bool bIsMac = false;
//GlslSource *glsl;

void ofApp::setup(){
    ofBackground(0);
    ofSetFrameRate(30);
    
    enumShaders();
    
    #ifdef TARGET_OSX
        //bIsMac = true;
        syphonServer.setName("ofxPiMapperANDY");
        getSyphonServers();
    #endif
    mapper.setup();
    enumMidi();

    #ifdef TARGET_RASPBERRY_PI
        ofSetFullscreen(true);
    #endif
    
    
}

void ofApp::enumMidi(){
    // print input ports to console
    midiIn.listInPorts();
    
    // open port by number (you may need to change this)
    
    // letzten Port oeffnen
    midiIn.openPort( midiIn.getInPortList().size()-1 );
    //midiIn.openPort("IAC Pure Data In");    // by name
    //midiIn.openVirtualPort("ofxMidiIn Input"); // open a virtual port
    
    // don't ignore sysex, timing, & active sense messages,
    // these are ignored by default
    midiIn.ignoreTypes(false, false, false);
    
    // add ofApp as a listener
    midiIn.addListener(this);
    
    // print received messages to the console
    midiIn.setVerbose(true);
}


/*
    Get other available Syphon-Servers.
 */
void ofApp::getSyphonServers(){
    syphonList.setup();
    vector<ofxSyphonServerDescription> lst = syphonList.getServerList();
    //ofLogNotice("Syphon Server count:" + ofToString(lst.size()) );
    for(int i=0; i<lst.size(); i++){
       ofLogNotice(lst.at(i).appName + " " + lst.at(i).serverName);
       registerSyphonServer( lst.at(i).serverName, lst.at(i).appName );
    }
}

/*
 Every available Syphon-Server is registered as a selectable Input
 */
void ofApp::registerSyphonServer(string pServername, string pAppname){
    ofLogNotice("enumSyphon" +  pServername);
    ofxFBOSyphonSource* syphon = new ofxFBOSyphonSource();
    syphon->init( pServername, pAppname );
    mapper.registerFboSource( syphon );
    
}

// Shader einlesen und automatisch erzeugen
void ofApp::enumShaders(){
    //string path = "../../../data/sources/shaders/";
    string path = ofToDataPath("sources/shaders/", true);
    ofDirectory dir(path);
    
    //only show .frag files
    dir.allowExt("frag");
    
    //populate the directory object
    dir.listDir();
    
    
     for(int i = 0; i < dir.size(); i++){
         ofLogNotice(dir.getPath(i));
     }
    ofxFBOGlslSource* array = new ofxFBOGlslSource[dir.size()];
    
    for (int i = 0; i < dir.size(); ++i)
    {
        string tmp = dir.getName(i).substr(0, dir.getName(i).find(".frag") );
        string tmp2 = dir.getPath(i).substr(0, dir.getPath(i).find(".frag") );
        array[i].loadShader(tmp, tmp2);
        mapper.registerFboSource( array[i] );
    }
}

void ofApp::update(){
    mapper.update();
    
    if(bNewMidi){
        bNewMidi=false;
        mapper.setNextSource();
    }
    
    if(bKey1 && bKey2){
        mapper.saveProject();
        mapper.reboot();
    }
    
}

void ofApp::draw(){
    mapper.draw();
    //if( bIsMac ){
    //    syphonServer.publishScreen();
    //}
    #ifdef TARGET_OSX
        syphonServer.publishScreen();
    #endif
        //individualTextureSyphonServer.publishTexture( ((GlslSource::FboSource*)mapper.getSurfaceManager()->getSurface(0)));
    //ofFbo fbo =
}

void ofApp::keyPressed(int key){
    
    if(key=='u'){
        mapper.togglePause();
    }
    
    if(key=='f'){
        bFullScreen = !bFullScreen;
        ofSetFullscreen(bFullScreen);
    }
    mapper.keyPressed(key);
    
     //ofLogNotice(ofToString(key));
}

void ofApp::keyReleased(int key){
    mapper.keyReleased(key);
}

void ofApp::mouseDragged(int x, int y, int button){
    mapper.mouseDragged(x, y, button);
}

void ofApp::mousePressed(int x, int y, int button){
    mapper.mousePressed(x, y, button);
}

void ofApp::mouseReleased(int x, int y, int button){
    mapper.mouseReleased(x, y, button);
}

//--------------------------------------------------------------
void ofApp::exit() {
    
    // clean up
    
    midiIn.closePort();
    midiIn.removeListener(this);
    
}

//--------------------------------------------------------------
void ofApp::newMidiMessage(ofxMidiMessage& msg) {
    
    // add the latest message to the message queue
    midiMessages.push_back(msg);
    
    // remove any old messages if we have too many
    while(midiMessages.size() > maxMessages) {
        midiMessages.erase(midiMessages.begin());
    }
    
    ofLogNotice("Midi Pitch:" + ofToString( msg.pitch ) );
    
    if(msg.velocity>63){
        bNewMidi = true;
        if(msg.pitch==1)
            bKey1 = true;
        
        if(msg.pitch==2)
            bKey2=true;
    }else{
        if(msg.pitch==1)
            bKey1 = false;
        
        if(msg.pitch==2)
            bKey2=false;
    }
    
}
