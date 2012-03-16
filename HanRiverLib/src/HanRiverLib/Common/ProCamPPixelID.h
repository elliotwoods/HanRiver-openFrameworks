#pragma once
#include "ProCamID.h"
#include "ID.h"

#include <stdint.h>

namespace HanRiverLib {
	struct ProCamPPixelID {
		ProCamID pcid;
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