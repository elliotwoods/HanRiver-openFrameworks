#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxUeye.h"

#include "ThreadedCapture.h"

#define SELECTION_HEIGHT 50

using namespace ofxCv;
using namespace cv;

class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	void keyPressed(int key);
	void mouseMoved( int x, int y );
	void mousePressed( int x, int y, int button );

	void drawSelect();

	bool selecting;

	////
	//select mode
	////
	//
	int iSelection;
	vector<ofxUeyeDevice> devices;
	//
	////

	////
	//run mode
	////
	//
	ThreadedCapture camera;
	Mat lastFrame;
	Calibration calibration;
	//
	float frameDifference;
	float calmFrameDifferenceThreshold;
	float calmDurationThreshold;
	float lastMovement;
	//
	////
};