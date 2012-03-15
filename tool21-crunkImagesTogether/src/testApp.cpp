#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofSetFrameRate(30.0f);
	gui.init();
	ofBackground(20);
	gui.addInstructions();
	
	ofPtr<ofxCvGui::Panels::Node> nodePanel = gui.add(positions, "3D");
	nodePanel->setGridColor( ofColor(40) );

	gui.add(positions.map, "Output");
	
	ofxCvGui::PanelGroupPtr layerGrid = gui.addGrid();
	positions.load();
	layers.reserve(16);
	for (int i=0; i<16; i++) {
		layers.push_back( Layer(positions, i) );
		layerGrid->add(ofxCvGui::PanelPtr( new ofxCvGui::Panels::Draws( layers.back() ) ) );
	}

	this->frame = 0;
	this->frameCount = 0;
	this->playing = true;
}

//--------------------------------------------------------------
void testApp::update(){

	positions.map.update();
	if (this->playing && this->frameCount != 0) {
		if ( this->loaded.count(this->frame) ) {
			this->positions.map = this->loaded[this->frame];
			this->positions.map.update();
		} else {
			loadFrame();
			for (int i=0; i<layers.size(); i++)
				layers[i].update();
			assignPixels();
			this->loaded.insert(pair<int, ofImage>( this->frame, this->positions.map ) );
		}
		
		this->frame++;
		this->frame %= frameCount;
	}
}

//--------------------------------------------------------------
void testApp::draw(){
	stringstream status;
	status << "Frame = " << this->frame << " / " << this->frameCount << "\n";
	status << "Playing [" << (this->playing ? "x" : " ") << "]";
	ofxCvGui::AssetRegister.drawText(status.str(), 20, 300, "", true);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if (key == 'l')
		loadLayers();
	if (key == ' ')
		this->playing ^= true;
	if (key == OF_KEY_LEFT)
		this->frame = 0;
}

//--------------------------------------------------------------
void testApp::loadLayers() {
	contentPath = ofSystemLoadDialog("Select content path", true).getPath();
	if (!ofDirectory::doesDirectoryExist(contentPath) || contentPath=="") {
		ofLogError() << "Path " << contentPath << " not found!";
		return;
	}

	ofDirectory findFolders;
	findFolders.listDir( contentPath );
	const int cameraStringLength = string("Camera").length();

#pragma omp parallel for
	for (int i=0; i<findFolders.size(); i++) {
		string path;
		string trunk;
		string cameraIndex;
		path = findFolders.getPath(i);
		trunk = ofFilePath::getFileName(path);
		if (trunk.length() > cameraStringLength) {
			if (trunk.substr(0, cameraStringLength) == "Camera") {
				cameraIndex = trunk.substr(cameraStringLength, trunk.length() - cameraStringLength);
				layers[ofToInt(cameraIndex) - 1].load(path);
			}
		}
	}

	if (!ofDirectory::doesDirectoryExist(this->contentPath + "/output"))
		ofDirectory::createDirectory(this->contentPath + "/output", false);

	this->frameCount = this->layers.front().count();
	this->frame = 0;
	this->loaded.clear();
}

//--------------------------------------------------------------
void testApp::loadFrame() {
#pragma omp parallel for
	for (int i=0; i<layers.size(); i++)
		layers[i].setFrame(this->frame);
}

//--------------------------------------------------------------
void testApp::assignPixels() {
	positions.fbo.begin();

	ofClear(0,0,0,255);
	vector<Layer>::iterator it;
	for (it = this->layers.begin(); it != this->layers.end(); it++)
		it->drawBits();

	positions.fbo.end();
	positions.fbo.readToPixels(positions.map.getPixelsRef());
	positions.map.saveImage(this->contentPath + "/output/" + ofToString(frame) + ".png");
}