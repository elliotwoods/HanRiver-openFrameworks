#include "testApp.h"

using namespace ofxCv;
using namespace cv;

const float diffThreshold = 2.5; // maximum amount of movement
const float timeThreshold = 1; // minimum time between snapshots
const int startCleaning = 10; // start cleaning outliers after this many samples


void testApp::setup() {
	ofSetVerticalSync(true);
	selecting = true;
	devices = ofxUeye::listDevices();
	ofBackground(100);

	this->frameDifference = 0;
	this->calmFrameDifferenceThreshold = 1.0f;
	this->calmDurationThreshold = 2.0f;
	this->lastMovement = 0;
}

void testApp::update() {
	if (selecting) {
		return;
	}

	camera.update();
	if (camera.isFrameNew()) {
		Mat thisFrame = toCv(camera);
		if (lastFrame.size == thisFrame.size) {
			Mat difference;
			cv::absdiff(thisFrame, lastFrame, difference);
			this->frameDifference = cv::sum(difference)[0] / (camera.getWidth() * camera.getHeight());
			if (this->frameDifference > calmFrameDifferenceThreshold)
				lastMovement = ofGetElapsedTimef();
			else if(ofGetElapsedTimef() - lastMovement > calmDurationThreshold) {
				//calibration.add(thisFrame);
			}
		}
		ofxCv::copy(thisFrame, lastFrame);
	}
}

void testApp::draw() {
	if (selecting) {
		drawSelect();
		return;
	}
	if (!camera.isReady())
		return;

	camera.draw(0, 0, ofGetWidth(), ofGetHeight());

	stringstream message;
	if (calibration.imagePoints.size() > 0) {
		message << calibration.imagePoints.size() << " recordings made" << endl;
		vector<Point2f>::iterator it;
		ofPushStyle();
		ofNoFill();
		ofSetLineWidth(2.0f);
		ofBeginShape();
		ofSetColor(100,200,100);
		for (it = calibration.imagePoints.back().begin(); it != calibration.imagePoints.back().end(); it++) {
			ofCircle(it->x, it->y, 20);
			ofVertex(it->x, it->y);
		}
		ofSetColor(100,100,200);
		ofEndShape(false);
		ofPopStyle();
	}

	
	message << "frameDifference = " << frameDifference << endl;
	message << "calmFrameDifferenceThreshold = " << calmFrameDifferenceThreshold << endl;
	message << "lastMovement = " << (ofGetElapsedTimef() - lastMovement) << " seconds ago" << endl;
	message << "calmDurationThreshold = " << calmDurationThreshold << endl;
	drawHighlightString(message.str(), 20, 30, ofColor(200, 100, 100));
}

void testApp::drawSelect() {
	int y = -(SELECTION_HEIGHT / 2) + 5;
	stringstream message;
	message << devices.size() << " cameras found. Please select one by clicking on it.";
	ofDrawBitmapString(message.str(), 30, y+=SELECTION_HEIGHT);
	for (int i=0; i<devices.size(); i++) {
		if (iSelection == i) {
			ofPushStyle();
			ofSetColor(200,155,100);
			ofFill();
			ofSetLineWidth(0);
			ofRect(0, (i + 1) * SELECTION_HEIGHT, ofGetWidth(), SELECTION_HEIGHT);
			ofPopStyle();
		}
		ofNoFill();
		ofSetLineWidth(1);
		ofDrawBitmapString(ofToString(devices[i].cameraID) + " " + devices[i].model + " " + devices[i].serial,
			30, y+=SELECTION_HEIGHT);
	}
}

void testApp::keyPressed(int key) {
	if (key == 'f')
		ofToggleFullscreen();
}

void testApp::mouseMoved(int x, int y) {
	if (selecting)
		iSelection = y / SELECTION_HEIGHT - 1;
}

void testApp::mousePressed(int x, int y, int button) {
	if (selecting) {
		iSelection = y / SELECTION_HEIGHT - 1;
		if (iSelection >= 0 && iSelection < devices.size()) {
			camera.open(devices[iSelection]);
			selecting = false;
		}
	}		
}
