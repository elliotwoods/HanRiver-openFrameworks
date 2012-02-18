#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofSetFullscreen(true);
	ofBackground(100,100,100);
	sharpness = 0;

	history = vector<float>(100, 0.0f);
	position = history.begin();

	ofEnableSmoothing();

	camera.init(1);
	camera.setPixelClock(96);
	showPreview = false;
	ofSetCircleResolution(40);

	magnification = 1.0f;
}


//--------------------------------------------------------------
void testApp::update(){
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

	ofSetColor(255);
	float factor = (magnification - 1.0f);
	ofVec2f offset((float)ofGetMouseX() / ofGetWidth(), (float)ofGetMouseY() / ofGetHeight());
	offset *= ofVec2f(camera.getWidth() * factor,
						camera.getHeight() * factor);
	ofPushMatrix();
	ofTranslate(-offset);
	ofScale(magnification, magnification);
	if (showPreview)
		preview.draw(0, 0);
	else
		camera.draw(0, 0);
	ofPopMatrix();

	ofPushMatrix();
	ofScale(30.0f, 30.0f, 30.0f);
	string text = ofToString(sharpness, 1);
	ofSetColor(40);
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
}


//--------------------------------------------------------------
void testApp::keyPressed  (int key){ 
	if (key=='p')
		showPreview ^= true;
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
	magnification *= 2.0f;
	if (magnification > 16.0f)
		magnification = 0.5f;
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
