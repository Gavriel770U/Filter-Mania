#include "ofApp.h"

#include "imageProcessing.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    camWidth 		= 320;
    camHeight 		= 240;
    
    vector<ofVideoDevice> devices = vidGrabber.listDevices();
    
    for(unsigned int i = 0; i < devices.size(); i++){
        cout << devices[i].id << ": " << devices[i].deviceName;
        if( devices[i].bAvailable ){
            cout << endl;
        }else{
            cout << " - unavailable " << endl;
        }
    }
    
    vidGrabber.setDeviceID(0);
    vidGrabber.setDesiredFrameRate(60);
    vidGrabber.setup(camWidth,camHeight);
    
    videoOutputBuffer 	= new unsigned char[camWidth*camHeight*3];
    videoTexture.allocate(camWidth,camHeight, GL_RGB);
    ofSetVerticalSync(true);
}


//--------------------------------------------------------------
void ofApp::update(){
    
    ofBackground(150,150,150);
    
    vidGrabber.update();
    
    if (vidGrabber.isFrameNew()){
        unsigned char * pixels = vidGrabber.getPixels().getData();
        //processNegative(pixels, videoOutputBuffer, camWidth, camHeight);
		//processFilterOne(pixels, videoOutputBuffer, camWidth, camHeight);
		//processFilterTwo(pixels, videoOutputBuffer, camWidth, camHeight);
        //processFilterThree(pixels, videoOutputBuffer, camWidth, camHeight);
        //processFilterFour(pixels, videoOutputBuffer, camWidth, camHeight);
        //processFilterLGBT(pixels, videoOutputBuffer, camWidth, camHeight);
        //processFilterIsrael(pixels, videoOutputBuffer, camWidth, camHeight);
        //processFilterFlip(pixels, videoOutputBuffer, camWidth, camHeight);
        //processFilterPixelation(pixels, videoOutputBuffer, camWidth, camHeight);
        //processFilterWriteWithBlue(pixels, videoOutputBuffer, camWidth, camHeight);
        //processFilterTriangleFlip(pixels, videoOutputBuffer, camWidth, camHeight);
        //processFilterDarkFrame(pixels, videoOutputBuffer, camWidth, camHeight);
        //processFilterWhiteBottomGlow(pixels, videoOutputBuffer, camWidth, camHeight);
        processFilterStickmanWorld(pixels, videoOutputBuffer, camWidth, camHeight);
		videoTexture.loadData(videoOutputBuffer, camWidth, camHeight, GL_RGB);
    }    
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetHexColor(0xffffff);
    vidGrabber.draw(20,20);
    videoTexture.draw(50+camWidth,20,camWidth,camHeight);
}


//--------------------------------------------------------------
void ofApp::keyPressed  (int key){
    
    // in fullscreen mode, on a pc at least, the
    // first time video settings the come up
    // they come up *under* the fullscreen window
    // use alt-tab to navigate to the settings
    // window. we are working on a fix for this...
    
    if (key == 's' || key == 'S'){
        vidGrabber.videoSettings();
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}
