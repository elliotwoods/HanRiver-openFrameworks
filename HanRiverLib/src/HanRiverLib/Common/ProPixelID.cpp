#include "ProPixelID.h"
namespace HanRiverLib {
	//---------
	bool ProPixelID::operator<(const ProPixelID & other) const {
		if ( this->projector == other.projector)
			return this->projectorPixel < other.projectorPixel;
		return this->projector < other.projector;
	}
}