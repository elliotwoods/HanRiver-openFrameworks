#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	rgb.allocate(640, 480, OF_PIXELS_RGB);
	mono.allocate(640, 480, OF_PIXELS_MONO);

	for (int i=0; i<480; i++) {
		for (int j=0; j<640; j++) {
			rgb.getPixels()[(j + i * 640) * 3] = j;
			mono.getPixels()[j + i * 640] = j;
		}
	}
	
	ofImage monoSaver(mono);
	ofSaveImage(rgb, "rgb.png");
	ofSaveImage(mono, "mono.png");
	monoSaver.saveImage("monoSaver.png");


}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	
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