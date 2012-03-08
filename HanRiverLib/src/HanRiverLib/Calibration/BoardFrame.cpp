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
		this->success = boardFinder.findBoard(toCv(pixels), this->imagePoints);
		pixels.clear();
		this->complete = true;
	}
}