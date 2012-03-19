#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxCvGui.h"

#include "ofxGraycode.h"
#include "CameraHead.h"

using namespace ofxGraycode;

class testApp : public ofBaseApp {

public:
	~testApp();
	void setup();
	void update();
	void draw();

	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);


	vector<ofxUeye *> cameras;

	PayloadGraycode payload;
	ofxGraycode::Encoder encoder;
	vector<ofxGraycode::Decoder *> decoders;
	
	ofPixels pixels;
	ofTexture tex;
	int projector;
	bool frameSent;
};
