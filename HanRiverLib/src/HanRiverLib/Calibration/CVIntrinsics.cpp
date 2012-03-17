#include "CVIntrinsics.h"

namespace HanRiverLib {
	//-----------
	CVIntrinsics::CVIntrinsics() {
		this->cameraMatrix = Mat(3, 3, CV_64F);
		this->undistortedCameraMatrix = Mat(3, 3, CV_64F);
		this->distortion = Mat(5, 1, CV_64F);
	}

	//-----------
	CVIntrinsics::CVIntrinsics(ofxCv::Calibration calibration) {
		this->cameraMatrix = calibration.getDistortedIntrinsics().getCameraMatrix();
		this->undistortedCameraMatrix = calibration.getUndistortedIntrinsics().getCameraMatrix();
		this->distortion = calibration.getDistCoeffs();
		this->fov.x = calibration.getDistortedIntrinsics().getFov().x;
		this->fov.y = calibration.getDistortedIntrinsics().getFov().y;
	}

	//-----------
	void CVIntrinsics::saveTo(ofstream & file) const {
		file.write( (char*) this->cameraMatrix.data, sizeof(double) * 9);
		file.write( (char*) this->undistortedCameraMatrix.data, sizeof(double) * 9);
		file.write( (char*) this->distortion.data, sizeof(double) * 5);
		file.write( (char*) & this->fov, sizeof(float) * 2);
	}

	//-----------
	void CVIntrinsics::loadFrom(ifstream & file) {
		file.read( (char*) this->cameraMatrix.data, sizeof(double) * 9);
		file.read( (char*) this->undistortedCameraMatrix.data, sizeof(double) * 9);
		file.read( (char*) this->distortion.data, sizeof(double) * 5);
		file.read( (char*) & this->fov, sizeof(float) * 2);
	}
}