#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxUeye.h"

using namespace ofxCv;
using namespace cv;

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
		
		ofxUeye camera;

		float sharpness;
		Mat treated;

		vector<float> history;
		vector<float>::iterator position;
		ofImage preview;
		bool showPreview;
};
