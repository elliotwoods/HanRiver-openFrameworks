#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxUeye.h"

using namespace ofxCv;
using namespace cv;

#define SELECTION_HEIGHT 50

class testApp : public ofBaseApp{
	
	public:
		
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);		
		
	////
	//select mode
	////
	//
	bool selecting;
	void drawSelect();
	void scanCameras();
	void selectCamera(int iSelection);
	int iSelection;
	ofxUeyeDevice device;
	vector<ofxUeyeDevice> devices;
	//
	////

		ofxUeye camera;

		float sharpness;
		Mat treated;

		vector<float> history;
		vector<float>::iterator position;
		ofImage preview;
		bool showPreview;
		float magnification;
};
