#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
	ofDrawBitmapString(ofToString(matrix), 10, 10);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	
	if (key == 's') {
		ofFile writer;
		writer.open(ofSystemSaveDialog("matrix.mat", "Save matrix").getPath(), ofFile::WriteOnly, false);
		writer << matrix;
 		writer.close();
	}

	if (key == 'l') {
		ofFile reader;
		reader.open(ofSystemLoadDialog("Load matrix").getPath(), ofFile::ReadOnly, false);
		reader >> matrix;
		reader.close();
	}

	if (key == 'r') {
		matrix.rotate(90, 0, 0, 1);
	}

	if (key == 'c') {
		matrix.makeIdentityMatrix();
	}

	if (key == 't') {
		ofMatrix4x4 test;
		test.makeIdentityMatrix();
		cout << test << endl;
		ofFile testWrite("save.mat", ofFile::WriteOnly, false);
		testWrite << test;
		testWrite.close();

		test.rotate(90,0,0,1);

		ofFile testRead("save.mat", ofFile::ReadOnly, false);
		testRead >> test;
		cout << test << endl;
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