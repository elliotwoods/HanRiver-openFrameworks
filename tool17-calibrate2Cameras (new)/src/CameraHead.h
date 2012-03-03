#pragma once
#include "ofxUeyeThreaded.h"
#include "ofxUeyePreset_5480ChessBoard.h"
#include "ofxCv.h"
#include "ofMain.h"

#include "CameraSolver.h"

#include "ofxCvGui/Assets.h"

#define MAX_DOWNSAMPLED_RES 1024

using namespace ofxCv;
using namespace cv;
using namespace ofxCvGui;

class CameraHead : ofThread, public ofBaseDraws {
public:
	CameraHead(const ofxUeyeDevice& device);
	~CameraHead();

	void add(int currentCapture);
	void save();
	const ofPixels& getPixelsRef() const;
	const ofxUeye& getCamera() const;
	const vector<ofPixels>& getCaptures() const;
	bool isFound() const { return this->found; }

	//external calibration
	CameraSolver& getSolver();

	//ofBaseDraws
	void draw(float x, float y);
	void draw(float x, float y, float width, float height);
	float getWidth();
	float getHeight();

	void savePixels();

protected:
	void threadedFunction();
	void makePreview(bool lock);
	void capture();

	ofxUeye camera;
	ofPixels raw;
	ofPixels downSampled; ///<downSampled is used for preview and for quick check to find boards
	vector<Point2f> previewCorners;
	bool found;

	CameraSolver captures;

	ofTexture preview;
	bool needsUpdate;
};