#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	maxValue = 0;
	ofBackground(50,50,50);
	mesh.setMode(ofPrimitiveMode::OF_PRIMITIVE_POINTS);
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
	camera.begin();

	ofPushStyle();
	ofSetColor(250,150,150);
	ofDrawGrid();
	ofPopStyle();
	mesh.drawVertices();
	camera.end();

	int y = 0;
	ofDrawBitmapString("Max length: " + ofToString(maxValue), 10, y+=15);
	ofDrawBitmapString("Mean position: " + ofToString(mean), 10, y+=15);
	ofDrawBitmapString("Vertex count: " + ofToString(mesh.getNumVertices()), 10, y+=15);

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
		screenshot.saveImage("screenshot.png");
	}

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
	if (!img.loadPFMImage(ofSystemLoadDialog("Select image").fileName))
		ofLogError() << "Failed to load image";
	else {
		//fill mesh and find max value
		maxValue = 0;
		mean = (ofVec3f)0;

		const int count = img.getWidth() * img.getHeight();
		
		ofVec3f* vertex = (ofVec3f*) img.getPixels();
		mesh.clearVertices();
		float lengthSq;
		
		for (int i=0; i<count; i++, vertex++) {
			lengthSq = vertex->lengthSquared();

			if (lengthSq != 0) {
				//cout << "add vertex " << *vertex << endl;
				mesh.addVertex(*vertex);
				if (lengthSq > maxValue)
					maxValue = lengthSq;
				mean += *vertex;
				/*for (int j=0; j<12; j++)
					printf("%c", ((char*)vertex)+j);
				cout << vertex->x << "," << vertex->y << "," << vertex->z << endl;*/
			}
		}

		maxValue = sqrt(maxValue);
		mean /= float(count);
	}
} 