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
		ProCam(const ofMatrix4x4 & view, const ofMatrix4x4 & projection, int width, int height, const float * distortion);
		float * getDistortionPtr();
		const float * getDistortionPtr() const;
	protected:
		float distortion[5]; ///<Distortion defined by opencv's 5-double distortion parameters
	};
}