#pragma once
#include "ProCam.h"
#include "../Calibration/CameraSet.h"

#include <stdint.h>
#include <map>
#include <set>

namespace HanRiverLib {
	///Index in the map is cameraID.
	///and we presume that projectors have ID << 8
	class ProCamSet : public map<uint16_t, ProCam>, public ofNode {
	public:
		ProCamSet() { };
		ProCamSet(const CameraSetContent & cameraSet);

		void add(const CameraSetContent & cameraSet);

		void save(string filename="");
		void load(string filename="");

		void enforceXZPlane(); ///<rearranges cameras so they all approximately lie in xz plane
		void bakeTransform(); ///<Apply ProCamSet's transform to each procam

		int getCalibratedCameraCount();

	protected:
		void customDraw();
	};
}