#include "testApp.h"

using namespace ofxCv;
using namespace cv;

const float diffThreshold = 2.5; // maximum amount of movement
const float timeThreshold = 1; // minimum time between snapshots
const int startCleaning = 10; // start cleaning outliers after this many samples


void testApp::setup() {
	ofSetVerticalSync(true);
	selecting = true;
	ofBackground(100);

	this->frameDifference = 0;
	this->calmFrameDifferenceThreshold = 2.0f;
	this->calmDurationThreshold = 0.5f;
	this->lastMovement = 0;
	this->active = false;
	this->waitingForMovement = false;

	calibration.setPatternType(ofxCv::CalibrationPattern::CHESSBOARD);
	calibration.setSquareSize(0.04);
	calibration.setPatternSize(9, 6);

	this->scanCameras();
	if (this->devices.size() == 1)
		selectCamera(0);
}

void testApp::update() {
	if (selecting) {
		return;
	}

	camera.update();
	if (camera.isFrameNew()) {
		camera.lock();
		thisFrame = toCv(camera);
		camera.unlock();
		if (lastFrame.size == thisFrame.size) {
			Mat difference;
			cv::absdiff(thisFrame, lastFrame, difference);
			this->frameDifference = cv::sum(difference)[0] / (camera.getWidth() * camera.getHeight());
			if (this->frameDifference > calmFrameDifferenceThreshold) {
				this->lastMovement = ofGetElapsedTimef();
				this->waitingForMovement = false;
			} else if(active && ofGetElapsedTimef() - lastMovement > calmDurationThreshold)
				addFrame();
			if (!active || waitingForMovement)
				this->lastMovement = ofGetElapsedTimef();
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
		float x, y;
		for (it = calibration.imagePoints.back().begin(); it != calibration.imagePoints.back().end(); it++) {
			x = it->x / camera.getWidth() * ofGetWidth();
			y = it->y / camera.getHeight() * ofGetHeight();
			ofCircle(x, y, 20);
			ofVertex(x, y);
		}
		ofSetColor(100,100,200);
		ofEndShape(false);
		ofPopStyle();
	}

	message << "[SPACE] = toggles automatic capture " << (active ? "[x]" : "[ ]") << endl;
	message << "[a] = add frame manually" << endl;
	message << endl;
	message << calibration.imagePoints.size() << " captures made" << endl;
	if (calibration.isReady())
		message << calibration.getReprojectionError() << "px reprojetion error" << endl;
	if (active) {
		message << "frameDifference = " << frameDifference << endl;
		message << "calmFrameDifferenceThreshold = " << calmFrameDifferenceThreshold << endl;
		message << "lastMovement = " << (ofGetElapsedTimef() - lastMovement) << " seconds ago" << endl;
		message << "calmDurationThreshold = " << calmDurationThreshold << endl;
		if (waitingForMovement)
			message << "Please move board to new location" << endl;
	}
	ofPushMatrix();
	ofScale(2.0f, 2.0f);
	drawHighlightString(message.str(), 20, 20, ofColor(200, 100, 100));
	ofPopMatrix();
}

void testApp::scanCameras() {
	devices = ofxUeye::listDevices();
}

void testApp::selectCamera(int iSelection) {
	if (iSelection >= 0 && iSelection < devices.size()) {
		camera.open(devices[iSelection]);
		this->device = devices[iSelection];
		selecting = false;
	}
}

void testApp::drawSelect() {
	int y = -(SELECTION_HEIGHT / 2) + 5;
	stringstream message;
	message << devices.size() << " cameras found. Please select one by clicking on it. Press [SPACE] to rescan.";
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
	if (selecting) {
		if (key == ' ')
			scanCameras();
		return;
	}

	if (key == ' ')
		active = !active;
	if (key == 'f')
		ofToggleFullscreen();
	if (key == 'a')
		addFrame();
	if (key == 's')
		calibration.save(this->device.serial + ".yml");
}

void testApp::mouseMoved(int x, int y) {
	if (selecting)
		iSelection = y / SELECTION_HEIGHT - 1;
}

void testApp::mousePressed(int x, int y, int button) {
	if (selecting) {
		iSelection = y / SELECTION_HEIGHT - 1;
		this->selectCamera(iSelection);
	}		
}

void testApp::addFrame() {
	vector<Point2f> corners;
	this->calibration.add(thisFrame);
	this->lastMovement = ofGetElapsedTimef();
	this->waitingForMovement = true;

	if (calibration.imagePoints.size() > MIN_CAPTURES) {
		calibration.calibrate();
	}
}