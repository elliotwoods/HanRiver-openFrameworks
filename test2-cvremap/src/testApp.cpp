#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofSetWindowShape(1280, 480);
	ofSetLogLevel(OF_LOG_NOTICE);

	video.initGrabber(640, 480, true);
	imitate(mapped, video);
	mapped.update();
	mapped.getPixelsRef().set(2,0);
	//take a video input image, and remap a central 
	//set of pixels to fill the whole image

	for (int j=0; j<640; j++)
		for (int i=0; i<480; i++) {
			source.push_back(ofVec2f(i, j));
			target.push_back(ofVec2f(i, j));
		}
	ready = false;
}

//--------------------------------------------------------------
void testApp::update(){
	if (ready)  {
		Mat matMapped = toCv(mapped);
		cv::remap(toCv(video), matMapped, remapx, remapy, CV_INTER_LINEAR, BORDER_CONSTANT, Scalar(0,0,0));
	}
	mapped.update();
	video.update();
}

//--------------------------------------------------------------
void testApp::draw(){
	video.draw(0,0);
	mapped.draw(640,0);

	if (!ready) {
		glBegin(GL_POINTS);
		for (int i=0; i<source.size(); i++)
			glVertex2f(source[i].x, source[i].y);
	

		for (int i=0; i<target.size(); i++)
			glVertex2f(target[i].x + 640, target[i].y);
		glEnd();
	}
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	remapx = Mat(640,480, CV_32FC1);
	remapy = Mat(640,480, CV_32FC1);
	for (int i=0; i<source.size(); i++) {
		remapx.at<float>(source[i].y, source[i].x) = target[i].x;
		remapy.at<float>(source[i].y, source[i].x) = target[i].y;
	}
	ready = true;
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