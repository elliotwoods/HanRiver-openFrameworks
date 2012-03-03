#pragma once
#include <set>

#include "ofxCv.h"
#include "ofThread.h"

#define PATTERN_SIZE 9, 6
#define SQUARE_SIZE 0.04

using namespace ofxCv;
///locking on addBuffer
class CameraSolver : public ofThread {
public:
	CameraSolver();
	~CameraSolver();
	void add(int currentCapture, const ofPixels& pixels, const ofPixels& preview);
	float getReprojectionError();
	const vector<ofPixels>& getPreviews() const;
	const vector<ofPixels>& getPixels() const;
	void saveCalibration(string filename);
	void savePixels(int cameraID);

	Calibration& lockCalibration();
	void unlockCalibration();
	void calibrate();

	bool hasCapture(int captureIndex);
	vector<Point2f> getCapture(int captureIndex);

protected:
	void threadedFunction();

	vector<ofPixels> pixels;
	vector<ofPixels> previews;
	vector<int> captureIndices; ///<this solver has a subset of all images. the indices in the main set available locally are stored here
	
	ofxCv::Calibration calibration;

	map<int, ofPixels> addBuffer;

	Poco::Mutex calibrationLock;
};