//
//  WireSet.h
//  optimise2
//
#pragma once

#include "ofMain.h"
#include <set>
#include "Wire.h"
#include "Element.h"

#define WIRESET_MAX_TRIES_PER_RADIUS 10
#define WIRESET_FIND_DENSITY_THRESHOLD_SQ 1 / density;
#define RADIUS_STEP 0.0001;
#define WIRESET_MAX_TRIES_PER_WIRE 1000

class WireSet : public ofThread {
public:
	WireSet(const ofVec3f& size, const ofVec2f& center, int targetElementTotal, int targetElementPerWire);
	
	void reset();
	void addWire();
	void calc();
	void fillWire(const float elementHeight, const float elementRadius);
	
	void draw();
	
	bool isDensityHigh(const ofVec2f& position) const;
	bool inside(const ofVec2f& position) const;
	
	vector<Wire> wires;
	vector<Element> elements;	
	float radius;
	const float& height;
	
	///Temporary : single viewpoint
	ofVec3f viewpoint;

	ofVec3f volumeSize; ///< volume size in meters
	ofVec2f center; ///< where wires are built from
	float wireDensity; ///< wires per square meter
	int targetElementTotal; ///< total elements in whole system
	int targetElementPerWire1; ///< input target
	int targetElementPerWire2; ///< calculated target based on number of actual wires
	
protected:
	void threadedFunction();
	bool tryAddWire();
	bool tryAddElement(const float elementHeight, const float elementRadius);
	
	bool finishedWires;
	ofMesh elementsMesh;
	ofMesh wiresMesh;
	vector<Wire>::iterator wiresIterator;
	
};