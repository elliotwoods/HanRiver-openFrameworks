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
void CameraSolver::add(int currentCapture, const ofPixels& pixels, const ofPixels& preview) {
	if (!pixels.isAllocated())
		return;

	this->lock();
	this->addBuffer.insert(pair<int, ofPixels>(currentCapture, pixels));
	this->unlock();
	this->previews.push_back(preview);
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
const vector<ofPixels>& CameraSolver::getPixels() const {
	return this->pixels;
}

//---------
void CameraSolver::saveCalibration(string filename) {
	this->lockCalibration();
	this->calibration.save(filename);
	this->unlockCalibration();
}

//---------
void CameraSolver::savePixels(int cameraID) {
	this->lock();
	for (int i=0; i<pixels.size(); i++)
		ofSaveImage(pixels[i], "c" + ofToString(cameraID) + "-" + ofToString(i) + ".png");
}
//---------
Calibration& CameraSolver::lockCalibration() {
	this->calibrationLock.lock();
	return this->calibration;
}

//---------
void CameraSolver::unlockCalibration() {
	this->calibrationLock.unlock();
}

//---------
vector<Point2f> CameraSolver::getCapture(int captureIndex) {
	for (int i=0; i<captureIndices.size(); i++)
		if (captureIndices[i] == captureIndex)
			if (i > this->calibration.imagePoints.size())
				return vector<Point2f>(0);
			else
				return this->calibration.imagePoints[i];
	return vector<Point2f>(0);
}

//---------
void CameraSolver::threadedFunction() {
	while (isThreadRunning()) {
		this->lock();
		if (addBuffer.size() > 0) {
			map<int, ofPixels>::iterator it;
			for (it = addBuffer.begin(); it != addBuffer.end(); it++) {
				captureIndices.push_back(it->first);
				pixels.push_back(it->second);
				//ofImage saver;
				//saver.allocate(it->second.getWidth() / 3, it->second.getHeight(), OF_IMAGE_COLOR);
				//for (int y=0; y<saver.getHeight(); y++)
				//	memcpy(saver.getPixels() + int(saver.getWidth()) * 3 * y, it->second.getPixels() + int(it->second.getWidth()) * y, saver.getWidth() * 3);
				//saver.saveImage(ofToString(ofGetFrameNum()) + ".png");
			}
			addBuffer.clear();
		}
		this->unlock();
		ofSleepMillis(20);
	}
}

//---------
void CameraSolver::calibrate() {
	this->lock();
	this->lockCalibration();
	set<int> toRemove;
	int toAdd;
	if (calibration.imagePoints.size() < pixels.size()) {
		while (calibration.imagePoints.size() < pixels.size()) {
			toAdd = calibration.imagePoints.size();
			if (!calibration.add(toCv(pixels[toAdd])))
				toRemove.insert(toAdd);
		}
	}

	if (toRemove.size() != 0) {
		set<int>::iterator it;
		for (it = toRemove.end(); ;) {
			it--;

			pixels.erase(pixels.begin() + *it);
			previews.erase(previews.begin() + *it);
			captureIndices.erase(captureIndices.begin() + *it);

			if (it == toRemove.begin())
				break;
		}
	}


	calibration.calibrate();
	this->unlockCalibration();
	this->unlock();
}