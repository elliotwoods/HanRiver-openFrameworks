#pragma once
#include "ofxCv.h"

#define HAS_BOARD_FINDER

namespace HanRiverLib {
	class GlobalBoardFinder {
	public:
		static void init(float boardPitch, int countX, int countY);
#ifdef HAS_BOARD_FINDER
		static ofxCv::Calibration boardFinder;
		static vector<cv::Point3f> objectPoints;
#endif
	};	
}