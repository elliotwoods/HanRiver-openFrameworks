//
//  Wire.cpp
//  optimise2
//
#include "Wire.h"

Wire::Wire() {
	this->defined = false;
}

Wire::Wire(float radius, float height, const ofVec2f &center) {
	float thi = ofRandom(-PI,+PI);
	this->x = cos(thi) * radius + center.x;
	this->y = sin(thi) * radius + center.y;
	this->defined = true;
	this->height = height;
}

Element Wire::makeRandomElement(float elementHeight, float elementRadius) const {
	float y;
	for (int i=0; i<WIRE_FIND_RESOLUTION; i++) {
		y = ofRandom(0, height);
		Element testElement(ofVec3f(this->x, y, this->y), elementHeight, elementRadius);
		
		bool conflicts = false;
		std::set<Element*>::iterator it;
		const Element* element;
		for (it = elements.begin(); it != elements.end(); it++) {
			element = *it;
			conflicts |= testElement.conflictsInY(*element);
			if (conflicts)
				break;
		}
		
		if (!conflicts)
			return testElement;
	}
	
	//return an undefined element if we haven't found any position which desn't conflict
	return Element();
}

void Wire::insert(Element& element) {
	elements.insert(&element);
}

void Wire::draw() const {
	ofLine(this->x, -height/2.0f, this->y,
		   this->x, +height/2.0f, this->y);
}

int Wire::size() const {
	return elements.size();
}

bool Wire::operator<(const Wire& other) const {
	return this->lengthSquared() < other.lengthSquared();
}