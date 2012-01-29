#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	maxValue = 0;
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
	ofDrawGrid();
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
	ofDrawBitmapString("[SPACE] = load PFM for points", 10, y+=15);
	
	
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
		screenshot.saveImage("screenshot.png");
	}
	if (key =='e')
		saveCompressed();
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
	if (!input.loadImage(ofSystemLoadDialog("Select image").fileName))
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

//--------------------------------------------------------------
void testApp::saveCompressed() {
	if (!input.isAllocated()) {
		ofLogError() << "Cannot export, image is not yet laoded";
		
		////
		// cycle through and find actives
		////
		//
		vector<ofVec3f> results;
		ofVec3f* xyz = (ofVec3f*)input.getPixels();
		for (int i=0; i<input.getPixelsRef().size(); i++, xyz++) {
			if (xyz->length() > 0.0f) {
				//we got a winner
				results.push_back(*xyz);
			}
		}
		//
		////
		
		
		////
		// output as compressed map
		////
		//
		int count = results.size();
		int width = 1 << (int)ceil(log(sqrt((float)count)) / log(2.0f));
		int height = ceil((float)count / (float)width);

		output.allocate(width, height, OF_IMAGE_COLOR);
		memcpy(output.getPixels(), &results[0], sizeof(ofVec3f) * count);
		output.saveImage("compressed.hdr");
		//
		////
	}
}