#include "testApp.h"

//--------------------------------------------------------------
testApp::~testApp() {

}

//--------------------------------------------------------------
void testApp::setup(){
	ofSetLogLevel(OF_LOG_NOTICE);
	ofSetVerticalSync(true);
	ofSetFullscreen(true);
	ofHideCursor();
	ofBackground(0);

	payload.init(1280, 800);
	encoder.init(payload);
	this->projector = 0;
	this->frameSent = false;

	payload.allocatePixels(pixels);
	tex.allocate(pixels);
	tex.setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);

	vector<ofxUeyeDevice> devices = ofxUeye::listDevices();
	cameras.reserve( devices.size() );
	decoders.reserve( devices.size() );
#pragma omp parallel for
	for (int i=0; i<devices.size(); i++) {
		ofxUeye * camera = new ofxUeye();
		if (devices[i].model == "UI548xCP-C")
			camera->init( devices[i], IS_SET_CM_BAYER );
		else
			camera->init( devices[i], IS_SET_CM_Y8 );
		ofxUeyePreset_5480SL().apply( * camera );
		cameras.push_back( camera );
	}

	for (int i=0; i<devices.size(); i++) {
		ofxGraycode::Decoder * decoder = new ofxGraycode::Decoder();
		decoder->init( this->payload );
		decoders.push_back( decoder );
	}
}

//--------------------------------------------------------------
void testApp::update(){
	//just incase, we don't use first 5 frames
	//this doesn't seem to matter
	//just paranoid
	if (ofGetFrameNum() < 5)
		return;

	if (frameSent) {
		ofSleepMillis(40);
#pragma omp parallel for
		for (int i=0; i<cameras.size(); i++) {
			cameras[i]->capture();
			cameras[i]->capture();
			*decoders[i] << *cameras[i];
		}
	}
	frameSent = encoder >> pixels;
	if (!frameSent) {
#pragma omp parallel for
		for (int i=0; i<cameras.size(); i++) { 
			decoders[i]->saveDataSet("c" + ofToString(cameras[i]->getCameraID()) + "-p" + ofToString(this->projector) + ".DataSet");
			decoders[i]->reset();
		}
		encoder.reset();
		projector++;
		if (projector == 5)
			std::exit(0);
	} else
		tex.loadData( pixels );
}

//--------------------------------------------------------------
void testApp::draw(){
	if (projector < 2)
		// bottom row
		tex.draw( (1 + projector) * 1280, 800);
	else
		// top row
		tex.draw( (projector - 2) * 1280, 0);

	stringstream message;
	message << "Projector: " << this->projector << endl;
	message << "Frame: " << this->encoder.getFrame() << "/" << this->encoder.getFrameCount();
	ofDrawBitmapString(message.str(), 20, 850);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if (key == OF_KEY_ESC) {
		ofSetFullscreen( false );
//#pragma omp parallel for
//		for (int i=0; i<this->cameras.size(); i++)
//			this->cameras[i].close();
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
