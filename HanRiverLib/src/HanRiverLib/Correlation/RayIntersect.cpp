#include "RayIntersect.h"

using namespace ofxRay;

namespace HanRiverLib {
	//---------
	ofVec3f RayIntersect::getCrossover() {
		float distance = std::numeric_limits<float>::infinity();
		ofVec3f center;
		RayIntersect:const_iterator it1, it2;
		ofRay intersect;
		for (it1 = this->begin(); it1 != this->end(); it1++) {
			it2 = it1;
			it2++;
			for (; it2 != this->end(); it2++) {
				intersect = it1->second.intersect(it2->second);
				if (intersect.getLength() < distance) {
					distance = intersect.getLength();
					center = intersect.getMidpoint();
				}
			}
		}
		return center;
	}
}