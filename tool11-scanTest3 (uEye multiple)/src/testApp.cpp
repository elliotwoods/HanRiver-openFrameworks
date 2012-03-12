#include "testApp.h"

//--------------------------------------------------------------
testApp::~testApp() {
	cameras.clear();
}

//--------------------------------------------------------------
void testApp::setup(){
	ofSetLogLevel(OF_LOG_NOTICE);
	payload.init(1280, 800);
	cameras.push_back(ofPtr<CameraHead>(new CameraHead(1, this->payload)));
	cameras.push_back(ofPtr<CameraHead>(new CameraHead(2, this->payload)));

	gui.init();
	for (int i=0; i<cameras.size(); i++) {
		gui.add(cameras[i]->getCamera(), "Camera " + ofToString(cameras[i]->getID()));
		gui.add(cameras[i]->getDecoder().getCaptures(), "Camera " + ofToString(cameras[i]->getID()) + " capture set");
		gui.add(cameras[i]->getDecoder(), "Decoder " + ofToString(cameras[i]->getID()));
	}

	rx.setup(SERVER_PORT);
}

//--------------------------------------------------------------
void testApp::update(){
	for (int i=0; i<cameras.size(); i++)
		cameras[i]->update();

	while (rx.hasWaitingMessages()) 
		processInput();
}

//--------------------------------------------------------------
void testApp::draw(){

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if (key == OF_KEY_UP)
		moveThreshold(+2);
	if (key == OF_KEY_DOWN)
		moveThreshold(-2);
	
	/*
	if (key=='l')
		decoder.loadDataSet();
	if (key=='s')
		decoder.saveDataSet();
	*/
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
void testApp::processInput(){ 
	
	ofxOscMessage msg;
	while (rx.hasWaitingMessages()) {
		rx.getNextMessage(&msg);
	
		if (msg.getAddress() == "/capture") {
	#pragma omp parallel for
			for (int i=0; i<cameras.size(); i++)
				cameras[i]->capture();
		}

		if (msg.getAddress() == "/clear") {
	#pragma omp parallel for
			for (int i=0; i<cameras.size(); i++)
				cameras[i]->clear();
		}
	}
}

//--------------------------------------------------------------
void testApp::moveThreshold(int distance){
	/*
	if (ofGetKeyPressed(OF_KEY_SHIFT))
		distance *= 5;
	decoder.setThreshold(ofClamp((int)decoder.getThreshold() + distance, 0, 255));
	ofLogNotice() << "Threshold set at " << (int)decoder.getThreshold();
	*/
}