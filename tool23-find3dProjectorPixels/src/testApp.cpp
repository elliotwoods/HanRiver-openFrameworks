#include "testApp.h"

class Bounds : public ofNode {
protected:
	void customDraw() {
		ofPushStyle();
		ofPushMatrix();
		ofTranslate(3.6f, -2.0f, -2.15);
		ofScale(4.0f, 2.0f, 2.0f);
		
		ofNoFill();
		ofSetLineWidth(3.0f);
		ofBox(1.0f);
		
		ofFill();
		ofEnableAlphaBlending();
		ofSetColor(0, 100);
		ofBox(1.0f);
		
		ofPopMatrix();
		ofPopStyle();
	}
};

//-------------
void testApp::setup() {
	ofSetVerticalSync(true);
	ofSetLogLevel(OF_LOG_NOTICE);
	gui.init();
	gui.addInstructions();
	ofPtr<ofxCvGui::Panels::Node> panel = gui.add(proCamSet, "ProCamSet");
	panel->setCursorEnabled(true);
	panel->push(this->pointSet);
	panel->push(*(new Bounds())); //push this on last as it's got alpha
	this->loadProCamSet("procamset");

	CameraSet cameraSet;
	cameraSet.openDevices(ofxUeye::listDevices());
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
		this->loadProCamSet();

	if (key == 'a') {
		ofxGraycode::DataSet dataSet;
		dataSet.load();
		HanRiverLib::ProCamID id( dataSet.getFilename() );
		this->pointSet.add(id, this->proCamSet, dataSet);
	}
	if (key == 'c')
		this->pointSet.clear();
	if (key == 'i')
		this->pointSet.findCameraPoints();
	if (key == 'z')
		this->loadFolder();
}

//-------------
void testApp::loadProCamSet(string filename) {
	ofLogNotice() << "loadProCamSet " << filename;
	proCamSet.resetTransform();
	if (filename != "" && ofFile::doesFileExist(filename))
		proCamSet.load(filename);
	else
		proCamSet.load();

	proCamSet.enforceXZPlane();
	proCamSet.rotate(180.0f, 1.0f, 0.0f, 0.0f);
	proCamSet.bakeTransform();
}
//-------------
void testApp::loadFolder() {
	string path = ofSystemLoadDialog("Select folder", true).getPath();

	if (! ofDirectory::doesDirectoryExist(path) || path == "") {
		ofLogError() << "No folder selected " << path;
		return;
	}

	ofDirectory listDataSets;
	listDataSets.allowExt("DataSet");
	listDataSets.listDir(path);
	for (int i=0; i<listDataSets.size(); i++) {
		ofLogNotice() << "Loading graycode dataset " << ofFilePath::getBaseName( listDataSets.getPath(i) );
		ofxGraycode::DataSet dataSet;
		dataSet.load( listDataSets.getPath(i) );
		HanRiverLib::ProCamID id( dataSet.getFilename() );
		this->pointSet.add(id, this->proCamSet, dataSet);
	}

	this->pointSet.findCameraPoints();
}