#pragma once

#include "ofMain.h"

#include "ofxGrabCam.h"
#include "ofxRay.h"
#include "WireSet.h"

class testApp : public ofBaseApp{

public:
	testApp();
	void setup();
	void update();
	void draw();
	void drawScene();
	
	void keyPressed  (int key);
	
	void addWire();

	ofxGrabCam camera;
	
	WireSet wireSet;
	ofProjector projector;
	ofRectangle viewport[3];
};
