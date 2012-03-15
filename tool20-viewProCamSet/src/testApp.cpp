#include "testApp.h"

class Bounds : public ofNode {
protected:
	void customDraw() {
		ofPushStyle();
		ofNoFill();
		ofSetLineWidth(3.0f);
		ofPushMatrix();
		ofTranslate(3.6f, -2.0f, -2.15);
		ofScale(4.0f, 2.0f, 2.0f);
		ofBox(1.0f);
		ofPopMatrix();
		ofPopStyle();
	}
};
//-------------
void testApp::setup() {
	ofSetVerticalSync(true);
	gui.init();
	ofPtr<ofxCvGui::Panels::Node> panel = gui.add(scene, "ProCamSet");
	panel->setCursorEnabled(true);
	panel->push(*(new Bounds()));
	this->keyPressed(' ');
}

//-------------
void testApp::update() {
}

//-------------
void testApp::draw() {
}

//-------------
void testApp::keyPressed(int key) {
	if (key == ' ') {
		scene.resetTransform();
		scene.load();
		scene.enforceXZPlane();
		scene.rotate(180.0f, 1.0f, 0.0f, 0.0f);
	}
}