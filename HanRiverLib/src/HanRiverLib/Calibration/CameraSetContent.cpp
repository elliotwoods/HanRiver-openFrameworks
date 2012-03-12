#include "CameraSetContent.h"

namespace HanRiverLib {
	//----------
	vector<ofPtr<CameraHead> > CameraSetContent::getCamerasAsVector() const {
		vector<ofPtr<CameraHead> > result;
		result.reserve(cameraIndices.size());
		vector<uint16_t>::const_iterator it;
		for (it = cameraIndices.begin(); it != cameraIndices.end(); it++)
			result.push_back(this->cameraMap.at(*it));
		return result;
	}

	//----------
	const CameraHead & CameraSetContent::getCameraByID(uint16_t cameraID) const {
		return *(this->cameraMap.at(cameraID));
	}

	//----------
	const CameraHead & CameraSetContent::getFirstCamera() const {
		return *(this->cameraMap.begin()->second);
	}

	//----------
	cv::Size CameraSetContent::getImageSize() const {
		return this->getFirstCamera().getImageSize();
	}

	//----------
	void CameraSetContent::setCameraExtrinsics(uint16_t cameraID, const ofMatrix4x4 & transform) const {
		this->cameraMap.at(cameraID)->setExtrinsics(transform);
	}
	//----------
	size_t CameraSetContent::size() const {
		return this->cameraIndices.size();
	}
}