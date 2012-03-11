#pragma once
#include "ofxCv.h"

namespace HanRiverLib {
	class GlobalBoardFinder {
	public:
		static void init(float boardPitch, int countX, int countY);
		static ofxCv::Calibration boardFinder;
		static vector<cv::Point3f> objectPoints;
	};	
}