#pragma once

#include "ofxRay.h"
#include "ofxCv.h"
#include "../Calibration/CameraHead.h"

using namespace ofxCv;
using namespace cv;
using namespace ofxRay;

namespace HanRiverLib {
	class ProCam : public Projector {
	public:
		ProCam(const CameraHead & calibratedCamera);
		ProCam(const Intrinsics & intrinsics, const Mat distortion, const ofMatrix4x4 & extrinsics, const cv::Size & imageSize);
		ProCam(const ofMatrix4x4 & view, const ofMatrix4x4 & projection, int width, int height, const Mat cameraMatrix, const Mat distortion);

		ofVec2f undistort(ofVec2f & cameraXY) const;
		Mat cameraMatrix;
		Mat distortion;
	};
}