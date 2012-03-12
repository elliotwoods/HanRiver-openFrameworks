#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	projector.setPosition(0,0,3);

	const ofVec3f lbf(-2.0f, -1.0f, -1.0f);
	const ofVec3f rtb(2.0f, 1.0f, 1.0f);
	for (int i=0; i<1000; i++)
		points.addVertex(ofVec3f(	ofRandom(lbf.x, rtb.x),
									ofRandom(lbf.y, rtb.y),
									ofRandom(lbf.z, rtb.z)));
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
	ofPushView();
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(projector.getProjectionMatrix().getPtr());
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(projector.getViewMatrix().getPtr());

	points.drawVertices();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	ofVec3f projected;
	ofNoFill();
	for (int i=0; i<points.getNumVertices(); i++) {
		projected = projector.projectPoint(points.getVertex(i));
		ofCircle(projected.x, projected.y, 0.01);
	}
	ofPopView();
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