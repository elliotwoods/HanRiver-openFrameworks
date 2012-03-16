#pragma once
#include "ofFileUtils.h"

#include <stdint.h>
#include <string>

namespace HanRiverLib {
	struct ProCamID {
		ProCamID() { }
		ProCamID(const string & filename);

		uint8_t projector;
		uint8_t camera;
		bool operator<(const ProCamID & other) const;
		bool operator==(const ProCamID & other) const;
	};
}