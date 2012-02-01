//
//  WireSet.cpp
//  optimise2
//

#include "WireSet.h"

WireSet::WireSet(const ofVec3f& size, const ofVec2f& center, int targetElementTotal, int targetElementPerWire) : height(size.y) {
	this->volumeSize = size;
	this->center = center;
	this->targetElementPerWire1 = targetElementPerWire;
	this->targetElementTotal = targetElementTotal;
	this->wireDensity = float(targetElementTotal) / float(targetElementPerWire1) / (volumeSize.x * volumeSize.z);
	
	elementsMesh.setMode(OF_PRIMITIVE_POINTS);
	wiresMesh.setMode(OF_PRIMITIVE_POINTS);
	
	reset();
}

void WireSet::reset() {
	wires.clear();
	elements.clear();
	elementsMesh.clear();
	wiresMesh.clear();
	this->radius = 0;
	finishedWires = false;
}

void WireSet::addWire() {
	if (!tryAddWire())
		radius += RADIUS_STEP;
}

void WireSet::calc() {
	targetElementPerWire2 = targetElementTotal / wires.size();
	wiresIterator = wires.begin();
}

void WireSet::fillWire(const float elementHeight, const float elementRadius) {
	if (wiresIterator != wires.end()) {
		int nTries = 0;
		while (nTries < WIRESET_MAX_TRIES_PER_WIRE && (*wiresIterator).size() < targetElementPerWire2)
			tryAddElement(elementHeight, elementRadius);
		wiresIterator++;
	}
}

void WireSet::draw() {
//	wiresMesh.draw();
	elementsMesh.draw();
}

bool WireSet::isDensityHigh(const ofVec2f& position) const {
	//we're just excluding if anything is close enough
	const float radiusThresholdSquared = 1.0f / this->wireDensity;
	vector<Wire>::const_iterator it;
	for (it = wires.begin(); it != wires.end(); it++)
		if ((*it - position).lengthSquared() < radiusThresholdSquared)
			return true;
	
	return false;
}

bool WireSet::inside(const ofVec2f& position) const {
	//here we define the bounds of the WireSet
	ofRectangle bounds(-volumeSize.x/2,-volumeSize.y/2,volumeSize.x,volumeSize.y);
	return bounds.inside(position.x, position.y);
}

void WireSet::threadedFunction() {
	while (ofThread::isThreadRunning()) {
		lock();
		if (!finishedWires) {
			addWire();
			
			if (radius > 14.0f) {
				calc();
				finishedWires = true;
			}
		} else
			fillWire(0.04f, 0.02f);
		unlock();
	}
}

bool WireSet::tryAddWire() {
	// try and add a wire at this radius
	// check whether would exceed density
	// check whether within bounds of size
	for (int i=0; i<WIRESET_MAX_TRIES_PER_RADIUS; i++) {
		Wire wire(radius, height, center);
		if (!inside(wire))
			continue;
		if (!isDensityHigh(wire)) {
			wires.push_back(wire);
			return true;
		}
	}
	
	// if we reach here then we didn't find a suitable wire
	// return an undefined wire
	return false;
}

bool WireSet::tryAddElement(const float elementHeight, const float elementRadius) {
	if (wires.size() == 0)
		return false;
	
	//choose last wire
	Wire& wire(*wiresIterator);
	/**OPTION**/
	//we could instead choose a random wire each time
	
	//choose random position on that wire
	Element element = wire.makeRandomElement(elementHeight, elementRadius);
	if (element.defined == false)
		return false;
	//check if this element obfuscates existing elements 
	//in this instance we must also:
	//	1. each element must be aware of the viewpoints from which it is visible
	//	2. each element must have a minimum number of viewpoints. If viewpoints are to be from opposing sides, then we instead need a different measure of valid coverage of an element. e.g
	//		2.a. Minimum total angle of coverage (presumes cylindrical)
	//		2.b. 2 viewpoints must exist which are > 90degrees apart (or higher threshold) when you measure their polar position with respect to the element
	//
	// this might be a major distraction for the time being
	// let's focus on 1 viewpoint
	
	
	/**HACK**/
	//enable here for no occlusion checking
	elements.push_back(element);
	elementsMesh.addVertex(element.position);
	wire.insert(*elements.rbegin());
	return true;
	
	
	//check if this element obscures any existing element from a single viewpoint
	//
	// cycle through every other element and get the corner rays
	// check whether those corner rays intersect this element's plane section
	//
	/**OPTIMISE**/
	//we could check only elements on wires which could possibly contain elements which could be in the way (e.g. select a set of possible wires)
	vector<Element>::const_iterator it;
	ofPlane thisFace = element.getFace(viewpoint);
	ofPlane otherFace;
	ofRay otherCornerRays[4];
	for (it = elements.begin(); it != elements.end(); it++) {
		otherFace = it->getFace(viewpoint);
		otherFace.getCornerRaysFrom(viewpoint, otherCornerRays);
		for (int i=0; i<4; i++)
			if (thisFace.intersect(otherCornerRays[i]))
				return false;
	}
	
	// if we reach here, we're not obstructing any other faces
	// yipee!
	// let's add it
	elements.push_back(element);
	elementsMesh.addVertex(element.position);
	wire.insert(*elements.rbegin());
	return true;
}