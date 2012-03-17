#include "testApp.h"

//-------------
testApp::~testApp() {
}

//-------------
void testApp::setup() {
	ofSetVerticalSync(true);
	ofSetLogLevel(OF_LOG_NOTICE);

	//black backed chessboard from studio
	GlobalBoardFinder::init(0.04, 9, 6);
	this->cameraSet.openDevices( ofxUeye::listDevices() );
	
#ifdef HAVE_GUI
	this->gui.init();
	this->rebuildGUI();
#endif

	this->captureID = 0;
}

//-------------
void testApp::update() {
}

//-------------
void testApp::draw() {
}

//-------------
void testApp::keyPressed(int key) {
	if (key == ' ')
		this->cameraSet.capture(this->captureID++);
	if (key == 'i')
		this->cameraSet.solveIntrinsics();
	if (key == 'e')
		this->cameraSet.solveExtrinsics();
	if (key == 's') {
		this->cameraSet.solveAndSaveCalibration();
		ProCamSet(this->cameraSet).save();
	}

	if (key == 'a')
		this->cameraSet.save();
	if (key == 'z') {
		this->cameraSet.load();
		this->captureID = this->cameraSet.getMaxIndex() + 1;
#ifdef HAVE_GUI
		this->rebuildGUI();
#endif
	}
}


//-------------
void testApp::mouseMoved(int x, int y) {
}

//-------------
void testApp::mousePressed(int x, int y, int button) {

}

//-------------
#ifdef HAVE_GUI
void testApp::rebuildGUI() {
	this->gui.clear();
	this->gui.addInstructions();
	
	vector<ofPtr<CameraHead> > cameras = this->cameraSet.getCamerasAsVector();
	vector<ofPtr<CameraHead> >::iterator it;
	for (it = cameras.begin(); it != cameras.end(); it++)
		this->gui.add( **it, "Camera " + ofToString( (*it)->getCameraID() ) );
}
#endif