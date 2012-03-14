#include "testApp.h"

//-------------
void testApp::setup() {
	ofSetVerticalSync(true);
	gui.init();
	gui.add(scene, "ProCamSet")->setCursorEnabled(true);
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
		scene.load();
		scene.enforceXZPlane();
	}
}