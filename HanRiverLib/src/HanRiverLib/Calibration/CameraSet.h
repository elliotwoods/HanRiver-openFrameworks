#pragma once
#include "CameraSetContent.h"
#include "ExtrinsicsFinder.h"

using namespace std;

namespace HanRiverLib {
	class CameraSet : public CameraSetContent {
	public:
		~CameraSet();

		void openDevices(const vector<ofxUeyeDevice> & devices);
		void closeDevices();

		void capture(int captureID);

		void load();
		void save();

		void solveIntrinsics();
		void solveExtrinsics();
		void solveAndSaveCalibration();

		bool getAllHaveIntrinsics() const;
	};
}