#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofSetLogLevel(OF_LOG_NOTICE);
	ready = false;
	
	setPath(0);
	loadPixels(0);
	setPath(1);
	loadPixels(1);
	
	error[0] = 0.0f;
	error[1] = 0.0f;
	intrinsics[0].setPatternSize(9, 6);
	intrinsics[0].setSquareSize(0.04);
	intrinsics[0].setImageSize(1280, 720);
	intrinsics[1].setPatternSize(9, 6);
	intrinsics[1].setSquareSize(0.04);
	intrinsics[1].setImageSize(1280, 720);
	
	current = filenames.begin();
	
	this->windowResized(ofGetWidth(), ofGetHeight());
	
	ready = true;
}

//--------------------------------------------------------------
void testApp::setPath(int camera) {
	string notice = "Select folder for camera " + ofToString(camera);
	ofLogNotice() << notice;
	string path = ofSystemLoadDialog(notice, true).getPath();
	ofLogNotice() << path;
	files[camera].listDir(path);
}

//--------------------------------------------------------------
void testApp::loadPixels(int camera) {
	int count = files[camera].size();
	ofLogNotice() << "Found " << count << " files for camrea " << camera;
	string path;
	string extension;
	string trunk;
	for (int i = 0; i < count; i++) {
		path = files[camera].getPath(i);
		extension = ofToLower(ofFilePath::getFileExt(path));
		if (extension == "png" || extension == "jpeg" || extension == "jpg") {
			trunk = ofFilePath::getFileName(path);
			ofPtr<BoardFinder> newFinder(new BoardFinder(path)); 
			captures[camera].insert(pair<string, ofPtr<BoardFinder> >(trunk, newFinder));
			filenames.insert(trunk);
		}
	}
}

//--------------------------------------------------------------
void testApp::update(){
}

//--------------------------------------------------------------
void testApp::draw(){
	if (!ready)
		return;
	
	drawCapture(0);
	drawCapture(1);
	
	ofxCv::drawHighlightString(*current, 20, 20);
	
	for (int i=0; i<2; i++) {
		string message = "Reprojection error = " + ofToString(error[i]);
		ofxCv::drawHighlightString(message, 20 + viewports[i].x, ofGetHeight() - 40);
	}
	
}

//--------------------------------------------------------------
void testApp::drawCapture(int camera){
	map<string, ofPtr<BoardFinder> > & capture(captures[camera]);
	if (capture.find(*current) != capture.end())
		capture[*current]->draw(viewports[camera]);
}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){ 
	if (key == OF_KEY_RIGHT) {
		current++;
		if (current == filenames.end())
			current--;
	}
	
	if (key == OF_KEY_LEFT)
		if (current != filenames.begin())
			current--;
	
	if (key == 'c')
		calibrate();
}

//--------------------------------------------------------------
void testApp::calibrate() {
	intrinsics[0].imagePoints.clear();
	intrinsics[1].imagePoints.clear();
	set<string>::iterator it;
	for (it = filenames.begin(); it != filenames.end(); it++) {
		addCalibrationPoints(0, *it);
		addCalibrationPoints(1, *it);
	}
	
	cout << "camera 0 has " << intrinsics[0].imagePoints.size() << " sets of points" << endl;
	cout << "camera 2 has " << intrinsics[1].imagePoints.size() << " sets of points" << endl;
	
	intrinsics[0].calibrate();
	intrinsics[1].calibrate();
	error[0] = intrinsics[0].getReprojectionError();
	error[1] = intrinsics[1].getReprojectionError();
	
	//now we match up image point pairs to create the stereo calibration
	//we could either do this:
	//	1. editing points in the calibration objects
	//			but we could just have 1 calibration object everywhere
	//			and an intrinsics object per projector
	//	2. create a custom map of extrinsics
	vector<vector<Point2f> > imagePoints[2];
	vector<vector<Point3f> > objectPoints;
	bool found;
	for (it = filenames.begin(); it != filenames.end(); it++) {
		found = true;
		for (int i = 0; i<2; i++) {
			found &= captures[i][*it]->hasData();
		} if (found) {
			for (int i = 0; i<2; i++)
				imagePoints[i].push_back(captures[i][*it]->getImagePoints());
			objectPoints.push_back(ofxCv::Calibration::createObjectPoints(cv::Size(9, 6), 0.04, CHESSBOARD));
		}
	}
	ofLogNotice() << "Found " << imagePoints[0].size() << " matching sets of image points";
	
	Mat tra, rot;
	Mat essential;
	Mat fundamental;
	Mat camera[2];
	camera[0] = intrinsics[0].getDistortedIntrinsics().getCameraMatrix();
	camera[1] = intrinsics[1].getDistortedIntrinsics().getCameraMatrix();	
	Mat distortion[2];
	distortion[0] = intrinsics[0].getDistCoeffs();
	distortion[1] = intrinsics[1].getDistCoeffs();
	cv::Size imageSize(1280, 720);
	float stereoError = cv::stereoCalibrate(objectPoints, imagePoints[0], imagePoints[1],
						camera[0], distortion[0],
						camera[1], distortion[1],
						imageSize, rot, tra, essential, fundamental);
	ofLogNotice() << "Stereo calibration performed. Reprojection error = " << stereoError;
	ofLogNotice() << "Translation: " << tra;
	ofLogNotice() << "Rotation: " << rot;
}

//--------------------------------------------------------------
void testApp::addCalibrationPoints(int camera, string filename)  {
	if (captures[camera][filename]->hasData()) {
		intrinsics[camera].imagePoints.push_back(captures[camera][filename]->getImagePoints());
		cout << "adding " << captures[camera][filename]->getImagePoints().size() << " point to camera " << camera << endl;
	}
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
void testApp::windowResized(int w, int h) {
	viewports[0] = ofGetCurrentViewport();
	viewports[0].width /= 2.0f;
	viewports[1] = viewports[0];
	viewports[1].x = viewports[0].width;
}
