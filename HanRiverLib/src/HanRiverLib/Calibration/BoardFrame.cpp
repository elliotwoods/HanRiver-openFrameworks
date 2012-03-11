#include "BoardFrame.h"

namespace HanRiverLib {
	//----------
	BoardFrame::BoardFrame(const ofPixels & pixels) {
		this->pixels = pixels;
		this->success = false;
		this->complete = false;
		this->startThread(true, false);
	}

	//----------
	BoardFrame::BoardFrame(string filename) {
		this->load(filename);
	}

	//----------
	bool BoardFrame::isComplete() const {
		return this->complete;
	}

	//----------
	bool BoardFrame::isSuccess() const {
		return this->success;
	}

	//----------
	const vector<Point2f> & BoardFrame::getImagePoints() const{
		return this->imagePoints;
	}

	//----------
	void BoardFrame::threadedFunction() {
#pragma omp critical(ofLog)
		ofLogNotice() << "BoardFrame [" << this << "] began board find";
		this->success = GlobalBoardFinder::boardFinder.findBoard(toCv(pixels), this->imagePoints);
		pixels.clear();
		this->complete = true;
#pragma omp critical(ofLog)
		ofLogNotice() << "BoardFrame [" << this << "] completed board find [success=" << this->success << "]";
	}

	//----------
	void BoardFrame::load(string filename) {
		if (this->isThreadRunning())
			this->stopThread();

		ifstream file;
		file.open(filename, ios::binary);

		uint16_t pointCount;
		file.read((char*)&pointCount, sizeof(pointCount));
		this->imagePoints.resize(pointCount);
		file.read((char*)&this->imagePoints[0], sizeof(Point2f) * pointCount);

		file.close();
		this->success = true;
		this->complete = true;
	}

	//----------
	void BoardFrame::save(string filename) const {
		while (this->threadRunning)
			ofSleepMillis(10);

		ofstream file;
		file.open(filename, ios::binary);

		uint16_t pointCount = this->imagePoints.size();
		file.write((char*)&pointCount, sizeof(pointCount));
		file.write((char*)&this->imagePoints[0], sizeof(Point2f) * pointCount);

		file.close();
	}
}