#include "testApp.h"

testApp::testApp() :
wireSet(ofVec3f(4,2,3), ofVec2f(0,2), 1000, 4) {
}
//--------------------------------------------------------------
void testApp::setup(){
	ofBackground(50, 50, 50);
	ofEnableSmoothing();
	camera.setFixUpwards(false);

	projector.position = ofVec3f(0,0,-4);
	projector.aspectRatio = 4.0f / 3.0f;
	projector.throwRatio = 2.0f;
	projector.rotation.makeRotate(180, 0, 1, 0);
	
	wireSet.viewpoint = projector.position;
	wireSet.startThread(true, false);
	
	viewport[0] = ofRectangle(0,0,ofGetWidth()/2,ofGetHeight());
	viewport[1] = ofRectangle(ofGetWidth()/2,0,ofGetWidth()/2,ofGetHeight()/2);
	viewport[2] = ofRectangle(ofGetWidth()/2,ofGetHeight()/2,ofGetWidth()/2,ofGetHeight()/2);
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
	
	ofPushView();
	ofViewport(viewport[0]);
	camera.begin();
	drawScene();
	camera.end();
	ofPopView();
	
	ofPushView();
	ofViewport(viewport[1]);
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(projector.getProjectionMatrix().getPtr());
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(projector.getViewMatrix().getPtr());
	drawScene();
	ofPopView();
	
	ofPushView();
	ofViewport(viewport[2]);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	ofScale(1.0f, ofGetWidth() / ofGetHeight());
	ofRotate(90, 1, 0, 0);
	//ofScale(2.0f / 4.0f, 2.0f / 4.0f, 2.0f / 4.0f);
	ofPopView();
	
	ofPushStyle();
	ofNoFill();
	for (int i=0; i<3; i++)
		ofRect(viewport[i]);
	ofPopStyle();
	
	stringstream ss;
	ss << "Wires = " << wireSet.wires.size() << endl;
	ss << "Elements = " << wireSet.elements.size() << endl;
	ss << "Radius = " << wireSet.radius << endl;
	ofDrawBitmapString(ss.str(), 10, 20);
}

//--------------------------------------------------------------
void testApp::drawScene() {
	ofDrawGrid();
	wireSet.draw();
	projector.draw();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if (key=='c')
		camera.toggleCursorDraw();
	if (key==OF_KEY_UP)
		projector.position.z += 0.1;
	if (key==OF_KEY_DOWN)
		projector.position.z -= 0.1;
	if (key==OF_KEY_LEFT)
		projector.position.x -= 0.1;
	if (key==OF_KEY_RIGHT)
		projector.position.x += 0.1;
}