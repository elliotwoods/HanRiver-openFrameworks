//
//  Element.cpp
//  optimise1
//
#include "Element.h"

ofPlane Element::getFace(const ofProjector& projector) const {
	ofVec3f normal = projector.position - position;
	normal.y = 0.0f;
	normal.normalize();
	
	ofVec3f up(0,1,0);
	ofVec2f scale(2 * radius, height);
	return ofPlane(position, normal, up, scale);
}