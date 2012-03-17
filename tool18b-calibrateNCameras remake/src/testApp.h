#pragma once
#include "ofMain.h"

#define HAVE_GUI

#ifdef HAVE_GUI
#include "ofxCvGui.h"
#endif


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

#ifdef HAVE_GUI
	void rebuildGUI();
	ofxCvGui::Builder gui;
#endif
	int captureID;
	CameraSet cameraSet;
};

//threaded board finder
//testApp can control captures
