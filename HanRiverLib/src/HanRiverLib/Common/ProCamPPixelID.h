#pragma once
#include "ProCamPairID.h"
#include "ID.h"

#include <stdint.h>

namespace HanRiverLib {
	struct ProCamPPixelID {
		ProCamPairID pcid;
		PixelID projectorPixel;
		bool operator<(const ProCamPPixelID & other) const;
	};
	typedef ProCamPPixelID PCPID;

	struct CamPPixelID {
		CamID camera;
		PixelID projectorPixel;
		bool operator<(const CamPPixelID & other) const;
	}
	typedef CamPPixelID;
}