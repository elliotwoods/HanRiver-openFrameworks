#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxUeyeThreaded.h"
#include "ofxCvGui.h"

#include "CameraHead.h"

#define SELECTION_HEIGHT 50
#define MIN_CAPTURES 3

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

	void add();
	ofxCvGui::Builder gui;
	vector<ofPtr<CameraHead> > cameras;

	ofSoundPlayer pop;
};