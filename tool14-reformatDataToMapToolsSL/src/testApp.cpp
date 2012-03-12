#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	cout << "Select camera 1 to projector 1" << endl;
	string c1p1Filename = ofSystemLoadDialog("Select camera 1 to projector 1").getPath();
	if (c1p1Filename == "")
		return;

	cout << "Select camera 2 to projector 1" << endl;
	string c2p1Filename = ofSystemLoadDialog("Select camera 2 to projector 1").getPath();
	if (c2p1Filename == "")
		return;

	DataSet c1p1, c2p1;
	c1p1.load(c1p1Filename);
	c2p1.load(c2p1Filename);

	if (c1p1.getPayloadSize() != c2p1.getPayloadSize()) {
		ofLogError() << "2 Data sets dont match projector (payload) dimensions)";
			return;
	}

	if (!(c1p1.getHasData() && c2p1.getHasData())) {
		ofLogError() << "load failed";
		return;
	}

	cout << "Select output MapToolsSL file" << endl;
	string outputFilename = ofSystemSaveDialog("scan.bin", "Select output file").getPath();

	ofstream output(outputFilename.c_str(), ios::binary);

	uint8_t cameraCount = 2;
	uint16_t projectorWidth = c1p1.getPayloadWidth();
	uint16_t projectorHeight = c1p1.getPayloadHeight();
	uint32_t size = projectorWidth * projectorHeight;

	uint32_t cameraWidth = c1p1.getWidth();
	uint32_t cameraHeight = c2p1.getHeight();
	ofVec2f cameraSize(cameraWidth, cameraHeight);

	output.write((char*)&cameraCount, 1);
	output.write((char*)&projectorWidth, 2);
	output.write((char*)&projectorHeight, 2);

	uint32_t camera1, camera2;
	uint16_t projectorX, projectorY;
	ofVec2f projectorSize = ofVec2f(projectorWidth, projectorHeight);

	const uint8_t* active1 = c1p1.getActive().getPixels();
	const uint8_t* active2 = c2p1.getActive().getPixels();

	ofVec2f projectorNorm;
	ofVec2f camera1Norm, camera2Norm;
	for (uint32_t i=0; i<size; i++) {
		camera1 = c1p1.getDataInverse()[i];
		camera2 = c2p1.getDataInverse()[i];
		if (active1[camera1] && active2[camera2]) {
			projectorX = i % (uint32_t) projectorWidth;
			projectorY = i % (uint32_t) projectorHeight;			
			projectorNorm = ofVec2f(projectorX, projectorY) / projectorSize;

			camera1Norm = ofVec2f(camera1 % cameraWidth, camera1 / cameraHeight) / cameraSize;
			camera2Norm = ofVec2f(camera2 % cameraWidth, camera2 / cameraHeight) / cameraSize;
			
			output.write((char*)&projectorX, 2);
			output.write((char*)&projectorY, 2);
			output.write((char*)&projectorNorm, 2 * 4);

			output.write((char*)&camera1Norm, 2 * 4);
			output.write((char*)&camera2Norm, 2 * 4);
		}
	}

	output.close();

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