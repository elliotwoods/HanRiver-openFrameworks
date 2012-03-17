#pragma once

#include "ofxCv.h"
#include <fstream>

using namespace cv;

namespace HanRiverLib {
	struct CVIntrinsics {
		CVIntrinsics();
		CVIntrinsics(ofxCv::Calibration calibration);

		void saveTo(ofstream & file) const;
		void loadFrom(ifstream & file);

		Mat cameraMatrix;
		Mat undistortedCameraMatrix;
		Mat distortion;
		ofVec2f fov;
	};
}