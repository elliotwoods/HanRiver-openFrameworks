#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	gui.init();

	vector<ofxUeyeDevice> devices = ofxUeye::listDevices();

	ofSetLogLevel(OF_LOG_NOTICE);
	for (int i=0; i<devices.size(); i++) {
		ofPtr<ofxUeyeThreaded> camera(new ofxUeyeThreaded());
		cameras.push_back(camera);
		cameras.back()->init(i);
		gui.add(*cameras[i], "Camera " + ofToString(cameras[i]->getCamera().getCameraID()));
	}
}

//--------------------------------------------------------------
void testApp::update(){
	for (int i=0; i<cameras.size(); i++)
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