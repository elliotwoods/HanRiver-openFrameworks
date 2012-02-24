#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	gui.init();

	ofSetLogLevel(OF_LOG_NOTICE);
	for (int i=0; i<NCAMERAS; i++) {
		ofPtr<ofxUeyeThreaded> camera(new ofxUeyeThreaded());
		cameras.push_back(camera);
		cameras.back()->init(i+1, true);
		gui.add(*cameras[i], "Camera " + ofToString(i));
	}
}

//--------------------------------------------------------------
void testApp::update(){
	for (int i=0; i<NCAMERAS; i++)
		cameras[i]->update();
}

//--------------------------------------------------------------

void testApp::draw(){

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
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