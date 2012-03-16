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
	
		dst.x = dst.x * this->cameraMatrix.at<double>(0, 0) + this->cameraMatrix.at<double>(2, 0);
		dst.y = dst.y * this->cameraMatrix.at<double>(1, 1) + this->cameraMatrix.at<double>(2, 1);

		dst *= 2.0f;
		dst.x = dst.x - 1.0f;
		dst.y = 1.0f - dst.y;

		return dst;
	}
}