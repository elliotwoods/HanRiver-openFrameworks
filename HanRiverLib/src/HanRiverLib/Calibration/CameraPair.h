#pragma once
#include <stdint.h>

namespace HanRiverLib {
	struct CameraPair {
		CameraPair(uint16_t first, uint16_t second) :
				first(first < second ? first : second),
				second(first < second ? second : first) { }

		bool operator<(const CameraPair& other) const {
			return this->asInt32() < other.asInt32();
		}

		uint32_t asInt32() const {
			return ( (uint32_t)first << 16 ) + ( (uint32_t)second );
		}
		const uint16_t first;
		const uint16_t second;
	};
}