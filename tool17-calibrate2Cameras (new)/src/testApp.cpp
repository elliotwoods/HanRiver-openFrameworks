#include "testApp.h"

using namespace ofxCv;
using namespace cv;

const float diffThreshold = 2.5; // maximum amount of movement
const float timeThreshold = 1; // minimum time between snapshots
const int startCleaning = 10; // start cleaning outliers after this many samples

//-------------
void testApp::setup() {
	ofSetFrameRate(60);

	//cameras
	vector<ofxUeyeDevice> devices = ofxUeye::listDevices();
	vector<ofxUeyeDevice>::iterator it;
	for (it = devices.begin(); it != devices.end(); it++)
		cameras.push_back(ofPtr<CameraHead> (new CameraHead(*it)));

	
	//gui
	gui.init();
	gui.addInstructions();
	vector<ofPtr<CameraHead> >::iterator it2;
	for (it2 = cameras.begin(); it2 != cameras.end(); it2++) {
		gui.add((**it2), string("Camera ") + ofToString((*it2)->getCamera().getCameraID()) + " live");
		gui.add((**it2).getCaptures(), string("Camera ") + ofToString((*it2)->getCamera().getCameraID()) + " captures");
	}

	//sounds
	pop.loadSound("pop-5.wav");
	pop.setVolume(1.0f);
	lastAdd = ofGetElapsedTimef();
}

//-------------
void testApp::update() {
	if (ofGetElapsedTimef() - lastAdd > ADD_FREQUENCY) {
		vector<ofPtr<CameraHead> >::iterator it2;
		for (it2 = cameras.begin(); it2 != cameras.end(); it2++) {
			if ((*it2)->isFound()) {
				pop.setSpeed(1.0f);
				pop.play();
			} else {
				pop.setSpeed(0.6f);
				pop.play();
			}
			ofSleepMillis(300);
		}
		add();
	}
}

//-------------
void testApp::draw() {
}

//-------------
void testApp::keyPressed(int key) {
	if (key == ' ')
		add();
}

//-------------
void testApp::mouseMoved(int x, int y) {
}

//-------------
void testApp::mousePressed(int x, int y, int button) {
	add();
}

//-------------
void testApp::add() {
	vector<ofPtr<CameraHead> >::iterator it;
#pragma omp parallel for
	for (it = cameras.begin(); it != cameras.end(); it++)
		(*it)->add();
	lastAdd = ofGetElapsedTimef();
}