#pragma once
#include "ofMain.h"
#include "ofxUeyeThreaded.h"
#include "ofxUeyePreset_5480SL.h"
#include "ofxGraycode.h"
#include "ofxCvGui.h"

#include <map>

using namespace ofxGraycode;
using namespace ofxCvGui;

class CameraHead {
public:
	CameraHead();
	CameraHead(int cameraID, const Payload& payload);
	~CameraHead();

	void init(int cameraID, const Payload& payload);
	void update();
	
	int getCameraID();
	void setProjectorID(int projector);
	int getProjectorID() const;

	ofxUeye& getCamera();
	Decoder& getDecoder();

	bool capture(); //returns true on complete
	void clear();
	void moveThreshold(int distance);

	void load();
	void save();
	void saveCurrent();

protected:
	ofxUeye camera;
	map<int, Decoder> decoders;
	const Payload* payload;
	int projector;

	bool initialised;
};