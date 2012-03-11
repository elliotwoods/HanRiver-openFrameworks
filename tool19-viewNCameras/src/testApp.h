#pragma once
#include "ofMain.h"
#include "ofxCvGui.h"
#include "ofxUeyeThreaded.h"
#include "ofxUeyePreset_5480Chessboard.h"


class testApp : public ofBaseApp {
public:
	~testApp();
	void setup();
	void update();
	void draw();
	vector<ofxUeyeThreaded*> cameras;
	ofxCvGui::Builder gui;
};