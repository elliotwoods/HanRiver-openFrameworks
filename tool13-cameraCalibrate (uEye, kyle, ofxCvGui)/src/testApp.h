#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxUeyeThreaded.h"
#include "ofxUeyePreset_5480ChessBoard.h"

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

	void drawSelect();


	////
	//select mode
	////
	//
	bool selecting;
	void scanCameras();
	void selectCamera(int iSelection);
	int iSelection;
	vector<ofxUeyeDevice> devices;
	//
	////

	////
	//run mode
	////
	//
	void addFrame();
	//
	bool active;
	ofxUeyeDevice device;
	ofxUeyeThreaded camera;
	Mat thisFrame, lastFrame;
	Calibration calibration;
	//
	float frameDifference;
	float calmFrameDifferenceThreshold;
	float calmDurationThreshold;
	float lastMovement;
	bool waitingForMovement;
	//
	bool saved;
	//
	////
};