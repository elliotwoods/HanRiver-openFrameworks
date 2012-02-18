#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofSetLogLevel(OF_LOG_NOTICE);

	payload.init(1280, 800);

	ofSetWindowShape(payload.getWidth(), payload.getHeight() / 2);
	
	video.init(1, IS_SET_CM_BAYER);
	decoder.init(payload);

	rx.setup(SERVER_PORT);

	previewFrame = -2;
}

//--------------------------------------------------------------
void testApp::update(){
	video.update();

	while (rx.hasWaitingMessages()) 
		processInput();
}

//--------------------------------------------------------------
void testApp::draw(){
	int width = ofGetWidth() / 2;
	int height = ofGetHeight();

	video.draw(0, 0, width, height);
	if (decoder.hasData() && previewFrame == -2)
		decoder.draw(width, 0, width, height);
	else if (capturePreview.isAllocated())
		capturePreview.draw(width, 0, width, height);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if (key == OF_KEY_LEFT)
		moveFrame(-1);
	if (key == OF_KEY_RIGHT)
		moveFrame(+1);
	if (key == OF_KEY_UP)
		moveThreshold(+2);
	if (key == OF_KEY_DOWN)
		moveThreshold(-2);
	
	if (key=='l')
		decoder.loadDataSet();
	if (key=='s')
		decoder.saveDataSet();
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
	rx.getNextMessage(&msg);

	if (msg.getAddress() == "/capture") {
		video.update();
		decoder << video;
		ofLogNotice() << "capture frame " << decoder.getFrame();
	}

	if (msg.getAddress() == "/clear") {
		ofLogNotice() << "clear frames";
		decoder.reset();
		video.update();
		decoder << video;
		ofLogNotice() << "capture frame " << decoder.getFrame();
	}
}

//--------------------------------------------------------------
void testApp::moveFrame(int distance){ 
	previewFrame = ofClamp(previewFrame + distance, -3, decoder.getCaptures().size()-1);

	ofLogNotice() << "Preview frame #" << previewFrame;
	if (previewFrame >= 0 && previewFrame < decoder.getCaptures().size())
		capturePreview = ofImage(decoder.getCaptures()[previewFrame]);
	if (previewFrame == -1)
		capturePreview = decoder.getCameraInProjector();
	if (previewFrame == -2)
		capturePreview = ofImage(decoder.getMean());
}

//--------------------------------------------------------------
void testApp::moveThreshold(int distance){ 
	if (ofGetKeyPressed(OF_KEY_SHIFT))
		distance *= 5;
	decoder.setThreshold(ofClamp((int)decoder.getThreshold() + distance, 0, 255));
	ofLogNotice() << "Threshold set at " << (int)decoder.getThreshold();
	moveFrame(0);
}