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
	protected:
		void findCommonImageSets(const vector<CameraHead> & cameraVec);
		void defineJourneys(const vector<CameraHead> & cameraVec);
		void findCalibrationRoutes(const map<int, CameraHead*> cameraMap);
		void selectExtrinsicsCalibrations();
		void calcExtrinsics(const map<int, CameraHead*> cameraMap);
		void calcExtrinsics(const map<int, CameraHead*> cameraMap, const CameraPair & cameraPair);

		ofxTSP::Problem problem;
		map<CameraPair, set<int> > commonImageSets;
		map<int, ofxTSP::Route > calibrationRoutes; //<camera index, route to camera>
		map<CameraPair, ofMatrix4x4> interCamExtrinsics;
	};
}