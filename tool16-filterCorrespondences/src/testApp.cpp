#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofBackground(50);
	//indices start at 1 here
	camera = 0;
	projector = 0;
	addProjector();
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
	stringstream message;
	message << "Loads ofxGraycode datasets and outputs a correspondence table" << endl;
	message << "Add a projector, then add multiple cameras, then add next projector, then cameras, etc.." << endl;
	message << "[p] = add projector (current index " << projector << " << 8)" << endl;
	message << "[c] = add camera (current index " << camera << ")" << endl;
	message << "[s] = save correspondence table";

	ofDrawBitmapString(message.str(), 20, 30);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if (key == 'p')
		addProjector();
	if (key == 'c')
		addCamera();
	if (key == 's')
		correspondences.save();
}

//--------------------------------------------------------------
void testApp::addProjector(){
	projector++;
	camera = -1;
	addCamera();
}

//--------------------------------------------------------------
void testApp::addCamera(){
	camera++;
	ofxGraycode::DataSet scan;
	cout << "Select correspondences between camera " << camera << " and projector " << projector << endl;
	scan.load();
	correspondences.add(scan, camera, projector << 8);
}
