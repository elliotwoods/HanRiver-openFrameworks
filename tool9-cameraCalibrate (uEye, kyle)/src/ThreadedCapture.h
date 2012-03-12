#pragma once
#include "ofMain.h"
#include "ofxUeye.h"

#define PIXELCLOCK 96
class ThreadedCapture : public ofThread, public ofBaseHasPixels {
public:
	ThreadedCapture();
	void update();
	void draw(int x, int y); //thread safe
	void draw(int x, int y, int width, int height);

	void open(ofxUeyeDevice& device);
	void close();

	ofPixels& getPixelsRef(); ///<remember to lock if you use this!
	unsigned char * getPixels(); ///<remember to lock if you use this!
	int getWidth();
	int getHeight();
	ofxUeye& getCamera();

	bool isReady() const;
	bool isFrameNew() const;
protected:
	void threadedFunction();
	ofxUeye camera;
	ofxUeyeDevice device;
	ofImage secondBuffer;
	bool ready;
	bool frameNew;
	bool hasNewFrame;
};