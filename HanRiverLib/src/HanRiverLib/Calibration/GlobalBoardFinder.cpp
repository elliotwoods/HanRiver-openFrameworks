#include "GlobalBoardFinder.h"

namespace HanRiverLib {
	ofxCv::Calibration GlobalBoardFinder::boardFinder = ofxCv::Calibration();
	vector<cv::Point3f> GlobalBoardFinder::objectPoints = vector<cv::Point3f>();

	//---------
	void GlobalBoardFinder::init(float boardPitch, int countX, int countY) {
		GlobalBoardFinder::boardFinder.setSquareSize(boardPitch);
		GlobalBoardFinder::boardFinder.setPatternSize(countX, countY);
		GlobalBoardFinder::objectPoints = ofxCv::Calibration::createObjectPoints(GlobalBoardFinder::boardFinder.getPatternSize(), boardPitch, ofxCv::CHESSBOARD);
	}
}