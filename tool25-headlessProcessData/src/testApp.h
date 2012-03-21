#pragma once
#include "ofMain.h"

#include "ofxCvGui.h"
#include "ofxGraycode.h"

#include "HanRiverLib.h"

using namespace HanRiverLib;

class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	void keyPressed(int key);
	void loadProCamSet(string filename = "");
	void loadFolder();

	ofxCvGui::Builder gui;
	ofxGrabCam * camera;
	ProCamSet proCamSet;
	ProjectorPixelSet pointSet;

};