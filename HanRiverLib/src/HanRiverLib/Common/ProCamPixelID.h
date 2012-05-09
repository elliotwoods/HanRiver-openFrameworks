#pragma once
#include <stdint.h>

#include "ID.h"
#include "ofVec2f.h"

namespace HanRiverLib {

	struct ProCamPixelID {
		ProCamPixelID() { }
		ProCamPixelID(ProCamID proCam, PixelID pixelID) {
			this->proCam = proCam;
			this->pixelID = pixelID;
		}
		ProCamID proCam;
		PixelID pixelID;

		bool operator<(const ProCamPixelID & other) const;
	};

	struct CamPixelIDXY {
		CamPixelIDXY(CamID camera, ofVec2f xy) {
			this->camera = camera;
			this->xy = xy;
		}
		CamID camera;
		ofVec2f xy;
		bool operator<(const CamPixelIDXY & other) const;
	};

	typedef ProCamPixelID PPID;
	typedef ProCamPixelID CPID;
}