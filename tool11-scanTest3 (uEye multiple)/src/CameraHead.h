#pragma once
#include "ofMain.h"
#include "ofxUeyeThreaded.h"
#include "ofxUeyePreset_5480SL.h"
#include "ofxGraycode.h"

using namespace ofxGraycode;

class CameraHead {
public:
	CameraHead();
	~CameraHead();
	CameraHead(int cameraID, const Payload& payload);
	void init(int cameraID, const Payload& payload);
	void update();
	int getID();
	ofxUeyeThreaded& getCamera();
	Decoder& getDecoder();

	void capture();
	void clear();

protected:
	ofxUeyeThreaded camera;
	Decoder decoder;
	const Payload* payload;
};