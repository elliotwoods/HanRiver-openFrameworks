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
	void loadFolder();

	ofxCvGui::Builder gui;
	ProCamSet proCamSet;
	ProjectorPixelSet pointSet;
};