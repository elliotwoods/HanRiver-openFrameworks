#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	maxValue = 0;
	offset = 5.0f;
	ofBackground(50,50,50);
	mesh.setMode(OF_PRIMITIVE_POINTS);
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
	
	if (output.isAllocated())
		output.draw(0,0);
	
	camera.begin();

	ofPushStyle();
	ofSetColor(250,150,150);
	ofDrawGrid(4.0f, 5.0f, true);
	ofPopStyle();
	mesh.drawVertices();
	camera.end();

	int y = 0;
	ofDrawBitmapString("Max length: " + ofToString(maxValue), 10, y+=15);
	ofDrawBitmapString("Mean position: " + ofToString(mean), 10, y+=15);
	ofDrawBitmapString("Vertex count: " + ofToString(mesh.getNumVertices()), 10, y+=15);
	y+=15;
	ofDrawBitmapString("[f] = toggle fullscreen", 10, y+=15);
	ofDrawBitmapString("[s] = screenshot.png", 10, y+=15);
	ofDrawBitmapString("[e] = export as compressed space XYZ hdr", 10, y+=15);
	ofDrawBitmapString("[m] = set mean to 0", 10, y+=15);
	ofDrawBitmapString("[SPACE] = load points", 10, y+=15);
	ofDrawBitmapString("[LEFT]/[RIGHT] = input offset = " + ofToString(offset), 10, y+=15);
	
	if (output.isAllocated()) {
		y += 15;
		ofDrawBitmapString("width = " + ofToString(output.getWidth()), 10, y+=15);
		ofDrawBitmapString("height = " + ofToString(output.getHeight()), 10, y+=15);
	}

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if (key == ' ')
		loadImage();
	if (key == 'c')
		camera.toggleCursorDraw();
	if (key == 'f')
		ofToggleFullscreen();
	if (key == 's') {
		ofImage screenshot;
		screenshot.grabScreen(0,0,ofGetWidth(),ofGetHeight());
		screenshot.saveImage(ofSystemSaveDialog("screenshot.png", "Save screenshot...").fileName);
	}
	if (key =='e')
		saveCompressed();
	if (key =='m')
		centralise();

	if (key == OF_KEY_LEFT)
		offset -= 0.5;
	if (key == OF_KEY_RIGHT)
		offset += 0.5;

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

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
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}

//--------------------------------------------------------------
void testApp::loadImage() {
	if (!input.loadImage(ofSystemLoadDialog("Select image").getPath()))
		ofLogError() << "Failed to load image";
	else {
		//fill mesh and find max value
		maxValue = 0;
		mean = (ofVec3f)0;

		const int count = input.getWidth() * input.getHeight();
		
		ofVec3f* vertex = (ofVec3f*) input.getPixels();
		mesh.clearVertices();
		float lengthSq;
		
		for (int i=0; i<count; i++, vertex++) {
			lengthSq = vertex->lengthSquared();

			/**HACK**/
			//Having issue with negatives saved out from vvvv
			*vertex -= offset;

			if (lengthSq != 0) {
				mesh.addVertex(*vertex);
				if (lengthSq > maxValue)
					maxValue = lengthSq;
				mean += *vertex;
			}
		}

		maxValue = sqrt(maxValue);
		mean /= float(mesh.getNumVertices());
		input.update();
	}
} 

//--------------------------------------------------------------
void testApp::saveCompressed() {
	if (!input.isAllocated()) {
		ofLogError() << "Cannot export, image is not yet laoded";
		return;
	}		
	////
	// output as compressed map
	////
	//
	int count = mesh.getNumVertices();
	int width = 1 << (int)ceil(log(sqrt((float)count)) / log(2.0f));
	int height = width; //ceil((float)count / (float)width);

	output.allocate(width, height, OF_IMAGE_COLOR);
	memcpy(output.getPixels(), mesh.getVerticesPointer(), sizeof(ofVec3f) * count);
	output.saveImage(ofSystemSaveDialog("output.hdr", "Save hdr...").getPath());
	//
	////
}

//--------------------------------------------------------------
void testApp::centralise() {
	ofVec3f *vMesh = mesh.getVerticesPointer();
	int count = mesh.getNumVertices();
	
	for (int i=0; i<count; i++, vMesh++) {
		*vMesh -= mean;
	}
	mean -= mean;
}