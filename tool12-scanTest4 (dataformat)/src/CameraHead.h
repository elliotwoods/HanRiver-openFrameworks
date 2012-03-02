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
	~CameraHead();
	CameraHead(int cameraID, const Payload& payload, Builder& gui);
	CameraHead(int cameraID, const Payload& payload);

	void init(int cameraID, const Payload& payload, Builder& gui);
	void init(int cameraID, const Payload& payload);
	void update();
	
	int getCameraID();
	void setProjectorID(int projector);
	int getProjectorID() const;

	ofxUeyeThreaded& getCamera();
	Decoder& getDecoder();

	void capture();
	void clear();
	void moveThreshold(int distance);

	void load();
	void save();

protected:
	ofxUeyeThreaded camera;
	map<int, Decoder> decoders;
	const Payload* payload;
	int projector;

	ofxCvGui::Builder *gui;
	PanelPtr captureSetPanel;
	bool initialised;
};