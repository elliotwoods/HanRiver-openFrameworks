#pragma once
#include <stdint.h>

#include "ID.h"

namespace HanRiverLib {
	struct ProPixelID {
		ProID projector;
		PixelID projectorPixel;

		bool operator<(const ProPixelID & other) const;
	};
	typedef ProPixelID PPID;
}