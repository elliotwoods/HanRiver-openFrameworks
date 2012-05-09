#pragma once
#include "ofFileUtils.h"

#include <stdint.h>
#include <string>

namespace HanRiverLib {
	struct ProCamPairID {
		ProCamPairID() { }
		ProCamPairID(const string & filename);

		uint8_t projector;
		uint8_t camera;
		bool operator<(const ProCamPairID & other) const;
		bool operator==(const ProCamPairID & other) const;
	};
}