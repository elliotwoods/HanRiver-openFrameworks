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
	this->camera = & panel->getCamera();
	this->loadProCamSet("procamset");
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

	if (key == 'a' || key == 'b') {
		ofxGraycode::DataSet dataSet;
		dataSet.load();
		if (dataSet.getHasData()) {
			HanRiverLib::ProCamPairID id( dataSet.getFilename() );
			// if we already have 2 cameras for this projector
			// and the camera that we are adding hasn't already had a calibration bump
			// then we 
			if (key == 'a')
				this->pointSet.add(id, this->proCamSet, dataSet);
			else
				//Here we use the existing 3d points to calibrate the new object
				this->pointSet.calibrateAndAdd(id, this->proCamSet, dataSet);
		}
	}
	if (key == 'c')
		this->pointSet.clear();
	if (key == 'i')
		this->pointSet.findCameraPoints();
	if (key == 'z')
		this->loadFolder();
	if (key >= '1' && key <= '5') {
		ProCamSet::const_iterator it = this->proCamSet.find(key - '1' + 1);
		if ( it != this->proCamSet.end() ) {
			this->camera->setPosition( it->second.getPosition() );
			this->camera->setOrientation( ofQuaternion(180, ofVec3f(0, 1, 0) ) * it->second.getOrientationQuat() );
			this->camera->setFov( it->second.intrinsics.fov.y );
		}
	}
	if (key == 's')
		this->pointSet.saveRaw();
	if (key == 't')
		this->pointSet.saveBigMap();
	if (key == 'u')
		this->pointSet.saveCompressedMap();
	if (key == 'v')
		this->pointSet.saveCorrespondenceTable();
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
	//proCamSet.rotate(180.0f, 1.0f, 0.0f, 0.0f);
	proCamSet.bakeTransform();
}
//-------------
void testApp::loadFolder() {
	//string path = ofSystemLoadDialog("Select folder", true).getPath();
	string path = ofToDataPath("", true);
	//string path = "..\\data";
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
		HanRiverLib::ProCamPairID id( dataSet.getFilename() );
		this->pointSet.add(id, this->proCamSet, dataSet);
	}

	this->pointSet.findCameraPoints();
}