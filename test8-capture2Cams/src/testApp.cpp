#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	
	ofSetWindowShape(1440, 1440/2/4*3);
	
	ofSetLogLevel(OF_LOG_NOTICE);
	vidGrabber[0].setVerbose(true);
	vidGrabber[0].listDevices();
	
	vidGrabber[0].initGrabber(1280, 720, 8);
	vidGrabber[1].initGrabber(1280, 720, 9);

	if (!ofDirectory::doesDirectoryExist("camera1")) {
		ofDirectory::createDirectory("camera1");
	}
	
	if (!ofDirectory::doesDirectoryExist("camera2")) {
		ofDirectory::createDirectory("camera2");
	}
}


//--------------------------------------------------------------
void testApp::update(){
	vidGrabber[0].update();
	vidGrabber[1].update();	
}

//--------------------------------------------------------------
void testApp::draw(){
	vidGrabber[0].draw(ofGetWidth()/2, ofGetHeight(), 0, 0);
	vidGrabber[1].draw(ofGetWidth()/2, ofGetHeight(), ofGetWidth()/2, 0);
}


//--------------------------------------------------------------
void testApp::keyPressed  (int key){ 
	ofSaveImage(vidGrabber[0].getPixelsRef(), "camera1/" + ofToString(ofGetFrameNum()) + ".png");
	ofSaveImage(vidGrabber[1].getPixelsRef(), "camera2/" + ofToString(ofGetFrameNum()) + ".png");
	
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){ 
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
