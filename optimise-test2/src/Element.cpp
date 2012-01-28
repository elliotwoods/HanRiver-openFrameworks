//
//  Element.cpp
//  optimise1
//
#include "Element.h"

Element::Element() {
	this->defined = false;
}

Element::Element(const ofVec3f& position, float height, float radius) {
	this->position = position;
	this->height = height;
	this->radius = radius;
	this->defined = true;
}

ofPlane Element::getFace(const ofVec3f& viewpoint) const {
	ofVec3f normal = viewpoint - position;
	normal.y = 0.0f;
	normal.normalize();
	
	ofVec3f up(0,1,0);
	ofVec2f scale(2 * radius, height);
	return ofPlane(position, normal, up, scale);
}

bool Element::conflictsInY(const Element& other) const {
	float distance = other.position.y - this->position.y;
	
	//if distance -ve, then this is below the other
	return distance < 0 ? -distance < other.height : distance < this->height;
}