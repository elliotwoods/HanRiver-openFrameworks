#include "testApp.h"

//-------------
testApp::~testApp() {
#pragma omp parallel for
	for (int i=0; i<cameras.size(); i++)
		cameras[i]->close();
}

//-------------
void testApp::setup() {
	ofSetVerticalSync(true);
	gui.init();

	vector<ofxUeyeDevice> devices = ofxUeye::listDevices();
	cameras.resize(devices.size());
	for (int i=0; i<devices.size(); i++) {
		cameras[i] = new ofxUeyeThreaded();
		gui.add(*cameras[i], "Camera " + ofToString(devices[i].cameraID) );
		cameras[i]->init(devices[i]);
		cameras[i]->apply(ofxUeyePreset_5480Chessboard());
	}
}

//-------------
void testApp::update() {
	for (int i=0; i<cameras.size(); i++)
		cameras[i]->update();
}

//-------------
void testApp::draw() {
}