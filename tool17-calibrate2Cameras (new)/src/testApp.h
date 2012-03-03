#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxUeyeThreaded.h"
#include "ofxCvGui.h"

#include "CameraHead.h"

#define SELECTION_HEIGHT 50
#define MIN_CAPTURES 3
#define ADD_FREQUENCY 3.0f

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
	void solveExtrinsicsAndSave();

	void savePixels();

	ofxCvGui::Builder gui;
	vector<ofPtr<CameraHead> > cameras;
	map<uint32_t, ofMatrix4x4> extrinsics; //camera ID + cameraID << 16
	ofSoundPlayer pop;
	float lastAdd;
	int captureCount;
};