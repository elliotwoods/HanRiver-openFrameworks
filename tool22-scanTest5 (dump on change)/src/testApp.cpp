#include "testApp.h"

//--------------------------------------------------------------
testApp::~testApp() {
	cameras.clear();
}

//--------------------------------------------------------------
void testApp::setup(){
	ofSetLogLevel(OF_LOG_NOTICE);
	payload.init(1280, 800);

	gui.init(); //must init gui before cameras
	vector<ofxUeyeDevice> devices = ofxUeye::listDevices();
	for (int i=0; i<devices.size(); i++)
		cameras.push_back(ofPtr<CameraHead>(new CameraHead(devices[i].cameraID, this->payload, gui)));

	rx.setup(SERVER_PORT);
	string remoteIp = "localhost";
	tx.setup(remoteIp, SERVER_PORT);
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

	if (key=='s')
		for (int i=0; i<cameras.size(); i++)
			cameras[i]->save();
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
	if (cameras.size() == 0)
		return;

	ofxOscMessage msg;
	while (rx.hasWaitingMessages()) {
		rx.getNextMessage(&msg);

		if (msg.getRemoteIp() != this->remoteIp) {
			this->remoteIp = msg.getRemoteIp();
			this->tx.setup(this->remoteIp, CLIENT_PORT);
		}
	
		if (msg.getAddress() == "/capture") {
			#pragma omp parallel for
			for (int i=0; i<cameras.size(); i++)
				cameras[i]->capture();

			if (cameras[0]->getDecoder().getFrame() < payload.getFrameCount()) {
				ofxOscMessage msgCaptured;
				msgCaptured.setAddress("/requestnext");
				tx.sendMessage(msgCaptured);
			}
		}

		if (msg.getAddress() == "/clear") {
			#pragma omp parallel for
			for (int i=0; i<cameras.size(); i++)
				cameras[i]->clear();
		}

		if (msg.getAddress() == "/projector") {
			for (int i=0; i<cameras.size(); i++)
				cameras[i]->setProjectorID(msg.getArgAsInt32(0));
		}
	}
}

//--------------------------------------------------------------
void testApp::moveThreshold(int distance){
	if (ofGetKeyPressed(OF_KEY_SHIFT))
		distance *= 5;
	for (int i=0; i<cameras.size(); i++)
		cameras[i]->moveThreshold(distance);
}