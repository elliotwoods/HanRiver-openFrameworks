#include "testApp.h"

//-------------
void testApp::setup() {
	ofSetVerticalSync(true);
	gui.init();
	gui.add(scene);
	scene.load();
}

//-------------
void testApp::update() {
}

//-------------
void testApp::draw() {
}

//-------------
void testApp::keyPressed(int key) {
	scene.load();
}