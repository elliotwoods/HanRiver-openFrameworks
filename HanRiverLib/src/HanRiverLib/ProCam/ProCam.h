#pragma once

#include "ofxRay.h"
#include "ofxCv.h"
#include "../Calibration/CVIntrinsics.h"
#include "../Calibration/CameraHead.h"
#include "../Common/ID.h"

using namespace ofxCv;
using namespace cv;
using namespace ofxRay;

namespace HanRiverLib {
	class ProCam : public Projector {
	public:
		ProCam(const CameraHead & calibratedCamera);
		ProCam(const CVIntrinsics & intrinsics, const ofMatrix4x4 & extrinsics, const cv::Size & imageSize);
		ProCam(const ofMatrix4x4 & view, const ofMatrix4x4 & projection, int width, int height, const CVIntrinsics & intrinsics);

		ofVec2f undistort(ofVec2f & cameraXY) const;

		CVIntrinsics intrinsics;
	};
}