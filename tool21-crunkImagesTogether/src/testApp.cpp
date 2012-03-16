#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofSetFrameRate(30.0f);

	gui.init();
	gui.addInstructions();
	ofPtr<ofxCvGui::Panels::Node> nodePanel = gui.add(positions, "3D");
	nodePanel->setGridEnabled(false);
	this->camera = &nodePanel->getCamera();
	gui.add(positions.map, "Output");
	ofxCvGui::PanelGroupPtr layerGrid = gui.addGrid();
	positions.load();
	layers.reserve(16);
	for (int i=0; i<16; i++) {
		layers.push_back( Layer(positions, i) );
		layerGrid->add(ofxCvGui::PanelPtr( new ofxCvGui::Panels::Draws( layers.back() ) ) );
	}
	ofBackground(50);

	this->frame = 0;
	this->frameCount = 0;
	this->playing = true;
	this->animateCamera = false;
}

//--------------------------------------------------------------
void testApp::update(){

	positions.map.update();
	if (this->playing && this->frameCount != 0) {
		bool updated = false;
		if ( !this->loaded.count(this->frame) ) {
			if ( ofFile::doesFileExist(this->getPathOfMap(this->frame) ) ) {
				ofPixels loadFrame;
				ofLoadImage( loadFrame, this->getPathOfMap(this->frame) );
				this->loaded.insert(pair<int, ofImage>( this->frame, loadFrame) );
			} else {
				loadFrame();
				for (int i=0; i<layers.size(); i++)
					layers[i].update();
				assignPixels();
				this->loaded.insert(pair<int, ofImage>( this->frame, this->positions.map ) );
				updated=true;
			}
		}

		if (!updated) {
			this->positions.map = this->loaded[this->frame];
			this->positions.map.update();
		}
		
		this->frame++;
		this->frame %= frameCount;
	}

	if (this->animateCamera) {
		this->camera->setPosition(sin(ofGetElapsedTimef() / 20.0f * TWO_PI) * 2.0f, -0.1f, 3.0f + sin(ofGetElapsedTimef() / 40.0f * TWO_PI) * 2.0f);
		this->camera->lookAt( ofVec3f(0, 0, -1.0f) );
	}
}

//--------------------------------------------------------------
void testApp::draw(){
	stringstream status;
	status << "Frame = " << this->frame << " / " << this->frameCount << "\n";
	status << "Playing [" << (this->playing ? "x" : " ") << "]" << endl;
	status << "Animate camera [" << (this->animateCamera ? "x" : " ") << "]" << endl;
	status << "Far plane thickness [" << Layer::thickness << "]";
	ofxCvGui::AssetRegister.drawText(status.str(), 20, 310, "", true);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if (key == 'l')
		loadLayers();
	if (key == 'c')
		clearAll();
	if (key == ' ')
		this->playing ^= true;
	if (key == OF_KEY_LEFT)
		this->frame = 0;
	if (key == 'a')
		this->animateCamera ^= true;
	if (key == OF_KEY_PAGE_UP) {
		Layer::thickness += 0.05;
		Layer::thickness = ofClamp(Layer::thickness, 0, 2);
		for (vector<Layer>::iterator it = this->layers.begin(); it != this->layers.end(); it++)
			it->createMesh(this->positions);
	}
	if (key == OF_KEY_PAGE_DOWN) {
		Layer::thickness -= 0.05;
		Layer::thickness = ofClamp(Layer::thickness, 0, 2);
		for (vector<Layer>::iterator it = this->layers.begin(); it != this->layers.end(); it++)
			it->createMesh(this->positions);
	}
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
void testApp::clearAll() {
	if (ofDirectory::doesDirectoryExist(this->contentPath + "/output")) {
		ofDirectory::removeDirectory(this->contentPath + "/output", true);
		ofDirectory::createDirectory(this->contentPath + "/output", false);
	}
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
	positions.map.saveImage( this->getPathOfMap(this->frame) );
}

//---------
string testApp::getPathOfMap(int frame) const {
	return this->contentPath + "/output/" + ofToString(frame) + ".png";
}