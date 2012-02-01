#pragma once

#include "ofMain.h"

#include "ofxGrabCam.h"
#include "ofxRay.h"

class testApp : public ofBaseApp{

public:
	void setup();
	void update();
	void draw();
	
	void keyPressed  (int key);

	void addElement();
	
	ofxGrabCam camera;
	
	vector<Element> elements;
	vector<ofVec2f> wires;
	float radius;
};
