#include "CameraSolver.h"

//---------
CameraSolver::CameraSolver() {	
	this->calibration.setSquareSize(SQUARE_SIZE);
	this->calibration.setPatternSize(PATTERN_SIZE);
	this->startThread(true, false);
}

//---------
CameraSolver::~CameraSolver() {
	this->stopThread(false);
}

//---------
void CameraSolver::add(const ofPixels& pixels, const ofPixels& preview) {
	this->lock();
	this->toAdd.push_back(pixels);
	this->previews.push_back(preview);
	this->unlock();
}

//---------
float CameraSolver::getReprojectionError() {
	return this->calibration.getReprojectionError();
}

//---------
const vector<ofPixels>& CameraSolver::getPreviews() const {
	return this->previews;
}

//---------
void CameraSolver::saveCalibration(string filename) {
	this->calibration.save(filename);
}

//---------
void CameraSolver::threadedFunction() {
	while (isThreadRunning()) {
		this->lock();
		if (toAdd.size() > 0) {
			for (int i=0; i<toAdd.size(); i++)
				pixels.push_back(toAdd[i]);
			toAdd.clear();
		}
		this->unlock();
		int countAdded = calibration.imagePoints.size();
		if (countAdded < pixels.size()) {
			calibration.add(toCv(pixels[countAdded]));
			found.push_back(calibration.imagePoints[countAdded].size() > 0);
		}
		ofSleepMillis(20);
	}
}