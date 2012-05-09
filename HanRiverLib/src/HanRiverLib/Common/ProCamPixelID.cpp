#include "ProCamPixelID.h"
namespace HanRiverLib {
	//---------
	bool ProCamPixelID::operator<(const ProCamPixelID & other) const {
		if ( this->proCam == other.proCam)
			return this->pixelID < other.pixelID;
		return this->proCam < other.proCam;
	}

	//---------
	bool CamPixelIDXY::operator<(const CamPixelIDXY & other) const {
		if ( this->camera == other.camera) {
			if (this->xy.y == other.xy.y) {
				return this->xy.y < other.xy.y;
			} else {
				return this->xy.x < other.xy.x;
			}
		} else
			return this->camera < other.camera;
	}
}