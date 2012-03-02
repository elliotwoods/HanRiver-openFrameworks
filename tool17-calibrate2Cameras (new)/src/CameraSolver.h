#pragma once
#include "ofxCv.h"
#include "ofThread.h"

#define PATTERN_SIZE 9, 6
#define SQUARE_SIZE 0.04

using namespace ofxCv;
class CameraSolver : ofThread {
public:
	CameraSolver();
	~CameraSolver();
	void add(const ofPixels& pixels, const ofPixels& preview);
	float getReprojectionError();
	const vector<ofPixels>& getPreviews() const;
	void saveCalibration(string filename);

protected:
	void threadedFunction();
	vector<ofPixels> pixels;
	vector<ofPixels> previews;
	vector<bool> found;
	ofxCv::Calibration calibration;
private:
	vector<ofPixels> toAdd;
};