#include "testApp.h"

using namespace ofxCv;
using namespace cv;

const float diffThreshold = 2.5; // maximum amount of movement
const float timeThreshold = 1; // minimum time between snapshots
const int startCleaning = 10; // start cleaning outliers after this many samples

//-------------
void testApp::setup() {
	ofSetFrameRate(60);
	ofSetLogLevel(OF_LOG_NOTICE);

	//cameras
	vector<ofxUeyeDevice> devices = ofxUeye::listDevices();
	vector<ofxUeyeDevice>::iterator it;
	for (it = devices.begin(); it != devices.end(); it++)
		cameras.push_back(ofPtr<CameraHead> (new CameraHead(*it)));
	this->captureCount = 0;
	
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
	if (key == 's')
		solveExtrinsicsAndSave();
	if (key == 'a')
		savePixels();
}

//-------------
void testApp::mouseMoved(int x, int y) {
}

//-------------
void testApp::mousePressed(int x, int y, int button) {

}

//-------------
void testApp::add() {
	vector<ofPtr<CameraHead> >::iterator it;
//#pragma omp parallel for
	for (int i=0; i<cameras.size(); i++) {
		cameras[i]->add(this->captureCount);
	}
	lastAdd = ofGetElapsedTimef();
	this->captureCount++;
}

//-------------
void testApp::solveExtrinsicsAndSave() {
	/**<HACK>**/
	//let's just solve for 2 cameras for now
	int cameraCount = 2;
	vector<CameraSolver*> solvers(cameraCount);
	vector<Calibration*> calibrations(cameraCount);
	vector<const Intrinsics*> intrinsics(cameraCount);
	vector<vector<vector<Point2f> > > imagePoints(cameraCount);
	vector<int> cameraIndices(cameraCount);

	ofLogNotice() << "Calibrating intrinsics...";
#pragma omp parallel for
	for (int i=0; i<cameraCount; i++)
		cameras[i]->getSolver().calibrate();

	for (int i=0; i<cameraCount; i++) {
		solvers[i] = &cameras[i]->getSolver();
		solvers[i]->calibrate();
		calibrations[i] = &solvers[i]->lockCalibration();
		intrinsics[i] = &calibrations[i]->getDistortedIntrinsics();
		cameraIndices[i] = cameras[0]->getCamera().getCameraID();
	}

	ofLogNotice() << "Selecting image points";
	vector<vector<Point2f> > imagePointsPerCameras;
	vector<Point2f> imagePointsPerCamera;
	vector<int> imagePointIndex(cameraCount, 0);
	for (int i=0; i<captureCount; i++) {
		imagePointsPerCamera.clear();
		bool found = true;
		for (int j=0; j<cameraCount; j++) {
			imagePointsPerCamera = solvers[j]->getCapture(i);
			if (imagePointsPerCamera.size() == 0)
				found = false;
			else
				imagePointsPerCameras.push_back(imagePointsPerCamera);
		}

		if (found)
			for (int j=0; j<cameraCount; j++)
				imagePoints[j].push_back(imagePointsPerCameras[j]);
	}

	ofLogNotice() << "Found " << imagePoints[0].size() << " pairs";
	if (imagePoints[0].size() == 0) {
		ofLogError() << "No matching pairs found, abort abort!";
		return;
	}

	Mat translation, rotation;
	Mat fundamental, essential; //throw these away
	float error = cv::stereoCalibrate(calibrations[0]->getObjectPoints(),
		imagePoints[0], imagePoints[1],
		intrinsics[0]->getCameraMatrix(), calibrations[0]->getDistCoeffs(),
		intrinsics[1]->getCameraMatrix(), calibrations[1]->getDistCoeffs(),
		intrinsics[0]->getImageSize(),
		rotation, translation,
		essential, fundamental);

	solvers[0]->unlockCalibration();
	solvers[1]->unlockCalibration();

	ofLogNotice() << "stereoCalibrate performed between camreas " << cameras[0]->getCamera().getCameraID() << " and " << cameras[1]->getCamera().getCameraID() << ".";
	ofLogNotice() << error << "reprojection error.";

	uint32_t index = cameraIndices[0] + (cameraIndices[1] << 16);
	extrinsics[index] = ofxCv::makeMatrix(rotation, translation);

	map<uint32_t, ofMatrix4x4>::const_iterator it;
	for (it = this->extrinsics.begin(); it != this->extrinsics.end(); it++) {
		cout << "saving intrinsics set " << it->first << endl;
		string filename = ofToDataPath("c" + ofToString(it->first % (1 << 16)) + "c" + ofToString(it->first >> 16) + ".mat", true);
		ofstream outputMatrixFile(filename, ios::binary);
		outputMatrixFile.write((char*)&it->second, sizeof(ofMatrix4x4));
		outputMatrixFile << endl;
		outputMatrixFile << it->second;
		outputMatrixFile.close();
	}

	for (int i=0; i<cameraCount; i++) {
		solvers[i]->unlockCalibration();
		calibrations[i]->save(string("c") + ofToString(cameraIndices[i]) + ".yml");
	}
	/**</HACK>**/
	
	//lock all cameras

	//find corresponding pairs
	//all pairs? 
	//setup 3d view with all cameras included

}

//-------------
void testApp::savePixels() {
	for (int i=0; i<cameras.size(); i++)
		cameras[i]->savePixels();
}