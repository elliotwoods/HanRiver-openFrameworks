#pragma once

#include <map>
#include "CameraHead.h"
#include "CameraPair.h"

#include "ofxTSP.h"

namespace HanRiverLib {
	typedef map<int, CameraHead*> HeadIt;

	class ExtrinsicsFinder {
	public:
		void solve(const map<int, CameraHead*> & cameraMap, const vector<CameraHead> & cameraVec);
		map<int, ofxTSP::Route > calibrationRoutes; //<camera index, route to camera>
		map<CameraPair, ofMatrix4x4> interCamExtrinsics;
	};
}