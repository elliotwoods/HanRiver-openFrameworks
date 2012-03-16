#pragma once
#include "CameraHead.h"
#include "ofUtils.h"

#include <stdint.h>
#include "CameraSetContent.h"

#include <map>
#include <vector>

using namespace std;

namespace HanRiverLib {
	class CameraSetContent {
	public:
		vector<ofPtr<CameraHead> > getCamerasAsVector() const;
		const CameraHead & getCameraByID(uint16_t cameraID) const; 
		const CameraHead & getFirstCamera() const;
		cv::Size getImageSize() const;
		void setCameraExtrinsics(uint16_t cameraID, const ofMatrix4x4 & transform) const;

		size_t size() const;
		uint16_t getMaxIndex() const;

	protected:
		map<uint16_t, ofPtr<CameraHead> > cameraMap;
		vector<uint16_t> cameraIndices;
	};

	typedef map<uint16_t, ofPtr<CameraHead> >::iterator CameraSetIterator;
	typedef map<uint16_t, ofPtr<CameraHead> >::const_iterator CameraSetConstIterator;

}