#include "ProCamPPixelID.h"
namespace HanRiverLib {
	//---------
	bool ProCamPPixelID::operator<(const ProCamPPixelID & other) const {
		if ( this->pcid == other.pcid)
			return this->projectorPixel < other.projectorPixel;
		return this->pcid < other.pcid;
	}

	//---------
	bool CamPPixelID::operator<(const CamPPixelID & other) const {
		if ( this->camera != other.camera)
			return this->camera < other.camera;
		return this->projectorPixel < other.projectorPixel;
	}
}