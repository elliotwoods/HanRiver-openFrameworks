#include "ProCamPairID.h"

namespace HanRiverLib {
	//---------
	ProCamPairID::ProCamPairID(const string & filename) {
		string trunk = ofFilePath::getBaseName(filename);
		/**<WARNING>
		Here we presume filename format from scanTest3,4,5
		i.e. 
		c1-p1.dataset
		and that camera and projector indices are all single digit
		**/		
		this->camera = (uint8_t) (trunk[1] - '0');
		this->projector = (uint8_t) (trunk[4] - '0');
		/**</WARNING>**/
	}
	//---------
	bool ProCamPairID::operator<(const ProCamPairID & other) const {
		if (this->projector != other.projector)
			return this->projector < other.projector;
		return this->camera < other.camera;
	}

	//---------
	bool ProCamPairID::operator==(const ProCamPairID & other) const {
		return *(uint16_t*)this == *(uint16_t*) & other;
	}
}