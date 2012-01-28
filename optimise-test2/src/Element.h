#pragma once
//
//  Element.h
//  optimise1
//

#include "ofxRay.h"

//presume cylindrical element hanging at position
class Element {
public:
	Element();
	Element(const ofVec3f& position, float height, float radius);
	ofPlane getFace(const ofVec3f& viewpoint) const;
	
	bool conflictsInY(const Element& other) const;
	
	///Position of top of element
	ofVec3f position;
	float radius;
	float height;
	bool defined;
};
