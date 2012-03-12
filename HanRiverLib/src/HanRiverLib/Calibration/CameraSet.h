#pragma once
#include "CameraSetContent.h"
#include "ExtrinsicsFinder.h"

using namespace std;

namespace HanRiverLib {
	class CameraSet : public CameraSetContent {
	public:
		~CameraSet();

		void openAllDevices();
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