#pragma once
#include "GlobalBoardFinder.h"
#include "ofThread.h"

using namespace ofxCv;
using namespace cv;

namespace HanRiverLib {
	class BoardFrame : public ofThread {
	public:
		BoardFrame(const ofPixels & pixels);
		BoardFrame(string filename);

		bool isComplete() const;
		bool isSuccess() const;
		const vector<Point2f> & getImagePoints() const;

		void load(string filename);
		void save(string filename) const;

	protected:
		ofPixels pixels;
		void threadedFunction();
		vector<Point2f> imagePoints;
		bool success;
		bool complete;
	};
}