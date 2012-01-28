#pragma once
//
//  Element.h
//  optimise1
//

#include "ofxRay.h"

//presume cylindrical element
class Element {
public:
	ofPlane getFace(const ofProjector& projector) const;
	
	ofVec3f position;
	float radius;
	float height;
};
