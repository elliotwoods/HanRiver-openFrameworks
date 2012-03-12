#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	
	camWidth 		= 1280;	// try to grab at this size. 
	camHeight 		= 720;
	
	vidGrabber.setVerbose(true);
	vidGrabber.initGrabber(camWidth,camHeight);

	ofSetWindowShape(vidGrabber.getWidth(), vidGrabber.getHeight());
	sharpness = 0;

	history = vector<float>(100, 0.0f);
	position = history.begin();

	ofEnableSmoothing();
}


//--------------------------------------------------------------
void testApp::update(){
	
	ofBackground(100,100,100);
	
	vidGrabber.grabFrame();
	
	if (vidGrabber.isFrameNew()){
		Mat input = toCv(vidGrabber);
		if (vidGrabber.getPixelsRef().getNumChannels() > 1) {
			//presume rgb
			cv::cvtColor(input, treated, CV_RGB2GRAY);
			cv::Sobel(input, treated, 3, 2, 2);
		} else
			cv::Sobel(input, treated, 3, 2, 2);
		
		cv::pyrDown(input, input);
		cv::Sobel(input, treated, 3, 2, 2, 3);
		cv::absdiff(treated, cv::Scalar(0.0f), treated);
		cv::Scalar sum = cv::sum(treated);
		this->sharpness = sqrt(sum[0]  / (vidGrabber.getWidth() * vidGrabber.getHeight()));

		if (++position == history.end())
			position = history.begin();

		*position = this->sharpness;
	}

}

//--------------------------------------------------------------
void testApp::draw(){

	ofSetColor(255);
	vidGrabber.draw(0,0);

	ofPushMatrix();
	ofScale(5.0f, 5.0f, 5.0f);
	string text = "sharpness = " + ofToString(sharpness);
	ofSetColor(40);
	ofRect(10,10,text.length() * 8 + 10, 20);
	ofSetColor(255);
	ofDrawBitmapString(text, 10 + 5, 25);
	ofPopMatrix();

	ofPushStyle();
	ofNoFill();
	ofBeginShape();
	ofSetLineWidth(3.0f);
	ofSetColor(200,100,100);
	for (int i=0; i<history.size(); i++) {
		float x = (float)i / (float)history.size() * ofGetWidth();
		float y = ofGetHeight() - history[i] * ofGetHeight() / 20.0f;
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

	if (key == 'v')
		vidGrabber.videoSettings();
	
	
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
