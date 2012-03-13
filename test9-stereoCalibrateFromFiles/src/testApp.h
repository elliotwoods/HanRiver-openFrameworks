#pragma once

#include "ofMain.h"
#include "BoardFinder.h"

#include <set>

struct CameraCameraSet {
	uint16_t camera;
	uint16_t projector;
};
class testApp : public ofBaseApp {

public:
	void setup();
	void setPath(int camera);
	void loadPixels(int camera);
	
	void update();
	void draw();
	void drawCapture(int camera);
	
	void calibrate();
	void addCalibrationPoints(int camera, string filename);
	
	void keyPressed(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void windowResized(int w, int h);
	
protected:
	bool ready;
	ofDirectory files[2];
	map<string, ofPtr<BoardFinder> > captures[2];
	ofxCv::Calibration intrinsics[2];
	float error[2];
	
	set<string> filenames;
	set<string>::iterator current;
	
	ofRectangle viewports[2];
};
