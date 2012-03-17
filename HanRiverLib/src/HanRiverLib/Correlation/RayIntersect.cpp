#include "RayIntersect.h"

using namespace ofxRay;

namespace HanRiverLib {
	//---------
	void RayIntersect::draw() const {
		for (RayIntersect::const_iterator it = this->begin(); it != this->end(); it++)
			it->second.draw();
	}

	//---------
	ofVec3f RayIntersect::getCrossover() const {
		float distance = std::numeric_limits<float>::infinity();
		ofVec3f center;
		RayIntersect:const_iterator it1, it2;
		ofRay intersect;
		for (it1 = this->begin(); it1 != this->end(); it1++) {
			it2 = it1;
			it2++;
			for (; it2 != this->end(); it2++) {
				if (it1->first == it2->first)
					continue; //CamID's are the same
				const ofxRay::Ray & first(it1->second);
				const ofxRay::Ray & second(it2->second);
				intersect = first.intersect(second);
				if (intersect.getLength() < distance) {
					distance = intersect.getLength();
					center = intersect.getMidpoint();
				}
			}
		}
		return center;
	}
}