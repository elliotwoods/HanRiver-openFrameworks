#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofBackground(100,100,100);
	sharpness = 0;

	history = vector<float>(100, 0.0f);
	position = history.begin();

	ofEnableSmoothing();

	showPreview = false;
	ofSetCircleResolution(40);

	magnification = 1.0f;
	selecting = true;
	
	this->scanCameras();
	if (this->devices.size() == 1)
		selectCamera(0);
}


//--------------------------------------------------------------
void testApp::update(){
	if (selecting)
		return;

	camera.update();
	Mat input = toCv(camera);
	cv::Sobel(input, treated, 3, 2, 2, 3);
	ofxCv::copy(treated, preview);
	cv::absdiff(treated, cv::Scalar(0.0f), treated);
	cv::Scalar sum = cv::sum(treated);
	this->sharpness = sqrt(sum[0]  / (camera.getWidth() * camera.getHeight())) * 10;
	
	preview.update();
	preview.getTextureReference().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);

	if (++position == history.end())
		position = history.begin();

	*position = this->sharpness;
}

//--------------------------------------------------------------
void testApp::draw(){
	if (selecting) {
		drawSelect();
		return;
	}

	ofSetColor(255);
	ofVec2f factor = 1.0f - ofVec2f(ofGetWidth() / (camera.getWidth() * magnification), ofGetHeight() / (camera.getHeight() * magnification));
	if (factor.x < 0.0f)
		factor = 0.0f;
	ofVec2f offset((float)ofGetMouseX() / ofGetWidth(), (float)ofGetMouseY() / ofGetHeight());
	ofPushMatrix();
	ofTranslate(-1.0f * ofVec2f(camera.getWidth(), camera.getHeight()) * offset * factor);// + ofVec2f(ofGetWidth(), ofGetHeight()) );
	ofScale(magnification, magnification);
	
	if (showPreview)
		preview.draw(0, 0);
	else
		camera.draw(0, 0);
	ofPopMatrix();

	ofPushMatrix();
	ofScale(30.0f, 30.0f, 30.0f);
	string text = ofToString(sharpness, 1);
	ofEnableAlphaBlending();
	ofFill();
	ofSetColor(40,40,40,100);
	ofRect(5,15,text.length() * 8 + 10, 20);
	ofSetColor(255);
	ofDrawBitmapString(text, 10, 30);
	ofPopMatrix();

	float max = 0;
	float min = 1000000.0f;
	for (int i=0; i<history.size(); i++) {
		if (history[i] > max)
			max = history[i];
		if (history[i] < min)
			min = history[i];
	}
	if (min == max)
		min--;

	ofPushStyle();
	ofNoFill();
	ofBeginShape();
	ofSetLineWidth(3.0f);
	ofSetColor(200,100,100);
	for (int i=0; i<history.size(); i++) {
		float x = (float)i / (float)history.size() * ofGetWidth();
		float y = ofGetHeight() - (history[i] - min) * ofGetHeight() / (max - min);
		ofVertex(x, y);
		if (history.begin() + i == position) {
			ofPushStyle();
			ofSetLineWidth(2.0f);
			ofSetColor(100,100,100);
			ofEndShape(false);
			ofBeginShape();
			ofPopStyle();
			ofCircle(x, y, 20);
		} else {
			ofCircle(x, y, 10);
		}
	}
	ofSetLineWidth(2.0f);
	ofSetColor(100,100,100);
	ofEndShape(false);
	ofPopStyle();

	ofDrawBitmapString("[f] = toggle fullscreen", 10,10);
	ofDrawBitmapString("[p] = toggle preview", 10,20);
	ofDrawBitmapString("[left click]/[right click] = zoom in/zoom out [x" + ofToString(magnification) + "]", 10,30);
	ofDrawBitmapString("[r] = reselect camera", 10, 40);
	ofDrawBitmapString("[c] = set pixel clock 96", 10, 50);
	ofDrawBitmapString("[o] = set optimal camera timing", 10, 60);
	ofDrawBitmapString(device.toString(), 10, 90);
}


//--------------------------------------------------------------
void testApp::keyPressed  (int key){
	if (key == 'f')
		ofToggleFullscreen();

	if (selecting) {
		if (key == ' ')
			scanCameras();
		return;
	}

	if (key=='p')
		showPreview ^= true;
	if (key=='c')
		camera.setPixelClock(96);
	if (key=='o')
		camera.setOptimalCameraTiming();

	if (key=='r') {
		camera.close();
		this->selecting = true;
	}

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){ 
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	if (selecting)
		iSelection = y / SELECTION_HEIGHT - 1;
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	if (button==0)
		magnification *= 2.0f;
	if (button==2)
		magnification /= 2.0f;
	magnification = ofClamp(magnification, 0.25, 16.0f);


	if (selecting) {
		iSelection = y / SELECTION_HEIGHT - 1;
		this->selectCamera(iSelection);
	}		
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
void testApp::scanCameras() {
	devices = ofxUeye::listDevices();
}

void testApp::selectCamera(int iSelection) {
	if (iSelection >= 0 && iSelection < devices.size()) {
		this->device = devices[iSelection];
		camera.init(this->device);
		selecting = false;
		camera.setExposure(50.0f);
		camera.setGain(0.0f);
	}
}

void testApp::drawSelect() {
	int y = -(SELECTION_HEIGHT / 2) + 5;
	stringstream message;
	message << devices.size() << " cameras found. Please select one by clicking on it. Press [SPACE] to rescan.";
	ofDrawBitmapString(message.str(), 30, y+=SELECTION_HEIGHT);
	for (int i=0; i<devices.size(); i++) {
		if (iSelection == i) {
			ofPushStyle();
			ofSetColor(200,155,100);
			ofFill();
			ofSetLineWidth(0);
			ofRect(0, (i + 1) * SELECTION_HEIGHT, ofGetWidth(), SELECTION_HEIGHT);
			ofPopStyle();
		}
		ofNoFill();
		ofSetLineWidth(1);
		ofDrawBitmapString(ofToString(devices[i].cameraID) + " " + devices[i].model + " " + devices[i].serial,
			30, y+=SELECTION_HEIGHT);
	}
}