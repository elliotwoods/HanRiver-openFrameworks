#pragma once
#include "ofMain.h"
#include "ofxCvGui.h"

#include "HanRiverLib.h"

//minimum number of matching pairs between a stereo set
#define MINPAIRCOUNT 6

using namespace HanRiverLib;

class testApp : public ofBaseApp {
public:
	~testApp();
	void setup();
	void update();
	void draw();
	void keyPressed(int key);
	void mouseMoved( int x, int y );
	void mousePressed( int x, int y, int button );

	void rebuildGUI();

	ofxCvGui::Builder gui;
	int captureID;
	CameraSet cameraSet;
	ProCamSet proCamSet;
};

//threaded board finder
//testApp can control captures
