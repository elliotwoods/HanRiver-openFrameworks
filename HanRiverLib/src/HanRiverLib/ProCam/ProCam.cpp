#include "ProCam.h"

namespace HanRiverLib {
	//---------
	ProCam::ProCam(const CameraHead & calibratedCamera) {
		*this = ProCam( calibratedCamera.getIntrinsics(), calibratedCamera.getDistortion(), calibratedCamera.getExtrinsics(), calibratedCamera.getImageSize() );
	}

	//---------
	ProCam::ProCam(const Intrinsics & intrinsics, const Mat distortion, const ofMatrix4x4 & extrinsics, const cv::Size & imageSize) {
		ofMatrix4x4 projection;// = intrinsics.getProjectionMatrix(0.01, 6.0f);
		projection.makePerspectiveMatrix(intrinsics.getFov().y, intrinsics.getFov().x / intrinsics.getFov().y, 0.01, 10.0f);
		cout << "Camera matrix : " << endl;
		cout << projection << endl << endl;
		this->setProjection(projection);
		
		this->setTransformMatrix(extrinsics);
		this->setWidth(imageSize.width);
		this->setHeight(imageSize.height);

		this->cameraMatrix = intrinsics.getCameraMatrix();
		this->distortion = distortion;
	}

	//---------
	ProCam::ProCam(const ofMatrix4x4 & view, const ofMatrix4x4 & projection, int width, int height, const Mat cameraMatrix, const Mat distortion) {
		this->setTransformMatrix(view);
		this->setProjection(projection);
		this->setWidth(width);
		this->setHeight(height);
		this->cameraMatrix = cameraMatrix;
		this->distortion = distortion;
	}

	//---------
	ofVec2f ProCam::undistort(ofVec2f & cameraXY) const {
		ofVec2f dst;
		Mat matSrc = Mat(1, 1, CV_32FC2, &cameraXY.x);
		Mat matDst = Mat(1, 1, CV_32FC2, &dst.x);
		undistortPoints(matSrc, matDst, Mat(this->cameraMatrix), Mat(this->distortion));
		return dst;
	}
}