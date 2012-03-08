#include "testApp.h"

//-------------
void testApp::setup() {
	boardFinder.setSquareSize(0.04); //meters
	boardFinder.setPatternSize(9, 6); //black backed chessboard

	gui.init();
	gui.addInstructions();

//instantiate in sequence
	vector<ofxUeyeDevice> devices = ofxUeye::listDevices();
	for (int i=0; i<devices.size(); i++) {
		cameras.push_back(CameraHead());
		cameraMap.insert( pair<int, CameraHead*>( devices[i].cameraID, &cameras.back() ) );
		gui.add(cameras.back(), "Camera " + ofToString(devices[i].cameraID) );
	}

//initialise in parallel
#pragma omp parallel for
	for (int i=0; i<devices.size(); i++)
		cameras[i].init(devices[i]);

	this->captureID = 0;
}

//-------------
void testApp::update() {
}

//-------------
void testApp::draw() {
}

//-------------c
void testApp::keyPressed(int key) {
	if (key == ' ')
		addFrame();
	if (key == 'i')
		solveIntrinsics();
	if (key == 'e')
		solveExtrinsics();
	if (key == 's')
		solveAllAndSave();
}

//-------------
void testApp::mouseMoved(int x, int y) {
}

//-------------
void testApp::mousePressed(int x, int y, int button) {

}

//-------------
void testApp::addFrame() {
//#pragma omp parallel for
	for (int i=0; i<cameras.size(); i++)
		cameras[i].capture(captureID);
	captureID++;
}

//-------------
void testApp::solveIntrinsics() {
#pragma omp parallel for
	for (int i=0; i<cameras.size(); i++)
		cameras[i].solveIntrinsics();
}

//-------------
void testApp::solveExtrinsics() {
	if (cameras.size() < 2) {
		ofLogError() << "cannot solve extrsinics for less than 2 cameras";
		return;
	}

	for (int i=0; i<cameras.size(); i++) {
		if (!cameras[i].getHasIntrinsics()) {
			ofLogError() << "Cannot solve extrsinsics as 1 or more cameras does not have intrinsics (starting with camera " << cameras[i].getCameraID() << ")";
			return;
		}
	}

	ExtrinsicsFinder extrsinics;
	extrsinics.solve(this->cameraMap, this->cameras);
}

//-------------
void testApp::solveAllAndSave() {
	this->solveIntrinsics();
	this->solveExtrinsics();
}