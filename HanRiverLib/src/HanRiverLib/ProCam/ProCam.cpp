#include "ProCam.h"

namespace HanRiverLib {
	//---------
	ProCam::ProCam(const CameraHead & calibratedCamera) {
		*this = ProCam( calibratedCamera.getIntrinsics(), calibratedCamera.getDistortion(), calibratedCamera.getExtrinsics(), calibratedCamera.getImageSize() );
	}

	//---------
	ProCam::ProCam(const Intrinsics & intrinsics, const Mat distortion, const ofMatrix4x4 & extrinsics, const cv::Size & imageSize) {
		ofMatrix4x4 projection = intrinsics.getProjectionMatrix(0.01, 6.0f);
		cout << "Camera matrix : " << endl;
		cout << projection << endl << endl;
		this->setProjection(projection);

		for (int i=0; i<5; i++)
			this->distortion[i] = distortion.at<double>(i);
		
		this->setTransformMatrix(extrinsics);
		this->setWidth(imageSize.width);
		this->setHeight(imageSize.height);
	}

	//---------
	ProCam::ProCam(const ofMatrix4x4 & view, const ofMatrix4x4 & projection, int width, int height, const float * distortion) {
		this->setTransformMatrix(view);
		this->setProjection(projection);
		this->setWidth(width);
		this->setHeight(height);
		memcpy(this->distortion, distortion, sizeof(float) * 5);
	}

	//---------
	float * ProCam::getDistortionPtr() {
		return this->distortion;
	}

	//---------
	const float * ProCam::getDistortionPtr() const {
		return this->distortion;
	}
}