#pragma once

#include <map>
#include "CameraHead.h"
#include "CameraPair.h"
#include "CameraSetContent.h"

#include "ofxTSP.h"

namespace HanRiverLib {
	class ExtrinsicsFinder {
	public:
		void solve(const CameraSetContent & cameraSet);
	protected:

		void findCommonImageSets();
		void defineJourneys();
		void findCalibrationRoutes();
		void selectExtrinsicsCalibrations();
		void calcExtrinsics();
		void calcExtrinsics(const CameraPair & cameraPair);
		void integrateExtrinsics();

		//input
		const CameraSetContent * imagePoints;
		vector<ofPtr<CameraHead> > camerasVector;
		map<uint16_t, int> indicesInVector;

		//products
		map<CameraPair, set<int> > commonImageSets; ///<NOTE: CameraPair's indices here are in camerasVector, not cameraID's!
		ofxTSP::Problem problem;
		map<uint16_t, ofxTSP::Route > calibrationRoutes; ///<camera ID, route to camera is in cameras vector id's
		map<CameraPair, ofMatrix4x4> interCamExtrinsics;///<CameraPair's indices are in camera ID here
	};
}