#include "GlobalBoardFinder.h"

namespace HanRiverLib {
#ifdef HAS_BOARD_FINDER
ofxCv::Calibration GlobalBoardFinder::boardFinder = ofxCv::Calibration();
vector<cv::Point3f> GlobalBoardFinder::objectPoints = vector<cv::Point3f>();
#endif

	//---------
	void GlobalBoardFinder::init(float boardPitch, int countX, int countY) {
#ifdef HAS_BOARD_FINDER
		GlobalBoardFinder::boardFinder.setSquareSize(boardPitch);
		GlobalBoardFinder::boardFinder.setPatternSize(countX, countY);
		GlobalBoardFinder::objectPoints = ofxCv::Calibration::createObjectPoints(GlobalBoardFinder::boardFinder.getPatternSize(), boardPitch, ofxCv::CHESSBOARD);
#endif
	}
}