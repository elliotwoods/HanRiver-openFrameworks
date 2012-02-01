//
//  Wire.h
//  optimise2
//
#pragma once
#include "ofMain.h"
#include "Element.h"
#include <set>

#define WIRE_FIND_RESOLUTION 5

class Wire : public ofVec2f {
public:
	/// Creates an undefined wire
	Wire();
	
	/// Creates a wire at a random position at defined radius, with defined height
	Wire(float radius, float height, const ofVec2f &center);
	
	/// Defines an element in a spare space on this wire. returns an undefined Element if no space is found
	Element		makeRandomElement(float elementHeight, float elementRadius) const;
	
	void insert(Element& element);
	
	void draw() const;
	int size() const;
	bool operator<(const Wire& other) const;
	
protected:
	float height;
	bool defined;

	std::set<Element*> elements;
};