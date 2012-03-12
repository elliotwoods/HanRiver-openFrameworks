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

		//input
		const CameraSetContent * imagePoints;
		vector<ofPtr<CameraHead> > camerasVector;

		//products
		map<CameraPair, set<int> > commonImageSets;
		ofxTSP::Problem problem;
		map<int, ofxTSP::Route > calibrationRoutes; //<camera index, route to camera>
		map<CameraPair, ofMatrix4x4> interCamExtrinsics;
	};
}