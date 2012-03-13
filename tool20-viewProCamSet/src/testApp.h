#pragma once
#include "ofMain.h"
#include "ofxCvGui.h"
#include "HanRiverLib.h"


class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	void keyPressed(int key);

	ofxCvGui::Builder gui;
	HanRiverLib::ProCamSet scene;
};