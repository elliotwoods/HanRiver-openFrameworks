#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofSetLogLevel(OF_LOG_NOTICE);

	payload.init(1280, 800);

	ofSetWindowShape(payload.getWidth(), payload.getHeight() / 2);
	video.initGrabber(2592, 1944, true);
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
	int width = payload.getWidth() / 2;
	int height = payload.getHeight() / 2;

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

	if (key=='v')
		video.videoSettings();

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
		decoder << video;
		ofLogNotice() << "capture frame " << decoder.getFrame();
	}

	if (msg.getAddress() == "/clear") {
		ofLogNotice() << "clear frames";
		decoder.reset();
	}
}

//--------------------------------------------------------------
void testApp::moveFrame(int distance){ 
	previewFrame = ofClamp(previewFrame + distance, -2, decoder.getCaptures().size()-1);

	ofLogNotice() << "Preview frame #" << previewFrame;
	if (previewFrame >= 0 && previewFrame < decoder.getCaptures().size())
		capturePreview = ofImage(decoder.getCaptures()[previewFrame]);
	if (previewFrame == -1)
		capturePreview = ofImage(decoder.getMean());
}

//--------------------------------------------------------------
void testApp::moveThreshold(int distance){ 
	decoder.setThreshold(ofClamp((int)decoder.getThreshold() + distance, 0, 255));
	ofLogNotice() << "Threshold set at " << (int)decoder.getThreshold();
}