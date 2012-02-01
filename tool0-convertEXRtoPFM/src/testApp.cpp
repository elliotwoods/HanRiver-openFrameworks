#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
	ofDrawBitmapString(filename1, 10, 15);
	ofDrawBitmapString(filename2, 10, 30);

	if (img.isAllocated())
		img.draw(10, 50);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	convertImage();
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
	filename1 = ofSystemLoadDialog("load").filePath;
	filename2 = ofToDataPath(filename1);
	img.loadImage(filename1);
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
void testApp::convertImage() {

	ofFloatImage image;
	ofFile file(ofSystemLoadDialog("Load EXR image...").filePath);
	if (!image.loadImage(file)) {
		ofLogError() << "Failed to load image";
		return;
	} else {
		image.saveImage(ofSystemSaveDialog("output.pfm", "Save PFM image...").filePath);
	}
} 